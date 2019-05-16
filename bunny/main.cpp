#include <Windows.h>
#include <iostream>
#include "connector.h"
CHackProcess fProcess;

using namespace std;

#define onGround 257
#define crouchedGround	263

const DWORD dwPlayerBase = 0xCF1A3C;
const DWORD dwJump = 0x51A7090;
const DWORD entityList = 0x4D03B24;
const DWORD m_fFlags = 0x104;
const DWORD flashAlpha = 0xA3F0;
const DWORD isSpotted = 0x93D;
const DWORD dwCham = 0x70;
const DWORD dwTeam = 0xF4;
const DWORD dwCrossID = 0xB3AC;
const DWORD dwModelAmb = 0x58ED1C; //for brightness

bool bBhop = false;
bool bflash = false;
bool bRadar = false;
bool bChams = false;
bool bTrigger = false;

struct myPlayer_T
{
	DWORD dwLocalP = 0x0;
	int flag = 0;
	int flash = 0;
	int iTeam = 0;
	int iHealth = 0;
	int iCrossID = 0;
	void ReadInfo()
	{
		//Read this all the time..
		ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(fProcess.__dwordClient + dwPlayerBase), &dwLocalP, sizeof(DWORD), 0);
		ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(dwLocalP + m_fFlags), &flag, sizeof(int), 0);
		ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(dwLocalP + flashAlpha), &flash, sizeof(int), 0);
		ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(dwLocalP + dwTeam), &iTeam, sizeof(int), 0);
		ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(dwLocalP + dwCrossID), &iCrossID, sizeof(int), 0);
	}
}myPlayer;

void bunny()
{
	if (bBhop)
	{
		int doJump = 5;
		int stopJump = 4;

		if (myPlayer.flag == onGround || myPlayer.flag == crouchedGround)
		{
			WriteProcessMemory(fProcess.__HandleProcess, (PBYTE*)(fProcess.__dwordClient + dwJump), &doJump, sizeof(doJump), 0);
		}
		else
		{
			WriteProcessMemory(fProcess.__HandleProcess, (PBYTE*)(fProcess.__dwordClient + dwJump), &stopJump, sizeof(stopJump), 0);
		}
	}
	else
	{
		return;
	}
	
}

void flash()
{
	float newAlphaFlash = .2f;

	if (bflash)
	{
		if (myPlayer.flash > .5f)
		{
			WriteProcessMemory(fProcess.__HandleProcess, (PBYTE*)(myPlayer.dwLocalP + flashAlpha), &newAlphaFlash, sizeof(newAlphaFlash), 0);
		}
	}
	else
	{
		return;
	}
}

void radar()
{
	if (bRadar)
	{
		DWORD entity = 0x0;
		bool bSpot = true;
		for (int i = 0; i < 64; i++)
		{
			//Radar
			ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(fProcess.__dwordClient + entityList + (i * 0x10)), &entity, sizeof(DWORD), 0);
			//Spot
			if (entity != NULL)
			{
				WriteProcessMemory(fProcess.__HandleProcess, (PBYTE*)(entity + isSpotted), &bSpot, sizeof(bool), 0);
			}
		}
	}
	else
	{
		return;
	}
}

void drawChams()
{
	if (bChams)
	{
		DWORD entity = 0x0;
		int enemyTeam = 0; //actually EntityTeam
		//no need for alpha value because brightness does the work for us
		byte rgbColor[3]= {33, 103, 255}; //cyan
		byte rgbColorEnemy[3] = {255, 25, 155}; //pink
		float brightness = 65.f; //65 is very bright
		DWORD thisPtr = (int)(fProcess.__dwordEngine + dwModelAmb - 0x2c);
		DWORD xored = *(DWORD*)&brightness ^ thisPtr;
		
		for (int i = 0; i < 64; i++)
		{
			ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(fProcess.__dwordClient + entityList + (i * 0x10)), &entity, sizeof(DWORD), 0);
			ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(entity + dwTeam), &enemyTeam, sizeof(int), 0);
			if (entity != NULL && enemyTeam != myPlayer.iTeam)
			{
				WriteProcessMemory(fProcess.__HandleProcess, (PBYTE*)(entity + dwCham), &rgbColorEnemy, sizeof(rgbColorEnemy), 0);				
			}
			else if (entity != NULL && enemyTeam == myPlayer.iTeam)
			{
				WriteProcessMemory(fProcess.__HandleProcess, (PBYTE*)(entity + dwCham), &rgbColor, sizeof(rgbColor), 0);
			}
		}
		WriteProcessMemory(fProcess.__HandleProcess, (PBYTE*)(fProcess.__dwordEngine + dwModelAmb), &xored, sizeof(int), 0);
	}
	else
	{
		//reset
		DWORD entity = 0x0;
		int enemyTeam = 0;
		byte rgbColor[3] = {255, 255, 255};
		byte rgbColorEnemy[3] = {255, 255, 255};
		float resetBrightness = 0.f;
		DWORD thisPtr = (int)(fProcess.__dwordEngine + dwModelAmb - 0x2c);
		DWORD resetXored = *(DWORD*)&resetBrightness ^ thisPtr;
		for (int i = 0; i < 64; i++)
		{
			ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(fProcess.__dwordClient + entityList + (i * 0x10)), &entity, sizeof(DWORD), 0);
			ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(entity + dwTeam), &enemyTeam, sizeof(int), 0);
			if (entity != NULL && enemyTeam != myPlayer.iTeam)
			{
				WriteProcessMemory(fProcess.__HandleProcess, (PBYTE*)(entity + dwCham), &rgbColorEnemy, sizeof(rgbColorEnemy), 0);
			}
			else if (entity != NULL && enemyTeam == myPlayer.iTeam)
			{
				WriteProcessMemory(fProcess.__HandleProcess, (PBYTE*)(entity + dwCham), &rgbColor, sizeof(rgbColor), 0);
			}
		}
		WriteProcessMemory(fProcess.__HandleProcess, (PBYTE*)(fProcess.__dwordEngine + dwModelAmb), &resetXored, sizeof(int), 0);
	}	
}

void Trigger()
{
	DWORD aimedEntity = 0x0;
	DWORD entity = 0x0;
	int enemyTeam = 0;
	if (bTrigger)
	{
		int AimedAt = myPlayer.iCrossID;
		if (AimedAt > 0 && AimedAt < 64) 
		{
			ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(fProcess.__dwordClient + entityList + (AimedAt - 1) * (0x10)), &aimedEntity, sizeof(DWORD), 0);
		}
		for (int i = 0; i < 64; i++)
		{
			ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(aimedEntity + dwTeam), &enemyTeam, sizeof(int), 0);
		}
		if (myPlayer.iCrossID > 0 && enemyTeam != myPlayer.iTeam)
		{
			Sleep(14); 
			mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
			Sleep(10); 
			mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
		}
	}
	else
	{
		return;
	}
}

int main(void)
{
	fProcess.RunProcess();    //always forgetting this line...
	if (fProcess.__HWNDCSgo == NULL) //if window not found
	{
		cout << "Process 'csgo.exe' not found!" << endl;
		cout << "Last Error: " << GetLastError() << endl;
	}
	else
	{
		cout << "Bunny by c1tru5x - NOW WITH GLOWY SHIT!\nUPDATED: 16.MAI.19" << endl;
		cout << "F11 to close!" << endl;
		cout << "[F1] BHOP use SPACE" << endl;
		cout << "[F2] No Flash!" << endl;
		cout << "[F3] Radar" << endl;
		cout << "[F4] Chams" << endl;
		cout << "[F5] Trigger use ALT" << endl;

		while (!GetAsyncKeyState(VK_F11))
		{
			myPlayer.ReadInfo();
			if (GetAsyncKeyState(VK_F1))
			{
				bBhop = !bBhop;
				if (bBhop == false)
				{
					Beep(250, 200);
				}
				else
				{
					Beep(400, 200);
				}
			}
			if (GetAsyncKeyState(VK_F2))
			{
				bflash = !bflash;
				if (bflash == false)
				{
					Beep(250, 200);
				}
				else
				{
					Beep(400, 200);
				}
			}
			if (GetAsyncKeyState(VK_F3))
			{
				bRadar = !bRadar;
				if (bRadar == false)
				{
					Beep(250, 200);
				}
				else
				{
					Beep(400, 200);
				}
			}
			if (GetAsyncKeyState(VK_F4))
			{
				bChams = !bChams;
				if (bChams == false)
				{
					Beep(250, 200);
				}
				else
				{
					Beep(400, 200);
				}
			}
			if (GetAsyncKeyState(VK_F5))
			{
				bTrigger = !bTrigger;
				if (bTrigger == false)
				{
					Beep(250, 200);
				}
				else
				{
					Beep(400, 200);
				}
			}
			//Functioncall
			if (bTrigger == true)
			{
				if (GetAsyncKeyState(VK_LMENU))
				{
					Trigger();
				}
			}
			if (bBhop == true)
			{
				if (GetAsyncKeyState(VK_SPACE))
				{
					bunny();
				}
			}
			flash();
			radar();
			drawChams();
			Sleep(1);
		}
	}
	return 0;
}
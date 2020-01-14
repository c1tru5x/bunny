#include <Windows.h>
#include <iostream>
#include <vector>
#include <math.h>
#include <thread>
#include <chrono>
#include "connector.h"
#include "timercpp.h"
#include "csgo.hpp"

using namespace hazedumper::netvars;
using namespace hazedumper::signatures;

CHackProcess fProcess;

DWORD dwCham = 0x70;

bool bflash = false;
bool bRadar = false;
bool bChams = false;
bool bTrigger = false;
bool bDefuse = false;

struct myPlayer_T
{
	DWORD dwLocalP = 0x0;
	int flash = 0;
	int iTeam = 0;
	int iHealth = 0;
	int iCrossID = 0;
	
	void ReadInfo()
	{
		//Read this all the time..
		ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(fProcess.__dwordClient + dwLocalPlayer), &dwLocalP, sizeof(DWORD), 0);
		ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(dwLocalP + m_flFlashMaxAlpha), &flash, sizeof(int), 0);
		ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(dwLocalP + m_iTeamNum), &iTeam, sizeof(int), 0);
		ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(dwLocalP + m_iCrosshairId), &iCrossID, sizeof(int), 0);
		ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(dwLocalP + m_iHealth), &iHealth, sizeof(int), 0);
	}
}myPlayer;

void flash()
{
	float newAlphaFlash = .2f;

	if (bflash)
	{
		if (myPlayer.flash > .5f)
		{
			WriteProcessMemory(fProcess.__HandleProcess, (PBYTE*)(myPlayer.dwLocalP + m_flFlashMaxAlpha), &newAlphaFlash, sizeof(newAlphaFlash), 0);
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
			ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(fProcess.__dwordClient + dwEntityList + (i * 0x10)), &entity, sizeof(DWORD), 0);
			//Spot
			if (entity != NULL)
			{
				WriteProcessMemory(fProcess.__HandleProcess, (PBYTE*)(entity + m_bSpotted), &bSpot, sizeof(bool), 0);
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
		bool hasKit = 0;
		int enemyTeam = 0; //actually EntityTeam
		int health = 0;
		//no need for alpha value because brightness does the work for us
		byte rgbColor[3]= {33, 103, 255}; //cyan
		byte rgbColorEnemy[3] = {255, 25, 155}; //pink
		byte rgbColorEnemyLow[3] = {255,140,0}; //orange
		byte rgbColorDefuser[3] = { 0, 0, 255 }; //pure blue
		byte rgbColorEnemyDefuser[3] = { 255, 0, 0 }; //pure red
		float brightness = 15.f; //65 is very bright
		DWORD thisPtr = (int)(fProcess.__dwordEngine + model_ambient_min - 0x2c);
		DWORD xored = *(DWORD*)&brightness ^ thisPtr;
		
		for (int i = 0; i < 64; i++)
		{
			ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(fProcess.__dwordClient + dwEntityList + (i * 0x10)), &entity, sizeof(DWORD), 0);
			ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(entity + m_iTeamNum), &enemyTeam, sizeof(int), 0);
			ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(entity + m_bHasDefuser), &hasKit, sizeof(bool), 0);
			ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(entity + m_iHealth), &health, sizeof(bool), 0);
			if (entity != NULL && enemyTeam != myPlayer.iTeam)
			{
				//Defuse Boi red
				if (hasKit && health > 40)
				{
					WriteProcessMemory(fProcess.__HandleProcess, (PBYTE*)(entity + dwCham), &rgbColorEnemyDefuser, sizeof(rgbColorEnemyDefuser), 0);
				}
				else if (hasKit && health <= 40)
				{
					WriteProcessMemory(fProcess.__HandleProcess, (PBYTE*)(entity + dwCham), &rgbColorEnemyDefuser, sizeof(rgbColorEnemyDefuser), 0);
					Sleep(2);
					WriteProcessMemory(fProcess.__HandleProcess, (PBYTE*)(entity + dwCham), &rgbColorEnemyLow, sizeof(rgbColorEnemyLow), 0);
					Sleep(2);
				}
				//Normal Boi pink
				else
				{
					if (health > 40)
					{
						WriteProcessMemory(fProcess.__HandleProcess, (PBYTE*)(entity + dwCham), &rgbColorEnemy, sizeof(rgbColorEnemy), 0);
					}
					else
					{
						WriteProcessMemory(fProcess.__HandleProcess, (PBYTE*)(entity + dwCham), &rgbColorEnemyLow, sizeof(rgbColorEnemyLow), 0);
					}
				}
			}
			else if (entity != NULL && enemyTeam == myPlayer.iTeam)
			{
				if (hasKit)
				{
					WriteProcessMemory(fProcess.__HandleProcess, (PBYTE*)(entity + dwCham), &rgbColorDefuser, sizeof(rgbColorDefuser), 0);
				}
				else
				{
					WriteProcessMemory(fProcess.__HandleProcess, (PBYTE*)(entity + dwCham), &rgbColor, sizeof(rgbColor), 0);
				}
			}
		}
		WriteProcessMemory(fProcess.__HandleProcess, (PBYTE*)(fProcess.__dwordEngine + model_ambient_min), &xored, sizeof(int), 0);
	}
	else
	{
		//reset
		DWORD entity = 0x0;
		int enemyTeam = 0;
		byte rgbColor[3] = {255, 255, 255};
		byte rgbColorEnemy[3] = {255, 255, 255};
		float resetBrightness = 0.f;
		DWORD thisPtr = (int)(fProcess.__dwordEngine + model_ambient_min - 0x2c);
		DWORD resetXored = *(DWORD*)&resetBrightness ^ thisPtr;
		for (int i = 0; i < 64; i++)
		{
			ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(fProcess.__dwordClient + dwEntityList + (i * 0x10)), &entity, sizeof(DWORD), 0);
			ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(entity + m_iTeamNum), &enemyTeam, sizeof(int), 0);
			if (entity != NULL && enemyTeam != myPlayer.iTeam)
			{
				WriteProcessMemory(fProcess.__HandleProcess, (PBYTE*)(entity + dwCham), &rgbColorEnemy, sizeof(rgbColorEnemy), 0);
			}
			else if (entity != NULL && enemyTeam == myPlayer.iTeam)
			{
				WriteProcessMemory(fProcess.__HandleProcess, (PBYTE*)(entity + dwCham), &rgbColor, sizeof(rgbColor), 0);
			}
		}
		WriteProcessMemory(fProcess.__HandleProcess, (PBYTE*)(fProcess.__dwordEngine + model_ambient_min), &resetXored, sizeof(int), 0);
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
			ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(fProcess.__dwordClient + dwEntityList + (AimedAt - 1) * (0x10)), &aimedEntity, sizeof(DWORD), 0);
		}
		for (int i = 0; i < 64; i++)
		{
			ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(aimedEntity + m_iTeamNum), &enemyTeam, sizeof(int), 0);
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

void checkDefuse()
{
	DWORD entity = 0x0;
	int enemyTeam = 0; //actually EntityTeam
	bool defusing = 0;
	bool hasKit = 0;

	for (int i = 0; i < 64; i++)
	{
		ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(fProcess.__dwordClient + dwEntityList + (i * 0x10)), &entity, sizeof(DWORD), 0);
		ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(entity + m_iTeamNum), &enemyTeam, sizeof(int), 0);
		ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(entity + m_bIsDefusing), &defusing, sizeof(bool), 0);
		ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(entity + m_bHasDefuser), &hasKit, sizeof(bool), 0);
		if (entity != NULL && enemyTeam != myPlayer.iTeam)
		{
			if (defusing && hasKit)
			{
				Beep(550, 70); //lower interval if has defuserkit and higher freq
			}
			if (defusing && !hasKit)
			{
				Beep(350, 120);
			}
		}
	}
}

int main(void)
{
    SetConsoleTitle("Bunny but no BHOP!");
	fProcess.RunProcess();    //always forgetting this line...
		std::cout << "Made by c1tru5x [14.Jan.2020]" << std::endl;
		std::cout << "-------------------------" << std::endl;
		std::cout << "F11 to close!" << std::endl;
		std::cout << "[NUM1] No Flash" << std::endl;
		std::cout << "[NUM2] Radar" << std::endl;
		std::cout << "[NUM3] Chams" << std::endl;
		std::cout << "[NUM4] Trigger use [ALT]" << std::endl;
		std::cout << "[NUM5] Check for Defuse" << std::endl;
		
		while (!GetAsyncKeyState(VK_F11))
		{
            //Read all the time.
			myPlayer.ReadInfo();
			if (GetAsyncKeyState(VK_NUMPAD1))
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
			if (GetAsyncKeyState(VK_NUMPAD2))
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
			if (GetAsyncKeyState(VK_NUMPAD3))
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
			if (GetAsyncKeyState(VK_NUMPAD4))
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
			if (GetAsyncKeyState(VK_NUMPAD5))
			{
				bDefuse = !bDefuse;
				if (bDefuse == false)
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
			if (bflash == true)
			{
				flash();
			}
			if (bRadar == true)
			{
				radar();
			}
			if (bDefuse == true)
			{
				checkDefuse();
			}
			drawChams(); //Needs to be outside so it can reset the color back
			Sleep(1);
	}
	return 0;
}
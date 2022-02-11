#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <Windows.h>

#include "connector.h"
#include "json.hpp"
#include "OffsetParser.h"

//+++++++++++++++++Compile with C++17 and as 32 Bit!+++++++++++++++++++++++++

namespace fs = std::filesystem;
#pragma comment(lib, "urlmon.lib")
using namespace OffsettParser;

CHackProcess fProcess;

static Offsets offsets;

bool bflash = false;
bool bRadar = false;
bool bChams = false;
bool bTrigger = false;
bool bDefuse = false;
bool bWall = false;
bool bHop = false;

//toggle everything
bool bAll = false;

constexpr auto onGround = 257;
constexpr auto crouchedGround = 263;

void updateJson();

struct myPlayer_T
{
	DWORD dwLocalP = 0x0;
	int flash = 0;
	int iTeam = 0;
	int iHealth = 0;
	int iCrossID = 0;
	int iFlag = 0;

	void ReadInfo()
	{
		//Read this all the time..
		ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(fProcess.__dwordClient + offsets.signatures.dw_local_player), &dwLocalP, sizeof(DWORD), 0);
		ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(dwLocalP + offsets.netvars.m_fl_flash_max_alpha), &flash, sizeof(int), 0);
		ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(dwLocalP + offsets.netvars.m_i_team_num), &iTeam, sizeof(int), 0);
		ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(dwLocalP + offsets.netvars.m_i_crosshair_id), &iCrossID, sizeof(int), 0);
		ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(dwLocalP + offsets.netvars.m_i_health), &iHealth, sizeof(int), 0);
		ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(dwLocalP + offsets.netvars.m_f_flags), &iFlag, sizeof(int), 0);
	}
}myPlayer;


Offsets parseJson(const std::string& filePath) {
	// read a JSON file
	std::ifstream i(filePath);
	json j;
	i >> j;

	OffsettParser::Offsets parsedData = nlohmann::json::parse(j.dump());
	return parsedData;
}

void flash()
{
	float newAlphaFlash = 90.f;
	float maxAlpha = 255.f;

	if (bflash)
	{
		if (myPlayer.flash > 100.f)
		{
			WriteProcessMemory(fProcess.__HandleProcess, (PBYTE*)(myPlayer.dwLocalP + offsets.netvars.m_fl_flash_max_alpha), &newAlphaFlash, sizeof(newAlphaFlash), 0);
		}
	}
	else
	{
		WriteProcessMemory(fProcess.__HandleProcess, (PBYTE*)(myPlayer.dwLocalP + offsets.netvars.m_fl_flash_max_alpha), &maxAlpha, sizeof(maxAlpha), 0);
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
			ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(fProcess.__dwordClient + offsets.signatures.dw_entity_list + (i * 0x10)), &entity, sizeof(DWORD), 0);
			//Spot
			if (entity != NULL)
			{
				WriteProcessMemory(fProcess.__HandleProcess, (PBYTE*)(entity + offsets.netvars.m_b_spotted), &bSpot, sizeof(bool), 0);
			}
		}
	}
}

void drawChams()
{
    DWORD dwCham = 0x70;

	if (bChams)
	{
		DWORD entity = 0x0;
		bool hasKit = 0;
		int entityTeam = 0; //actually EntityTeam
		int health = 0;
		//no need for alpha value because brightness does the work for us
		byte rgbColor[3]= {33, 103, 255}; //cyan
		byte rgbColorEnemy[3] = {255, 25, 155}; //pink
		byte rgbColorEnemyLow[3] = {255, 140, 0}; //orange
		byte rgbColorDefuser[3] = {0, 0, 255}; //pure blue
		byte rgbColorEnemyDefuser[3] = {255, 0, 0}; //pure red
		float brightness = 10.f; //65 is very bright
		DWORD thisPtr = (int)(fProcess.__dwordEngine + offsets.signatures.model_ambient_min - 0x2c); //0x2c standard
		DWORD xored = *(DWORD*)&brightness ^ thisPtr;
		
		for (int i = 0; i < 32; i++) //testing 1 to see if it skips localplayer.
		{
			ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(fProcess.__dwordClient + offsets.signatures.dw_entity_list + ((i - 1)* 0x10)), &entity, sizeof(DWORD), 0);
			ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(entity + offsets.netvars.m_i_team_num), &entityTeam, sizeof(int), 0);
			ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(entity + offsets.netvars.m_b_has_defuser), &hasKit, sizeof(bool), 0);
			ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(entity + offsets.netvars.m_i_health), &health, sizeof(bool), 0);
			if (entity != NULL && entityTeam != myPlayer.iTeam)
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
			else if (entity != NULL && entityTeam == myPlayer.iTeam)
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
		WriteProcessMemory(fProcess.__HandleProcess, (PBYTE*)(fProcess.__dwordEngine + offsets.signatures.model_ambient_min), &xored, sizeof(int), 0);
	}
	else
	{
		//reset
		DWORD entity = 0x0;
		int entityTeam = 0;
		byte rgbColor[3] = {255, 255, 255};
		byte rgbColorEnemy[3] = {255, 255, 255};
		float resetBrightness = 0.f;
		DWORD thisPtr = (int)(fProcess.__dwordEngine + offsets.signatures.model_ambient_min - 0x2c);
		DWORD resetXored = *(DWORD*)&resetBrightness ^ thisPtr;
		for (int i = 0; i < 32; i++)
		{
			ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(fProcess.__dwordClient + offsets.signatures.dw_entity_list + ((i - 1) * 0x10)), &entity, sizeof(DWORD), 0);
			ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(entity + offsets.netvars.m_i_team_num), &entityTeam, sizeof(int), 0);
			if (entity != NULL && entityTeam != myPlayer.iTeam)
			{
				WriteProcessMemory(fProcess.__HandleProcess, (PBYTE*)(entity + dwCham), &rgbColorEnemy, sizeof(rgbColorEnemy), 0);
			}
			else if (entity != NULL && entityTeam == myPlayer.iTeam)
			{
				WriteProcessMemory(fProcess.__HandleProcess, (PBYTE*)(entity + dwCham), &rgbColor, sizeof(rgbColor), 0);
			}
		}
		WriteProcessMemory(fProcess.__HandleProcess, (PBYTE*)(fProcess.__dwordEngine + offsets.signatures.model_ambient_min), &resetXored, sizeof(int), 0);
	}	
}

void Trigger()
{
	DWORD aimedEntity = 0x0;
	int entityTeam = 0;
	if (bTrigger)
	{
		int AimedAt = myPlayer.iCrossID;
		if (AimedAt > 0 && AimedAt < 32) 
		{
			ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(fProcess.__dwordClient + offsets.signatures.dw_entity_list + (AimedAt - 1) * (0x10)), &aimedEntity, sizeof(DWORD), 0);
		}
		for (int i = 0; i < 32; i++)
		{
			ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(aimedEntity + offsets.netvars.m_i_team_num), &entityTeam, sizeof(int), 0);
		}
		if (myPlayer.iCrossID > 0 && entityTeam != myPlayer.iTeam)
		{
			Sleep(14); 
			mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
			Sleep(10); 
			mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
		}
	}
}

void checkDefuse()
{
	if (bDefuse)
	{
		DWORD entity = 0x0;
		int entityTeam = 0; //actually EntityTeam
		bool defusing = 0;
		bool hasKit = 0;

		for (int i = 0; i < 64; i++)
		{
			ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(fProcess.__dwordClient + offsets.signatures.dw_entity_list + (i * 0x10)), &entity, sizeof(DWORD), 0);
			ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(entity + offsets.netvars.m_i_team_num), &entityTeam, sizeof(int), 0);
			ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(entity + offsets.netvars.m_b_is_defusing), &defusing, sizeof(bool), 0);
			ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(entity + offsets.netvars.m_b_has_defuser), &hasKit, sizeof(bool), 0);
			if (entity != NULL && entityTeam != myPlayer.iTeam)
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
}


void wall()
{
	if(bWall)
	{ 
		DWORD glowObj = 0x0;
		DWORD entity = 0x0;

		int glowIndex = 0;
		int entityTeam = 0; //actually EntityTeam

		bool bOccluded = true;

		float full = 1.f; //255
		float alpha = .7f;

		ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(fProcess.__dwordClient + offsets.signatures.dw_glow_object_manager), &glowObj, sizeof(DWORD), 0);

		for (int i = 0; i < 32; i++)
		{
			ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(fProcess.__dwordClient + offsets.signatures.dw_entity_list + (i * 0x10)), &entity, sizeof(DWORD), 0);
			ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(entity + offsets.netvars.m_i_team_num), &entityTeam, sizeof(int), 0);
			ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(entity + offsets.netvars.m_i_glow_index), &glowIndex, sizeof(int), 0);

			if (entity != NULL && entityTeam != myPlayer.iTeam) //Find Enemy
			{
				//Show outlines
				WriteProcessMemory(fProcess.__HandleProcess, (PBYTE*)(glowObj + (glowIndex * 0x38) + 0x8), &full, sizeof(float), 0); //red
				//WriteProcessMemory(fProcess.__HandleProcess, (PBYTE*)(glowObj + (glowIndex * 0x38) + 0xC), 0, sizeof(float), 0); //green
				//WriteProcessMemory(fProcess.__HandleProcess, (PBYTE*)(glowObj + (glowIndex * 0x38) + 0x10), 0, sizeof(float), 0); //blue
				WriteProcessMemory(fProcess.__HandleProcess, (PBYTE*)(glowObj + (glowIndex * 0x38) + 0x14), &alpha, sizeof(float), 0); //alpha
			}     
			//Enables Outline !!!! Don't delete
			WriteProcessMemory(fProcess.__HandleProcess, (PBYTE*)(glowObj + (glowIndex * 0x38) + 0x28), &bOccluded, sizeof(bool), 0);
		}
	}
}

void bunny()
{
	if (bHop)
	{
		int doJump = 5;
		int stopJump = 4;

		if (myPlayer.iFlag == onGround || myPlayer.iFlag == crouchedGround)
		{
			WriteProcessMemory(fProcess.__HandleProcess, (PBYTE*)(fProcess.__dwordClient + offsets.signatures.dw_force_jump), &doJump, sizeof(doJump), 0);
		}
		else
		{
			WriteProcessMemory(fProcess.__HandleProcess, (PBYTE*)(fProcess.__dwordClient + offsets.signatures.dw_force_jump), &stopJump, sizeof(stopJump), 0);
		}
	}
}

void allOn()
{
	bflash = true;
	bRadar = true;
	bChams = true;
	bTrigger = true;
	bDefuse = true;
	bWall = true;
	bHop = true;
}

void allOff()
{
	bflash = false;
	bRadar = false;
	bChams = false;
	bTrigger = false;
	bDefuse = false;
	bWall = false;
	bHop = false;
}

void updateJson() 
{
	std::string dwnld_URL = "https://raw.githubusercontent.com/frk1/hazedumper/master/csgo.json";
	std::string savepath = "./hazedumperRepo/csgo.json";
	fs::create_directories("./hazedumperRepo");
	URLDownloadToFile(nullptr, dwnld_URL.c_str(), savepath.c_str(), 0, nullptr);
}

void updateMenu()
{
	system("cls");
	std::cout << "Made by c1tru5x and maxiangelo -- Compiled: " << __DATE__ << std::endl;
	std::cout << "-------------------------" << std::endl;
	std::cout << "F11 to close!" << std::endl;
	(bAll) ? std::cout << "on  | [NUM0] Toggle Everything" << std::endl : std::cout << "off | [NUM0] Toggle Everything" << std::endl;
	(bflash) ? std::cout << "on  | [NUM1] No Flash" << std::endl : std::cout << "off | [NUM1] No Flash" << std::endl;
	(bRadar) ? std::cout << "on  | [NUM2] Radar" << std::endl : std::cout << "off | [NUM2] Radar" << std::endl;
	(bChams) ? std::cout << "on  | [NUM3] Chams" << std::endl : std::cout << "off | [NUM3] Chams" << std::endl;
	(bTrigger) ? std::cout << "on  | [NUM4] Trigger" << std::endl : std::cout << "off | [NUM4] Trigger" << std::endl;
	(bDefuse) ? std::cout << "on  | [NUM5] Defuse Checker" << std::endl : std::cout << "off | [NUM5] Defuse Checker" << std::endl;
	(bWall) ? std::cout << "on  | [NUM6] Walls" << std::endl : std::cout << "off | [NUM6] Walls" << std::endl;
	(bHop) ? std::cout << "on  | [NUM7] BHOP" << std::endl : std::cout << "off | [NUM7] BHOP" << std::endl;
}

int main(void)
{
	updateJson();
	offsets = parseJson("./hazedumperRepo/csgo.json");

    fProcess.RunProcess();    //always forgetting this line...
	updateMenu();
    	
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
				updateMenu();
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
				updateMenu();
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
				updateMenu();
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
				updateMenu();
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
				updateMenu();
			}
            if (GetAsyncKeyState(VK_NUMPAD6))
            {
                bWall = !bWall;
                if (bWall == false)
                {
                    Beep(250, 200);
                }
                else
                {
                    Beep(400, 200);
                }
				updateMenu();
            }
			if (GetAsyncKeyState(VK_NUMPAD7))
			{
				bHop = !bHop;
				if (bHop == false)
				{
					Beep(250, 200);
				}
				else
				{
					Beep(400, 200);
				}
				updateMenu();
			}
			if (GetAsyncKeyState(VK_NUMPAD0))
			{
				bAll = !bAll;
				if (bAll == false)
				{
					allOff();
					Beep(250, 200);
				}
				else
				{
					allOn();
					Beep(400, 200);
				}
				updateMenu();
			}
			//This needs to run all the Time
			flash();
			radar();
			drawChams();
			if (GetAsyncKeyState(VK_LMENU)) //Alt Key
			{
				Trigger();
			}
			checkDefuse();
			wall();
			if (GetAsyncKeyState(VK_SPACE))
			{
				bunny();
			}
			Sleep(1);
	}
	return 0;
}
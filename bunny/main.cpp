#include <Windows.h>
#include <iostream>
#include <vector>
#include "OffsetParser.h"
#include "connector.h"
#include <fstream>
#include<string>
#include <filesystem>
#include "sound.h"

namespace fs = std::experimental::filesystem;
#pragma comment(lib, "urlmon.lib")

using namespace OffsettParser;

CHackProcess fProcess;

inline static Offsets offsets;

bool bflash = false;
bool bRadar = false;
bool bChams = false;
bool bTrigger = false;
bool bDefuse = false;
bool bWall = false;
bool masterSwitch = true;
bool dangerMode = false;

struct myPlayer_T {
    DWORD dwLocalP = 0x0;
    int flag = 0;
    int flash = 0;
    int iTeam = 0;
    int iHealth = 0;
    int iCrossID = 0;

    void ReadInfo() {
        //Read this all the time..
        ReadProcessMemory(fProcess.__HandleProcess,
                          (PBYTE *) (fProcess.__dwordClient + offsets.signatures.dw_local_player), &dwLocalP,
                          sizeof(DWORD), 0);
        ReadProcessMemory(fProcess.__HandleProcess, (PBYTE *) (dwLocalP + offsets.netvars.m_f_flags), &flag,
                          sizeof(int), 0);
        ReadProcessMemory(fProcess.__HandleProcess, (PBYTE *) (dwLocalP + offsets.netvars.m_fl_flash_max_alpha), &flash,
                          sizeof(int), 0);
        ReadProcessMemory(fProcess.__HandleProcess, (PBYTE *) (dwLocalP + offsets.netvars.m_i_team_num), &iTeam,
                          sizeof(int), 0);
        ReadProcessMemory(fProcess.__HandleProcess, (PBYTE *) (dwLocalP + offsets.netvars.m_i_crosshair_id), &iCrossID,
                          sizeof(int), 0);
        ReadProcessMemory(fProcess.__HandleProcess, (PBYTE *) (dwLocalP + offsets.netvars.m_i_health), &iHealth,
                          sizeof(int), 0);
    }
} myPlayer;

void updateJson();

Offsets parseJson(const std::string &filePath) {
    // read a JSON file
    std::ifstream i(filePath);
    json j;
    i >> j;

    OffsettParser::Offsets parsedData = nlohmann::json::parse(j.dump());
    return parsedData;
}

void flash() {
    float newAlphaFlash = .2f; //old val .2f

    if (bflash) {
        if (myPlayer.flash > .5f) {
            WriteProcessMemory(fProcess.__HandleProcess,
                               (PBYTE *) (myPlayer.dwLocalP + offsets.netvars.m_fl_flash_max_alpha), &newAlphaFlash,
                               sizeof(newAlphaFlash), nullptr);
        }
    }
}

void radar() {
    if (bRadar) {
        DWORD entity = 0x0;
        bool bSpot = true;

        for (short i = 0; i < 64; i++) {
            //Radar
            ReadProcessMemory(fProcess.__HandleProcess,
                              (PBYTE *) (fProcess.__dwordClient + offsets.signatures.dw_entity_list + (i * 0x10)),
                              &entity, sizeof(DWORD), nullptr);

            //Spot
            if (entity != NULL) {
                WriteProcessMemory(fProcess.__HandleProcess, (PBYTE *) (entity + offsets.netvars.m_b_spotted), &bSpot,
                                   sizeof(bool), nullptr);
            }
        }
    }
}

void drawChams() {
    DWORD dwCham = 0x70;

    if (bChams) {
        DWORD entity = 0x0;
        bool hasKit = 0;
        int enemyTeam = 0;
        int health = 0;
        //no need for alpha value because brightness does the work for us
        byte rgbColor[3] = {33, 103, 255}; //cyan
        byte rgbColorEnemy[3] = {255, 25, 155}; //pink
        byte rgbColorEnemyLow[3] = {255, 140, 0}; //orange
        byte rgbColorDefuser[3] = {0, 0, 255}; //pure blue
        byte rgbColorEnemyDefuser[3] = {255, 0, 0}; //pure red
        float brightness = 15.f; //65 is very bright
        DWORD thisPtr = (int) (fProcess.__dwordEngine + offsets.signatures.model_ambient_min - 0x2c);
        DWORD xored = *(DWORD *) &brightness ^thisPtr;

        for (int i = 0; i < 64; i++) {
            ReadProcessMemory(fProcess.__HandleProcess,
                              (PBYTE *) (fProcess.__dwordClient + offsets.signatures.dw_entity_list + (i * 0x10)),
                              &entity, sizeof(DWORD), nullptr);
            ReadProcessMemory(fProcess.__HandleProcess, (PBYTE *) (entity + offsets.netvars.m_i_team_num), &enemyTeam,
                              sizeof(int), nullptr);
            ReadProcessMemory(fProcess.__HandleProcess, (PBYTE *) (entity + offsets.netvars.m_b_has_defuser), &hasKit,
                              sizeof(bool), 0);
            ReadProcessMemory(fProcess.__HandleProcess, (PBYTE *) (entity + offsets.netvars.m_i_health), &health,
                              sizeof(bool), 0);

            if (entity != NULL && enemyTeam != myPlayer.iTeam) {

                //Defuse Boi red
                if (hasKit && health > 40) {
                    WriteProcessMemory(fProcess.__HandleProcess, (PBYTE *) (entity + dwCham), &rgbColorEnemyDefuser,
                                       sizeof(rgbColorEnemyDefuser), 0);
                } else if (hasKit && health <= 40) {
                    WriteProcessMemory(fProcess.__HandleProcess, (PBYTE *) (entity + dwCham), &rgbColorEnemyDefuser,
                                       sizeof(rgbColorEnemyDefuser), 0);
                    Sleep(2);
                    WriteProcessMemory(fProcess.__HandleProcess, (PBYTE *) (entity + dwCham), &rgbColorEnemyLow,
                                       sizeof(rgbColorEnemyLow), 0);
                    Sleep(2);
                }
                    //Normal Boi pink
                else {
                    if (health > 40) {
                        WriteProcessMemory(fProcess.__HandleProcess, (PBYTE *) (entity + dwCham), &rgbColorEnemy,
                                           sizeof(rgbColorEnemy), 0);
                    } else {
                        WriteProcessMemory(fProcess.__HandleProcess, (PBYTE *) (entity + dwCham), &rgbColorEnemyLow,
                                           sizeof(rgbColorEnemyLow), 0);
                    }
                }
            } else if (entity != NULL && enemyTeam == myPlayer.iTeam) {
                if (hasKit) {
                    WriteProcessMemory(fProcess.__HandleProcess, (PBYTE *) (entity + dwCham), &rgbColorDefuser,
                                       sizeof(rgbColorDefuser), 0);
                } else {
                    WriteProcessMemory(fProcess.__HandleProcess, (PBYTE *) (entity + dwCham), &rgbColor,
                                       sizeof(rgbColor), nullptr);
                }
            }
        }
        WriteProcessMemory(fProcess.__HandleProcess,
                           (PBYTE *) (fProcess.__dwordEngine + offsets.signatures.model_ambient_min), &xored,
                           sizeof(int), 0);
    } else {
        //reset
        DWORD entity = 0x0;
        int enemyTeam = 0;
        byte rgbColor[3] = {255, 255, 255};
        byte rgbColorEnemy[3] = {255, 255, 255};
        float resetBrightness = 0.f;
        const DWORD thisPtr = (int) (fProcess.__dwordEngine + offsets.signatures.model_ambient_min - 0x2c);
        DWORD resetXored = *(DWORD *) &resetBrightness ^thisPtr;

        for (int i = 0; i < 64; i++) {
            ReadProcessMemory(fProcess.__HandleProcess,
                              (PBYTE *) (fProcess.__dwordClient + offsets.signatures.dw_entity_list + (i * 0x10)),
                              &entity, sizeof(DWORD), nullptr);
            ReadProcessMemory(fProcess.__HandleProcess, (PBYTE *) (entity + offsets.netvars.m_i_team_num), &enemyTeam,
                              sizeof(int), nullptr);

            if (entity != NULL && enemyTeam != myPlayer.iTeam) {
                WriteProcessMemory(fProcess.__HandleProcess, (PBYTE *) (entity + dwCham), &rgbColorEnemy,
                                   sizeof(rgbColorEnemy), 0);
            } else if (entity != NULL && enemyTeam == myPlayer.iTeam) {
                WriteProcessMemory(fProcess.__HandleProcess, (PBYTE *) (entity + dwCham), &rgbColor, sizeof(rgbColor),
                                   0);
            }
        }

        WriteProcessMemory(fProcess.__HandleProcess,
                           (PBYTE *) (fProcess.__dwordEngine + offsets.signatures.model_ambient_min), &resetXored,
                           sizeof(int), nullptr);
    }
}

void trigger() {
    DWORD aimedEntity = 0x0;
    int enemyTeam = 0;

    if (bTrigger) {
        const int AIMED_AT = myPlayer.iCrossID;
        if (AIMED_AT > 0 && AIMED_AT < 64) {
            ReadProcessMemory(fProcess.__HandleProcess,
                              (PBYTE *) (fProcess.__dwordClient + offsets.signatures.dw_entity_list +
                                         (AIMED_AT - 1) * (0x10)), &aimedEntity,
                              sizeof(DWORD), nullptr);
        }

        for (int i = 0; i < 64; i++) {
            ReadProcessMemory(fProcess.__HandleProcess, (PBYTE *) (aimedEntity + offsets.netvars.m_i_team_num),
                              &enemyTeam, sizeof(int), nullptr);
        }

        if (myPlayer.iCrossID > 0 && ((enemyTeam != myPlayer.iTeam) || dangerMode)) {
            Sleep(14);
            mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
            Sleep(10);
            mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
        }
    }
}

void checkDefuse() {
    DWORD entity = 0x0;
    int enemyTeam = 0; //actually EntityTeam
    bool defusing = 0;
    bool hasKit = 0;

    for (int i = 0; i < 64; i++) {
        ReadProcessMemory(fProcess.__HandleProcess,
                          (PBYTE *) (fProcess.__dwordClient + offsets.signatures.dw_entity_list + (i * 0x10)), &entity,
                          sizeof(DWORD), nullptr);
        ReadProcessMemory(fProcess.__HandleProcess, (PBYTE *) (entity + offsets.netvars.m_i_team_num), &enemyTeam,
                          sizeof(int), nullptr);
        ReadProcessMemory(fProcess.__HandleProcess, (PBYTE *) (entity + offsets.netvars.m_b_is_defusing), &defusing,
                          sizeof(bool), nullptr);
        ReadProcessMemory(fProcess.__HandleProcess, (PBYTE *) (entity + offsets.netvars.m_b_has_defuser), &hasKit,
                          sizeof(bool), nullptr);
        if (entity != NULL && enemyTeam != myPlayer.iTeam) {
            if (defusing && hasKit) {
                Beep(550, 70); //lower interval if has defuserkit and higher freq
            }
            if (defusing && !hasKit) {
                Beep(350, 120);
            }
        }
    }
}

void wall() {
    DWORD glowObj = 0x0;
    DWORD entity = 0x0;

    int glowIndex = 0;
    int enemyTeam = 0; //actually EntityTeam

    bool bOccluded = true;
    bool bUnoccluded = false;

    float full = 1.f; //255
    float alpha = .7f;
    //int glowStyle = 2;

    ReadProcessMemory(fProcess.__HandleProcess,
                      (PBYTE *) (fProcess.__dwordClient + offsets.signatures.dw_glow_object_manager), &glowObj,
                      sizeof(DWORD), 0);

    for (int i = 0; i < 20; i++) {
        ReadProcessMemory(fProcess.__HandleProcess,
                          (PBYTE *) (fProcess.__dwordClient + offsets.signatures.dw_entity_list + (i * 0x10)), &entity,
                          sizeof(DWORD), nullptr);
        ReadProcessMemory(fProcess.__HandleProcess, (PBYTE *) (entity + offsets.netvars.m_i_team_num), &enemyTeam,
                          sizeof(int), nullptr);
        ReadProcessMemory(fProcess.__HandleProcess, (PBYTE *) (entity + offsets.netvars.m_i_glow_index), &glowIndex,
                          sizeof(int), nullptr);

        if (entity != NULL && ((enemyTeam != myPlayer.iTeam) || dangerMode)) //Find Enemy
        {
            //Show outlines
            WriteProcessMemory(fProcess.__HandleProcess, (PBYTE *) (glowObj + (glowIndex * 0x38) + 0x4), &full,
                               sizeof(float), 0); //red
            WriteProcessMemory(fProcess.__HandleProcess, (PBYTE *) (glowObj + (glowIndex * 0x38) + 0x10), &alpha,
                               sizeof(float), 0); //alpha
        }

        WriteProcessMemory(fProcess.__HandleProcess, (PBYTE *) (glowObj + (glowIndex * 0x38) + 0x24), &bOccluded,
                           sizeof(bool), 0);
        WriteProcessMemory(fProcess.__HandleProcess, (PBYTE *) (glowObj + (glowIndex * 0x38) + 0x25), &bUnoccluded,
                           sizeof(bool), 0);
    }
}

void updateJson() {
    std::string dwnld_URL = "https://raw.githubusercontent.com/frk1/hazedumper/master/csgo.json";
    std::string savepath = "./hazedumperRepo/csgo.json";
    fs::create_directories("./hazedumperRepo");
    URLDownloadToFile(nullptr, dwnld_URL.c_str(), savepath.c_str(), 0, nullptr);
}

int main() {
    // Get Offsets
    updateJson();

    offsets = parseJson("./hazedumperRepo/csgo.json");

    //always forgetting this line...
    fProcess.RunProcess();

    //if window not found
    if (fProcess.__HWNDCSgo == nullptr) {
        std::cout << "Process 'csgo.exe' not found!" << std::endl;
        std::cout << "Last Error: " << GetLastError() << std::endl;
    } else {
        std::cout << "Made by c1tru5x " << __DATE__ << std::endl;
        std::cout << "-------------------------" << std::endl;
        std::cout << "F11 to close!" << std::endl;
        std::cout << "[NUM1] No Flash" << std::endl;
        std::cout << "[NUM2] Radar" << std::endl;
        std::cout << "[NUM3] Chams" << std::endl;
        std::cout << "[NUM4] Trigger use [ALT]" << std::endl;
        std::cout << "[NUM5] Check for Defuse" << std::endl;
        std::cout << "[NUM6] Walls" << std::endl;
        std::cout << "[NUM9] Panik" << std::endl;
        std::cout << "[NUM7] Dangerzone mode" << std::endl;

        while (!GetAsyncKeyState(VK_F11)) {
            //Read all the time.
            myPlayer.ReadInfo();

            if (GetAsyncKeyState(VK_NUMPAD1)) {
                bflash = !bflash;
                sound::playSound(bflash);
            }
            if (GetAsyncKeyState(VK_NUMPAD2)) {
                bRadar = !bRadar;

                sound::playSound(bRadar);
            }
            if (GetAsyncKeyState(VK_NUMPAD3)) {
                bChams = !bChams;

                sound::playSound(bChams);
            }
            if (GetAsyncKeyState(VK_NUMPAD4)) {
                bTrigger = !bTrigger;

                sound::playSound(bTrigger);
            }
            if (GetAsyncKeyState(VK_NUMPAD5)) {
                bDefuse = !bDefuse;
                sound::playSound(bDefuse);
            }
            if (GetAsyncKeyState(VK_NUMPAD6)) {
                bWall = !bWall;
                sound::playSound(bWall);
            }
            if (GetAsyncKeyState(VK_NUMPAD9)) {
                masterSwitch = !masterSwitch;

                sound::playSound(!masterSwitch);
            }
            if (GetAsyncKeyState(VK_NUMPAD7)) {
                dangerMode = !dangerMode;

                sound::playSound(dangerMode);
            }


            if (masterSwitch) {
                //Functioncall
                if (bTrigger) {
                    if (GetAsyncKeyState(VK_LMENU)) //Alt Key
                    {
                        trigger();
                    }
                }

                if (bflash) {
                    flash();
                }
                if (bRadar) {
                    radar();
                }
                if (bDefuse) {
                    checkDefuse();
                }
                if (bWall) {
                    wall();
                }
            }
            drawChams(); //Needs to be outside so it can reset the color back
            Sleep(1);
        }
    }
    return 0;
}
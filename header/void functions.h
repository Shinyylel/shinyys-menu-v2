#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include "Offsets.h"
#include <stdlib.h>
#include "GetProcess.h"
#include <thread>

HWND hwnd;
uintptr_t moduleBase;

std::string Status;
std::string Status2;
std::string Status3;
std::string Status4;
std::string Status6;
std::string Status5;
int NewFov = 120;


DWORD localPlayerAddy = dwLocalPlayer;
DWORD fovOffset = m_iFOV;
DWORD observerModeOffset = m_iObserverMode;

struct offsets
{
    DWORD localPlayer = dwLocalPlayer;
    DWORD flashDuration = m_flFlashDuration;
} offset;

struct variables
{
    DWORD localPlayer;
    DWORD gameModule;
} val;

uintptr_t Read(uintptr_t addr, DWORD procId)
{
    uintptr_t val;
    Toolhelp32ReadProcessMemory(procId, (LPVOID)addr, &val, sizeof(val), NULL);
    return val;
}

uintptr_t getLocalPlayer()
{ //This will get the address to localplayer. 
    return RPM< uintptr_t>(moduleBase + dwLocalPlayer);
}

uintptr_t getPlayer(int index)
{  //Each player in the game has an index.
    return RPM< uintptr_t>(moduleBase + dwEntityList + index * 0x10); //We use index times 0x10 because the distance between each player 0x10.
}

int getTeam(uintptr_t player)
{
    return RPM<int>(player + m_iTeamNum);
}

int getCrosshairID(uintptr_t player)
{
    return RPM<int>(player + m_iCrosshairId);
}

void clearScreen()
{
    HANDLE hOut;
    COORD Position;

    hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    Position.X = 0;
    Position.Y = 0;
    SetConsoleCursorPosition(hOut, Position);
}

struct glowStructEnemy {
    float red = 1.f;
    float green = 0.f;
    float blue = 0.f;
    float alpha = 1.f;
    uint8_t padding[8];
    float unknown = 1.f;
    uint8_t padding2[4];
    BYTE renderOccluded = true;
    BYTE renderUnoccluded = false;
    BYTE fullBloom = false;
}glowEnm;

struct glowStructLocal {
    float red = 0.f;
    float green = 1.f;
    float blue = 0.f;
    float alpha = 1.f;
    uint8_t padding[8];
    float unknown = 1.f;
    uint8_t padding2[4];
    BYTE renderOccluded = true;
    BYTE renderUnoccluded = false;
    BYTE fullBloom = false;
}glowLocal;

void triggerbot()
{
    if (!GetAsyncKeyState(VK_END)) {
        int CrosshairID = getCrosshairID(getLocalPlayer());
        int CrosshairTeam = getTeam(getPlayer(CrosshairID - 1));
        int LocalTeam = getTeam(getLocalPlayer());
        if (CrosshairID > 0 && CrosshairID < 32 && LocalTeam != CrosshairTeam)
        {
            if (GetAsyncKeyState(VK_MENU))
            {
                mouse_event(MOUSEEVENTF_LEFTDOWN, NULL, NULL, 0, 0);
                mouse_event(MOUSEEVENTF_LEFTUP, NULL, NULL, 0, 0);
                Sleep(10);
            }
        }
    }
}

void shoot()
{
    while (true)
    {
        if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
        {
            mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 10, 0, 0);
            Sleep(60);
            mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
            Sleep(20);

        }
    }
}


void team()
{
    uintptr_t dwGlowManager = RPM<uintptr_t>(moduleBase + dwGlowObjectManager);
    int LocalTeam = RPM<int>(getLocalPlayer() + m_iTeamNum);

    for (int i = 1; i < 32; i++) {
        uintptr_t dwEntity = RPM<uintptr_t>(moduleBase + dwEntityList + i * 0x10);
        int iGlowIndx = RPM<int>(dwEntity + m_iGlowIndex);
        int EnmHealth = RPM<int>(dwEntity + m_iHealth); if (EnmHealth < 1 || EnmHealth > 100) continue;
        int Dormant = RPM<int>(dwEntity + m_bDormant); if (Dormant) continue;
        int EntityTeam = RPM<int>(dwEntity + m_iTeamNum);

        if (LocalTeam == EntityTeam)
        {
            WPM<glowStructLocal>(dwGlowManager + (iGlowIndx * 0x38) + 0x8, glowLocal);
        }
    }
}

void enmteam()
{
    uintptr_t dwGlowManager = RPM<uintptr_t>(moduleBase + dwGlowObjectManager);
    int LocalTeam = RPM<int>(getLocalPlayer() + m_iTeamNum);

    for (int i = 1; i < 32; i++) {
        uintptr_t dwEntity = RPM<uintptr_t>(moduleBase + dwEntityList + i * 0x10);
        int iGlowIndx = RPM<int>(dwEntity + m_iGlowIndex);
        int EnmHealth = RPM<int>(dwEntity + m_iHealth); if (EnmHealth < 1 || EnmHealth > 100) continue;
        int Dormant = RPM<int>(dwEntity + m_bDormant); if (Dormant) continue;
        int EntityTeam = RPM<int>(dwEntity + m_iTeamNum);

        if (LocalTeam != EntityTeam)
        {
            WPM<glowStructEnemy>(dwGlowManager + (iGlowIndx * 0x38) + 0x8, glowEnm);
        }
    }
}


void HandleGlow()
{
    uintptr_t dwGlowManager = RPM<uintptr_t>(moduleBase + dwGlowObjectManager);
    int LocalTeam = RPM<int>(getLocalPlayer() + m_iTeamNum);

    for (int i = 1; i < 32; i++) {
        uintptr_t dwEntity = RPM<uintptr_t>(moduleBase + dwEntityList + i * 0x10);
        int iGlowIndx = RPM<int>(dwEntity + m_iGlowIndex);
        int EnmHealth = RPM<int>(dwEntity + m_iHealth); if (EnmHealth < 1 || EnmHealth > 100) continue;
        int Dormant = RPM<int>(dwEntity + m_bDormant); if (Dormant) continue;
        int EntityTeam = RPM<int>(dwEntity + m_iTeamNum);

        if (LocalTeam == EntityTeam)
        {
            WPM<glowStructLocal>(dwGlowManager + (iGlowIndx * 0x38) + 0x8, glowLocal);
        }
        else if (LocalTeam != EntityTeam)
        {
            WPM<glowStructEnemy>(dwGlowManager + (iGlowIndx * 0x38) + 0x8, glowEnm);
        }
    }
}

void on()
{
    while (true)
    {
        system("Color 4");
        std::cout << "|------------------------------------------------|" << std::endl << std::flush;
        std::cout << "|   WELCOME TO SHINYY'S SIMPLE MENU              |" << std::endl << std::flush;
        std::cout << "|   HOLD [ALT] KEY FOR TRIGGER BOT               |" << std::endl << std::flush;
        std::cout << "|------------------------------------------------|" << std::endl << std::flush;
        std::cout << "|                                                |" << std::endl << std::flush;
        std::cout << "|   [F1] ENEMY TEAM GLOW : " << Status << "" << "                     | " << std::flush << std::endl;
        std::cout << "|   [F2] TEAM GLOW : " << Status6 << "" << "                           | " << std::flush << std::endl;
        std::cout << "|   [F3] BUNNY HOP : " << Status2 << "" << "                           |" << std::flush << std::endl;
        std::cout << "|   [F4] TRIGGER BOT : " << Status3 << "" << "                         |" << std::flush << std::endl;
        std::cout << "|   [F5] ANTI-FLASH : " << Status4 << "" << "                          |" << std::flush << std::endl;
        std::cout << "|                                                |" << std::endl << std::flush;
        std::cout << "|------------------------------------------------|" << std::endl << std::flush;
        Sleep(100);
        clearScreen();
    }
}

void bhop()
{
    uintptr_t LocalPlayer = NULL;
    uintptr_t Flags = NULL;
    if (GetAsyncKeyState(VK_SPACE))
    {
        LocalPlayer = Read((moduleBase + dwLocalPlayer), procId);
        Flags = Read((LocalPlayer + m_fFlags), procId);

        if (Flags == 257 || Flags == 263)
        {
            SendMessageA(hwnd, WM_KEYDOWN, VK_SPACE, 0x390000);
            SendMessageA(hwnd, WM_KEYUP, VK_SPACE, 0x390000);
        }
    }
}

void flash()
{
    int flashDur = 0;

    flashDur = RPM<int>(val.localPlayer + offset.flashDuration);
    if (flashDur > 0)
    {
        WPM<int>(val.localPlayer + offset.flashDuration, 0);
    }

}
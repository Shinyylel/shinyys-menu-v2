#include "void functions.h"

int main() {
    int flashDur = 0;

    hwnd = FindWindowA(NULL, "Counter-Strike: Global Offensive");
    GetWindowThreadProcessId(hwnd, &procId);
    moduleBase = GetModuleBaseAddress("client.dll");
    hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);

    if (val.localPlayer == NULL)
        while (val.localPlayer == NULL)
            val.localPlayer = RPM<DWORD>(moduleBase + offset.localPlayer);

    if (hwnd != 0)
    {
        system("Color 4");
        std::cout << "PROCESS FOUND" << std::endl;
        Sleep(1000);
        std::cout << "INJECTING...." << std::endl;
        Sleep(2000);
        system("CLS");

    }
    else
    {
        if (hwnd == INVALID_HANDLE_VALUE || hwnd == NULL)
        {
            system("Color 4");
            std::cout << "PROCESS NOT FOUND" << std::endl;
            Sleep(500);
            std::cout << "TERMINATING PROCESS..." << std::endl;
            Sleep(2000);
            exit(1);
        }
    }     

            Status = "0";
            Status2 = "0";
            Status3 = "0";
            Status4 = "0";
            Status6 = "0";

            std::thread t1(on);

            while (true)
            {
                if (GetKeyState(VK_F1) & 1)
                {
                    enmteam();
                    Status = "1";
                    Sleep(1);
                }
                else
                {
                    Status = "0";
                }
                if (GetKeyState(VK_F2) & 1)
                {
                    team();
                    Status6 = "1";
                    Sleep(1);
                }
                else
                {
                    Status6 = "0";
                }
                if (GetKeyState(VK_F3) & 1)
                {
                    bhop();
                    Status2 = "1";
                    Sleep(1);
                }
                else
                {
                    Status2 = "0";
                }
                if (GetKeyState(VK_F4) & 1)
                {
                    triggerbot();
                    Status3 = "1";
                    Sleep(1);
                }
                else
                {
                    Status3 = "0";
                }
                if (GetKeyState(VK_F5) & 1)
                {
                    flash();
                    Status4 = "1";
                    Sleep(1);
                }
                else
                {
                    Status4 = "0";
                }
                if (GetKeyState(VK_F6) & 1)
                {
                    shoot();
                    Status5 = "1";
                    Sleep(1);
                }
                else
                {
                    Status5 = "0";
                }
            }   
}

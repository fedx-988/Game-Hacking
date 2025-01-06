#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
#include <tchar.h> // _tcscmp
#include <vector>
#include <thread>
#include "offsets.h"

DWORD GetModuleBaseAddress(TCHAR* lpszModuleName, DWORD pID) {
    DWORD dwModuleBaseAddress = 0;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pID);
    MODULEENTRY32 ModuleEntry32 = { 0 };
    ModuleEntry32.dwSize = sizeof(MODULEENTRY32);

    if (Module32First(hSnapshot, &ModuleEntry32)) {
        do {
            if (_tcscmp(ModuleEntry32.szModule, lpszModuleName) == 0) {
                dwModuleBaseAddress = (DWORD)ModuleEntry32.modBaseAddr;
                break;
            }
        } while (Module32Next(hSnapshot, &ModuleEntry32));
    }
    CloseHandle(hSnapshot);
    return dwModuleBaseAddress;
}

void ClearConsole() {
    system("cls"); // Clear the console screen
}

void DisplayMenu(bool healthRegenerationActive, const std::string& regenStatus,
    int currentHealth, int currentArmour,
    int currentPistolAmmo, int currentARAmmo,
    int currentGrenades, int currentDualPistolAmmo,
    int currentPlayerCount) {
    ClearConsole();
    std::cout << "Assault Cube - fedx" << std::endl;
    std::cout << "----------------------------------------------" << std::endl;
    std::cout << "Current Health: " << currentHealth << std::endl;
    std::cout << "Current Armour: " << currentArmour << std::endl;
    std::cout << "Current Pistol Ammo: " << currentPistolAmmo << std::endl;
    std::cout << "Current AR Ammo: " << currentARAmmo << std::endl;
    std::cout << "Current Dual Pistols Ammo: " << currentDualPistolAmmo << std::endl;
    std::cout << "Current Amount Of Grenades: " << currentGrenades << std::endl;
    std::cout << "Current Amount Of Players: " << currentPlayerCount << std::endl;
    std::cout << "Press the DELETE key to EXIT" << std::endl;
    std::cout << "Press the Up Arrow key to set Pistol Ammo" << std::endl;
    std::cout << "Press the Down Arrow key to set Health" << std::endl;
    std::cout << "Press the Left Arrow key to set AR Ammo" << std::endl;
    std::cout << "Press the Right Arrow key to set Grenade Ammo" << std::endl;
    std::cout << "Press F1 to set Dual Pistols Ammo" << std::endl;
    std::cout << "Press F2 to set Armour" << std::endl;

    if (healthRegenerationActive) {
        std::cout << regenStatus << std::endl;
    }
    else {
        std::cout << "Press F3 to regenerate your health" << std::endl;
    }
}

int main() {
    HWND hGameWindow = FindWindow(NULL, L"AssaultCube");
    if (hGameWindow == NULL) {
        std::cout << "Start the game!" << std::endl;
        return 0;
    }

    DWORD pID = NULL; // ID of the game
    GetWindowThreadProcessId(hGameWindow, &pID);

    HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pID);
    if (processHandle == INVALID_HANDLE_VALUE || processHandle == NULL) {
        std::cout << "Failed to open process" << std::endl;
        return 0;
    }

    TCHAR gameName[] = _T("ac_client.exe");
    DWORD gameBaseAddress = GetModuleBaseAddress(gameName, pID);

    DWORD baseAddress = NULL;
    ReadProcessMemory(processHandle, (LPVOID)(gameBaseAddress + BASE_OFFSET), &baseAddress, sizeof(baseAddress), NULL);

    DWORD pointsAddress = baseAddress + PISTOL_AMMO_OFFSET;
    DWORD healthAddress = baseAddress + HEALTH_OFFSET;
    DWORD arAmmoAddress = baseAddress + AR_AMMO_OFFSET;
    DWORD grenadeAmmoAddress = baseAddress + GRENADE_AMMO_OFFSET;
    DWORD dualPistolAmmoAddress = baseAddress + DUAL_PISTOL_AMMO_OFFSET;
    DWORD armourAddress = baseAddress + ARMOUR_OFFSET;
    DWORD playerCountAddress = gameBaseAddress + PLAYER_COUNT_OFFSET;

    bool healthRegenerationActive = false;

    while (true) {
        Sleep(50);
        if (GetAsyncKeyState(VK_DELETE)) {
            return 0;
        }

        int currentHealth = 0;
        ReadProcessMemory(processHandle, (LPVOID)(healthAddress), &currentHealth, sizeof(currentHealth), NULL);

        int currentArmour = 0;
        ReadProcessMemory(processHandle, (LPVOID)(armourAddress), &currentArmour, sizeof(currentArmour), NULL);

        int currentPistolAmmo = 0;
        ReadProcessMemory(processHandle, (LPVOID)(pointsAddress), &currentPistolAmmo, sizeof(currentPistolAmmo), NULL);

        int currentARAmmo = 0;
        ReadProcessMemory(processHandle, (LPVOID)(arAmmoAddress), &currentARAmmo, sizeof(currentARAmmo), NULL);

        int currentGrenades = 0;
        ReadProcessMemory(processHandle, (LPVOID)(grenadeAmmoAddress), &currentGrenades, sizeof(currentGrenades), NULL);

        int currentDualPistolAmmo = 0;
        ReadProcessMemory(processHandle, (LPVOID)(dualPistolAmmoAddress), &currentDualPistolAmmo, sizeof(currentDualPistolAmmo), NULL);

        int currentPlayerCount = 0;
        ReadProcessMemory(processHandle, (LPVOID)(playerCountAddress), &currentPlayerCount, sizeof(currentPlayerCount), NULL);

        DisplayMenu(healthRegenerationActive, "",
            currentHealth, currentArmour,
            currentPistolAmmo, currentARAmmo,
            currentGrenades, currentDualPistolAmmo,
            currentPlayerCount);

        if (GetAsyncKeyState(VK_UP)) {
            std::cout << "How much Pistol Ammo do you want?" << std::endl;
            int newPistolAmmo = 0;
            std::cin >> newPistolAmmo;
            WriteProcessMemory(processHandle, (LPVOID)(pointsAddress), &newPistolAmmo, sizeof(newPistolAmmo), NULL);
            Sleep(2000);
        }

        if (GetAsyncKeyState(VK_DOWN)) {
            std::cout << "How much Health do you want?" << std::endl;
            int newHealth = 0;
            std::cin >> newHealth;
            WriteProcessMemory(processHandle, (LPVOID)(healthAddress), &newHealth, sizeof(newHealth), NULL);
            Sleep(2000);
        }

        if (GetAsyncKeyState(VK_LEFT)) {
            std::cout << "How much AR ammo do you want?" << std::endl;
            int newARAmmo = 0;
            std::cin >> newARAmmo;
            WriteProcessMemory(processHandle, (LPVOID)(arAmmoAddress), &newARAmmo, sizeof(newARAmmo), NULL);
            Sleep(2000);
        }

        if (GetAsyncKeyState(VK_RIGHT)) {
            std::cout << "How many Grenades do you want?" << std::endl;
            int newGrenadeCount = 0;
            std::cin >> newGrenadeCount;
            WriteProcessMemory(processHandle, (LPVOID)(grenadeAmmoAddress), &newGrenadeCount, sizeof(newGrenadeCount), NULL);
            Sleep(2000);
        }

        if (GetAsyncKeyState(VK_F1)) {
            std::cout << "How much Dual Pistols ammo do you want?" << std::endl;
            int newDualPistolAmmo = 0;
            std::cin >> newDualPistolAmmo;
            WriteProcessMemory(processHandle, (LPVOID)(dualPistolAmmoAddress), &newDualPistolAmmo, sizeof(newDualPistolAmmo), NULL);
            Sleep(2000);
        }

        if (GetAsyncKeyState(VK_F2)) {
            std::cout << "How much Armour do you want?" << std::endl;
            int newArmour = 0;
            std::cin >> newArmour;
            WriteProcessMemory(processHandle, (LPVOID)(armourAddress), &newArmour, sizeof(newArmour), NULL);
            Sleep(2000);
        }

        if (GetAsyncKeyState(VK_F3)) {
            healthRegenerationActive = !healthRegenerationActive;

            if (healthRegenerationActive) {
                auto regenThread = std::thread([=]() mutable {
                    while (healthRegenerationActive) {
                        int currentHealthThread = 0;
                        ReadProcessMemory(processHandle, (LPVOID)(healthAddress), &currentHealthThread, sizeof(currentHealthThread), NULL);

                        if (currentHealthThread < 100) {
                            currentHealthThread += 10;
                            WriteProcessMemory(processHandle, (LPVOID)(healthAddress), &currentHealthThread, sizeof(currentHealthThread), NULL);
                        }
                        else {
                            healthRegenerationActive = false;
                        }

                        Sleep(1000);
                    }
                    });
                regenThread.detach();
            }
        }
    }
}

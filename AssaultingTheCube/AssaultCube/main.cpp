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

    if (Module32First(hSnapshot, &ModuleEntry32))
    {
        do {
            if (_tcscmp(ModuleEntry32.szModule, lpszModuleName) == 0)
            {
                dwModuleBaseAddress = (DWORD)ModuleEntry32.modBaseAddr;
                break;
            }
        } while (Module32Next(hSnapshot, &ModuleEntry32)); 
    }
    CloseHandle(hSnapshot);
    return dwModuleBaseAddress;
}

// Function to clear the console
void ClearConsole() {
    system("cls"); // Clear the console screen
}

// Function to display the menu
void DisplayMenu(bool healthRegenerationActive, const std::string& regenStatus) {
    ClearConsole();
    std::cout << "Assault Cube - fedx" << std::endl;
    std::cout << "----------------------------------------------" << std::endl;
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

void HealthRegeneration(HANDLE processHandle, DWORD healthAddress, bool* active) {
    while (*active) {
        int currentHealth = 0;
        ReadProcessMemory(processHandle, (LPVOID)(healthAddress), &currentHealth, sizeof(currentHealth), NULL);

        if (currentHealth < 100) {
            currentHealth += 10;
            WriteProcessMemory(processHandle, (LPVOID)(healthAddress), &currentHealth, sizeof(currentHealth), NULL);
            Sleep(1000);
        }
        else {
            *active = false;
        }
    }
}

int main() {
    HWND hGameWindow = FindWindow(NULL, L"AssaultCube");
    if (hGameWindow == NULL) {
        std::cout << "Start the game!" << std::endl;
        return 0;
    }

    DWORD pID = NULL; // ID OF OUR GAME
    GetWindowThreadProcessId(hGameWindow, &pID);

    HANDLE processHandle = NULL;
    processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pID);
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

    DisplayMenu(false, "");

    bool healthRegenerationActive = false;

    while (true) {
        Sleep(50);
        if (GetAsyncKeyState(VK_DELETE)) {
            return 0;
        }

        if (GetAsyncKeyState(VK_UP)) {
            std::cout << "How much Pistol Ammo do you want?" << std::endl;
            int newPistolAmmo = 0;
            std::cin >> newPistolAmmo;
            WriteProcessMemory(processHandle, (LPVOID)(pointsAddress), &newPistolAmmo, sizeof(newPistolAmmo), NULL);
            Sleep(2000);
            DisplayMenu(healthRegenerationActive, "");
        }

        if (GetAsyncKeyState(VK_DOWN)) {
            std::cout << "How much Health do you want?" << std::endl;
            int newHealth = 0;
            std::cin >> newHealth;
            WriteProcessMemory(processHandle, (LPVOID)(healthAddress), &newHealth, sizeof(newHealth), NULL);
            Sleep(2000);
            DisplayMenu(healthRegenerationActive, "");
        }

        if (GetAsyncKeyState(VK_LEFT)) {
            std::cout << "How much AR ammo do you want?" << std::endl;
            int newARAmmo = 0;
            std::cin >> newARAmmo;
            WriteProcessMemory(processHandle, (LPVOID)(arAmmoAddress), &newARAmmo, sizeof(newARAmmo), NULL);
            Sleep(2000);
            DisplayMenu(healthRegenerationActive, "");
        }

        if (GetAsyncKeyState(VK_RIGHT)) {
            std::cout << "How many Grenades do you want?" << std::endl;
            int newGrenadeAmmo = 0;
            std::cin >> newGrenadeAmmo;
            WriteProcessMemory(processHandle, (LPVOID)(grenadeAmmoAddress), &newGrenadeAmmo, sizeof(newGrenadeAmmo), NULL);
            Sleep(2000);
            DisplayMenu(healthRegenerationActive, "");
        }

        if (GetAsyncKeyState(VK_F1)) {
            std::cout << "How much Dual Pistol ammo do you want?" << std::endl;
            int newDualPistolAmmo = 0;
            std::cin >> newDualPistolAmmo;
            WriteProcessMemory(processHandle, (LPVOID)(dualPistolAmmoAddress), &newDualPistolAmmo, sizeof(newDualPistolAmmo), NULL);
            Sleep(2000);
            DisplayMenu(healthRegenerationActive, "");
        }

        if (GetAsyncKeyState(VK_F2)) {
            std::cout << "How much Armour do you want?" << std::endl;
            int newArmour = 0;
            std::cin >> newArmour;
            WriteProcessMemory(processHandle, (LPVOID)(armourAddress), &newArmour, sizeof(newArmour), NULL);
            Sleep(2000);
            DisplayMenu(healthRegenerationActive, "");
        }

        if (GetAsyncKeyState(VK_F3)) {

            healthRegenerationActive = !healthRegenerationActive;

            if (healthRegenerationActive) {
                DisplayMenu(healthRegenerationActive, "Regenerating...");

                auto regenThread = std::thread([=]() mutable {
                    while (healthRegenerationActive) {
                        int currentHealth = 0;
                        ReadProcessMemory(processHandle, (LPVOID)(healthAddress), &currentHealth, sizeof(currentHealth), NULL);
                        if (currentHealth < 100) {
                            currentHealth += 10;
                            WriteProcessMemory(processHandle, (LPVOID)(healthAddress), &currentHealth, sizeof(currentHealth), NULL);
                        }
                        else {
                            healthRegenerationActive = false;
                            DisplayMenu(false, "Regenerated!");
                        }
                        Sleep(1000);
                    }
                    });
                regenThread.detach();
            }
            else {
                DisplayMenu(false, "");
            }
        }
    }
}
#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
#include <tchar.h> // _tcscmp
#include <vector>
#include <thread> // For std::thread
#include "offsets.h" // Include the offsets header

DWORD GetModuleBaseAddress(TCHAR* lpszModuleName, DWORD pID) {
    DWORD dwModuleBaseAddress = 0;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pID); // make snapshot of all modules within process
    MODULEENTRY32 ModuleEntry32 = { 0 };
    ModuleEntry32.dwSize = sizeof(MODULEENTRY32);

    if (Module32First(hSnapshot, &ModuleEntry32)) // store first Module in ModuleEntry32
    {
        do {
            if (_tcscmp(ModuleEntry32.szModule, lpszModuleName) == 0) // if Found Module matches Module we look for -> done!
            {
                dwModuleBaseAddress = (DWORD)ModuleEntry32.modBaseAddr;
                break;
            }
        } while (Module32Next(hSnapshot, &ModuleEntry32)); // go through Module entries in Snapshot and store in ModuleEntry32
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
    ClearConsole(); // Clear the console before displaying the menu
    std::cout << "Assault Cube - fedx" << std::endl;
    std::cout << "----------------------------------------------" << std::endl;
    std::cout << "Press the DELETE key to EXIT" << std::endl;
    std::cout << "Press the Up Arrow key to set Pistol Ammo" << std::endl;
    std::cout << "Press the Down Arrow key to set Health" << std::endl;
    std::cout << "Press the Left Arrow key to set AR Ammo" << std::endl;
    std::cout << "Press the Right Arrow key to set Grenade Ammo" << std::endl;
    std::cout << "Press F1 to set Dual Pistols Ammo" << std::endl;
    std::cout << "Press F2 to set Armour" << std::endl;

    // Display health regeneration status
    if (healthRegenerationActive) {
        std::cout << regenStatus << std::endl; // Show regenerating status or regenerated message
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
            Sleep(1000); // Wait for 1 second before adding more health
        }
        else {
            *active = false; // Stop regeneration if health is at max
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

    TCHAR gameName[] = _T("ac_client.exe"); // Use TCHAR for compatibility
    DWORD gameBaseAddress = GetModuleBaseAddress(gameName, pID);

    // Calculate base address using offsets from offsets.h
    DWORD baseAddress = NULL;

    ReadProcessMemory(processHandle, (LPVOID)(gameBaseAddress + BASE_OFFSET), &baseAddress, sizeof(baseAddress), NULL);

    DWORD pointsAddress = baseAddress + PISTOL_AMMO_OFFSET;      // The address we need for pistol ammo
    DWORD healthAddress = baseAddress + HEALTH_OFFSET;            // The address we need for health
    DWORD arAmmoAddress = baseAddress + AR_AMMO_OFFSET;          // Calculate AR ammo address
    DWORD grenadeAmmoAddress = baseAddress + GRENADE_AMMO_OFFSET; // Calculate grenade count address
    DWORD dualPistolAmmoAddress = baseAddress + DUAL_PISTOL_AMMO_OFFSET; // Calculate dual pistols ammo address
    DWORD armourAddress = baseAddress + ARMOUR_OFFSET;           // Calculate armour address

    DisplayMenu(false, ""); // Show initial menu

    bool healthRegenerationActive = false; // Track health regeneration status

    while (true) {
        Sleep(50);
        if (GetAsyncKeyState(VK_DELETE)) { // Exit
            return 0;
        }

        if (GetAsyncKeyState(VK_UP)) { // Modify Pistol Ammo
            std::cout << "How much Pistol Ammo do you want?" << std::endl;
            int newPistolAmmo = 0;
            std::cin >> newPistolAmmo;
            WriteProcessMemory(processHandle, (LPVOID)(pointsAddress), &newPistolAmmo, sizeof(newPistolAmmo), NULL);
            Sleep(2000); // Wait for 2 seconds
            DisplayMenu(healthRegenerationActive, ""); // Clear and show menu again
        }

        if (GetAsyncKeyState(VK_DOWN)) { // Modify Health
            std::cout << "How much Health do you want?" << std::endl;
            int newHealth = 0;
            std::cin >> newHealth;
            WriteProcessMemory(processHandle, (LPVOID)(healthAddress), &newHealth, sizeof(newHealth), NULL);
            Sleep(2000); // Wait for 2 seconds
            DisplayMenu(healthRegenerationActive, ""); // Clear and show menu again
        }

        if (GetAsyncKeyState(VK_LEFT)) { // Modify AR Ammo
            std::cout << "How much AR ammo do you want?" << std::endl;
            int newARAmmo = 0;
            std::cin >> newARAmmo;
            WriteProcessMemory(processHandle, (LPVOID)(arAmmoAddress), &newARAmmo, sizeof(newARAmmo), NULL);
            Sleep(2000); // Wait for 2 seconds
            DisplayMenu(healthRegenerationActive, ""); // Clear and show menu again
        }

        if (GetAsyncKeyState(VK_RIGHT)) { // Modify Grenade Ammo
            std::cout << "How many Grenades do you want?" << std::endl;
            int newGrenadeAmmo = 0;
            std::cin >> newGrenadeAmmo;
            WriteProcessMemory(processHandle, (LPVOID)(grenadeAmmoAddress), &newGrenadeAmmo, sizeof(newGrenadeAmmo), NULL);
            Sleep(2000); // Wait for 2 seconds
            DisplayMenu(healthRegenerationActive, ""); // Clear and show menu again
        }

        if (GetAsyncKeyState(VK_F1)) { // Modify Dual Pistols Ammo
            std::cout << "How much Dual Pistol ammo do you want?" << std::endl;
            int newDualPistolAmmo = 0;
            std::cin >> newDualPistolAmmo;
            WriteProcessMemory(processHandle, (LPVOID)(dualPistolAmmoAddress), &newDualPistolAmmo, sizeof(newDualPistolAmmo), NULL);
            Sleep(2000); // Wait for 2 seconds
            DisplayMenu(healthRegenerationActive, ""); // Clear and show menu again
        }

        if (GetAsyncKeyState(VK_F2)) { // Modify Armour
            std::cout << "How much Armour do you want?" << std::endl;
            int newArmour = 0;
            std::cin >> newArmour;
            WriteProcessMemory(processHandle, (LPVOID)(armourAddress), &newArmour, sizeof(newArmour), NULL);
            Sleep(2000); // Wait for 2 seconds
            DisplayMenu(healthRegenerationActive, ""); // Clear and show menu again
        }

        if (GetAsyncKeyState(VK_F3)) { // Toggle Health Regeneration on/off

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
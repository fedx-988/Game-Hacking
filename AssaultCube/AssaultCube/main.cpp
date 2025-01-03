#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
#include <tchar.h> // _tcscmp
#include <vector>
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

    DWORD pointsAddress = baseAddress + PISTOL_AMMO_OFFSET; // The address we need for ammo
    DWORD healthAddress = baseAddress + HEALTH_OFFSET; // The address we need for health
    DWORD arAmmoAddress = baseAddress + AR_AMMO_OFFSET; // Calculate AR ammo address

    //"UI"
    std::cout << "Assault Cube - fedx" << std::endl;
    std::cout << "Press the DELETE key to EXIT" << std::endl;
    std::cout << "Press the Up Arrow key to set Ammo" << std::endl;
    std::cout << "Press the Down Arrow key to set Health" << std::endl;
    std::cout << "Press the Left Arrow key to set AR Ammo" << std::endl;

    while (true) {
        Sleep(50);
        if (GetAsyncKeyState(VK_DELETE)) { // Exit
            return 0;
        }

        if (GetAsyncKeyState(VK_UP)) { // Modify Ammo
            std::cout << "How much ammo do you want?" << std::endl;
            int newPistolAmmo = 0;
            std::cin >> newPistolAmmo;
            WriteProcessMemory(processHandle, (LPVOID)(pointsAddress), &newPistolAmmo, sizeof(newPistolAmmo), NULL);
        }

        if (GetAsyncKeyState(VK_DOWN)) { // Modify Health
            std::cout << "How much health do you want?" << std::endl;
            int newHealth = 0;
            std::cin >> newHealth;
            WriteProcessMemory(processHandle, (LPVOID)(healthAddress), &newHealth, sizeof(newHealth), NULL);
        }

        if (GetAsyncKeyState(VK_LEFT)) { // Modify AR Ammo
            std::cout << "How much AR ammo do you want?" << std::endl;
            int newARAmmo = 0;
            std::cin >> newARAmmo;
            WriteProcessMemory(processHandle, (LPVOID)(arAmmoAddress), &newARAmmo, sizeof(newARAmmo), NULL);
        }
    }
}

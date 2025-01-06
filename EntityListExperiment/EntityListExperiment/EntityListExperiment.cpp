#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <thread> // For std::this_thread::sleep_for
#include <chrono> // For std::chrono::milliseconds

DWORD GetProcId(const wchar_t* procName) {
    PROCESSENTRY32 procEntry;
    procEntry.dwSize = sizeof(PROCESSENTRY32);
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (snapshot != INVALID_HANDLE_VALUE) {
        while (Process32Next(snapshot, &procEntry)) {
            if (wcscmp(procEntry.szExeFile, procName) == 0) {
                CloseHandle(snapshot);
                return procEntry.th32ProcessID;
            }
        }
    }
    CloseHandle(snapshot);
    return 0;
}

uintptr_t GetModuleBaseAddress(DWORD procId, const wchar_t* moduleName) {
    uintptr_t baseAddress = 0;
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);

    if (snapshot != INVALID_HANDLE_VALUE) {
        MODULEENTRY32 moduleEntry;
        moduleEntry.dwSize = sizeof(MODULEENTRY32);

        while (Module32Next(snapshot, &moduleEntry)) {
            if (wcscmp(moduleEntry.szModule, moduleName) == 0) {
                baseAddress = (uintptr_t)moduleEntry.modBaseAddr;
                break;
            }
        }
    }
    CloseHandle(snapshot);
    return baseAddress;
}

int main() {
    const wchar_t* processName = L"ac_client.exe";

    while (true) { // Infinite loop to keep updating
        DWORD procId = GetProcId(processName);

        if (procId) {
            uintptr_t moduleBase = GetModuleBaseAddress(procId, processName);
            uintptr_t playerCountAddress = moduleBase + 0x18AC0C;

            HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procId);
            int playerCount;

            if (ReadProcessMemory(hProcess, (BYTE*)playerCountAddress, &playerCount, sizeof(playerCount), nullptr)) {
                system("cls"); // Clear the console
                std::wcout << L"Player Count: " << playerCount << std::endl; // Print updated player count
            }
            else {
                std::cerr << "Failed to read memory." << std::endl;
            }

            CloseHandle(hProcess);
        }
        else {
            std::cerr << "Process not found." << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Wait for 100 milliseconds before the next update
    }

    return 0;
}

#pragma once
#include <windows.h>
#include <TlHelp32.h>
#include <iostream>
#include <vector>
#include <thread>
class processes {
public:
    //processid povna radganac shemdeg modbaseaddr da hprocess vipovo amkleobit
    uintptr_t GetProcessID(const wchar_t* name) {
        HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (snapshot == INVALID_HANDLE_VALUE) {
            std::cerr << "Failed to create process snapshot." << std::endl;
            return 0;
        }

        PROCESSENTRY32 entry = {};
        entry.dwSize = sizeof(PROCESSENTRY32W);

        if (Process32FirstW(snapshot, &entry)) {
            do {
                if (_wcsicmp(entry.szExeFile, name) == 0) {
                    CloseHandle(snapshot);
                    return entry.th32ProcessID;
                }
            } while (Process32NextW(snapshot, &entry));
        }

        CloseHandle(snapshot);
        std::cerr << "Process not found: " << name << std::endl;
        return 0;
    }
    //baseaddr povna radganac sachiroa radganac inside gakvt tu amas kitxulobt
    uintptr_t GetModuleBaseAddress(uintptr_t processid, const wchar_t* name) {
        HANDLE snapshotHandle = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, processid);
        if (snapshotHandle == INVALID_HANDLE_VALUE) {
            std::cerr << "Failed to create module snapshot." << std::endl;
            return 0;
        }

        MODULEENTRY32W entry = {};
        entry.dwSize = sizeof(MODULEENTRY32W);

        if (Module32FirstW(snapshotHandle, &entry)) {
            do {
                if (_wcsicmp(entry.szModule, name) == 0) {
                    CloseHandle(snapshotHandle);
                    return reinterpret_cast<uintptr_t>(entry.modBaseAddr);
                }
            } while (Module32NextW(snapshotHandle, &entry));
        }

        CloseHandle(snapshotHandle);
        std::cerr << "Module not found: " << name << std::endl;
        return 0;
    }
    //hprocess is povna
    HANDLE process() {
        uintptr_t processid = GetProcessID(L"cs2.exe");
        if (!processid) {
            std::cerr << "Failed to find process ID." << std::endl;
            return nullptr;
        }

        HANDLE hprocess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processid);
        if (!hprocess) {
            std::cerr << "Failed to open process." << std::endl;
        }
        return hprocess;
    }

    //basic readmemory gamartivebuli formit ro gamovikeno radganac sigmavar
    template <typename T>
    T ReadMemory(HANDLE process, uintptr_t address) {
        T value;
        SIZE_T bytesRead;
        if (!ReadProcessMemory(process, (LPCVOID)address, &value, sizeof(T), &bytesRead)) {
            std::cerr << "Failed to read memory at address: " << std::hex << address << std::dec << std::endl;
            return T(); 
        }
        return value;
    }

};

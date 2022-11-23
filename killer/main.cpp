#include <iostream>
#include <windows.h>
#include <process.h>
#include <Tlhelp32.h>
#include <winbase.h>
#include <cstring>
#include <string>
#include <vector>
#include <string>
#include <sstream>

const std::string ENV = "PROC_TO_KILL";
const std::string PROC_TO_KILL = "Telegram.exe, chrome";

bool kill_process_by_id(int id){
    HANDLE descriptor = OpenProcess(PROCESS_TERMINATE, FALSE, id);
    bool isKilled = false;
    if (descriptor != nullptr) {
        TerminateProcess(descriptor, 0);
        CloseHandle(descriptor);
        isKilled = true;
    }

    return isKilled;
}

bool kill_process_by_name(const char* name) {
    HANDLE snapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);
    PROCESSENTRY32 procEntry;
    procEntry.dwSize = sizeof(procEntry);
    BOOL isProc = Process32First(snapShot, &procEntry);
    bool isKilled = false;
    while (isProc) {
        if (strcmp(procEntry.szExeFile, name) == 0) {
            HANDLE procDescrptr = OpenProcess(PROCESS_TERMINATE, 0, (DWORD) procEntry.th32ProcessID);
            if (procDescrptr != nullptr) {
                TerminateProcess(procDescrptr, 9);
                CloseHandle(procDescrptr);
                isKilled = true;
            }
        }
        isProc = Process32Next(snapShot, &procEntry);
    }
    CloseHandle(snapShot);

    return isKilled;
}

void killEnvVar() {
    const DWORD max_size = 256;
    char buff[max_size];
    if(!GetEnvironmentVariable(ENV.c_str(), buff, max_size)) {
        return;
    }

    std::stringstream ss(buff);
    while (ss.getline(buff, max_size, ',')) {
        kill_process_by_name(buff);
    }
}

void kill_by_name_result(const char* name) {
    if(kill_process_by_name(name)) {
        std::cout << name << " was killed.\n";
    } else {
        std::cout << name << " wasn't killed!\n";
    }
}

void kill_by_id_result(int id) {
    if(kill_process_by_id(id)) {
        std::cout << "Process (id " << id << ") was killed.\n";
    } else {
        std::cout << "Process (id " << id << ") wasn't killed.\n";
    }
}

int main(int argc, char *argv[])
{
    SetEnvironmentVariableA(ENV.c_str(), PROC_TO_KILL.c_str());
    killEnvVar();

    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "--name") == 0) {
            kill_by_name_result(argv[i+1]);
        } else if (strcmp(argv[i], "--id") == 0) {
            kill_by_id_result(std::stoi(argv[i+1]));
        }
    }
    return 0;
}
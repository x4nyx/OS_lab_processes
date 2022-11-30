#include <windows.h>
#include <vector>
#include <string>


int main() {
    STARTUPINFO startupInf = {sizeof(startupInf)};
    SECURITY_ATTRIBUTES secAtt, secThr;
    PROCESS_INFORMATION procInf;

    secAtt.nLength = sizeof(secAtt);
    secAtt.lpSecurityDescriptor = nullptr;
    secAtt.bInheritHandle = FALSE;

    secThr.nLength = sizeof(secThr);
    secThr.lpSecurityDescriptor = nullptr;
    secThr.bInheritHandle = FALSE;

    DWORD dwPID = 3264;
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS,TRUE,dwPID);
    char szHANDLE[100];
    sprintf(szHANDLE,"%d",hProcess);
    SetEnvironmentVariable("PROC_TO_KILL", szHANDLE);

    TCHAR szInput[1000];
    std::string param = "--name Telegram.exe";
    lstrcpyA(szInput, param.c_str());

    if(CreateProcess(R"("..\\killer\\cmake-build-debug\\killer.exe")",
                     szInput, &secAtt, &secThr,
                     FALSE, 0, nullptr, nullptr, &startupInf, &procInf)){
        WaitForSingleObject(procInf.hProcess, INFINITE);
        CloseHandle(procInf.hThread);
        CloseHandle(procInf.hProcess);
    }
    SetEnvironmentVariable("PROC_TO_KILL", nullptr);
    return 0;
}
#include <stdio.h>
#include <windows.h>

int main(int argc, char* argv[]) {
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    char cmdLine[50];
    sprintf(cmdLine, "C:\\Desktop\\fib.exe %s", argv[1]);
    if ( !CreateProcess(NULL,
        cmdLine,
        NULL,
        NULL,
        FALSE,
        0,
        NULL,
        NULL,
        &si,
        &pi) ) {
        printf("Create Process Failed\n");
        return -1;
    }

    WaitForSingleObject(pi.hProcess, INFINITE);

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return 0;
}

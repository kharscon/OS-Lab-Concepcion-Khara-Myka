#include <windows.h>
#include <stdio.h>

int main() {
    printf("Parent PID: %lu\n", GetCurrentProcessId());

    STARTUPINFO si = { sizeof(si) };
    PROCESS_INFORMATION pi;
    char cmdLine[128];

    // Pass parent PID as argument
    sprintf(cmdLine, "child_echo.exe %lu", GetCurrentProcessId());

    if (!CreateProcess(NULL, cmdLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        printf("CreateProcess failed (%lu).\n", GetLastError());
        return 1;
    }

    // Wait for child
    WaitForSingleObject(pi.hProcess, INFINITE);

    DWORD exitCode;
    GetExitCodeProcess(pi.hProcess, &exitCode);

    printf("Child exited with code: %lu\n", exitCode);

    // Cleanup
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return 0;
}
#include <windows.h>
#include <stdio.h>

int main() {
    printf("Parent PID: %lu\n", GetCurrentProcessId());

    STARTUPINFO si1 = { sizeof(si1) }, si2 = { sizeof(si2) };
    PROCESS_INFORMATION pi1, pi2;
    char cmdLine1[128], cmdLine2[128];

    sprintf(cmdLine1, "child_echo.exe %lu", GetCurrentProcessId());
    sprintf(cmdLine2, "child_echo.exe %lu", GetCurrentProcessId());

    // Start child 1
    if (!CreateProcess(NULL, cmdLine1, NULL, NULL, FALSE, 0, NULL, NULL, &si1, &pi1)) {
        printf("CreateProcess (1) failed (%lu).\n", GetLastError());
        return 1;
    }

    // Start child 2
    if (!CreateProcess(NULL, cmdLine2, NULL, NULL, FALSE, 0, NULL, NULL, &si2, &pi2)) {
        printf("CreateProcess (2) failed (%lu).\n", GetLastError());
        return 1;
    }

    // Wait for both
    HANDLE handles[2] = { pi1.hProcess, pi2.hProcess };
    WaitForMultipleObjects(2, handles, TRUE, INFINITE);

    DWORD exitCode1, exitCode2;
    GetExitCodeProcess(pi1.hProcess, &exitCode1);
    GetExitCodeProcess(pi2.hProcess, &exitCode2);

    printf("Child 1 exited with code: %lu\n", exitCode1);
    printf("Child 2 exited with code: %lu\n", exitCode2);

    // Cleanup
    CloseHandle(pi1.hProcess); CloseHandle(pi1.hThread);
    CloseHandle(pi2.hProcess); CloseHandle(pi2.hThread);

    return 0;
}
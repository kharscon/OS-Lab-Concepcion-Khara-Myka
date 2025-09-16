#include <windows.h>
#include <stdio.h>

int main() {
    STARTUPINFO si = { sizeof(si) };
    PROCESS_INFORMATION pi;
    DWORD exitCode;

    // Example: run "cmd /c dir"
    char cmd[] = "cmd /c dir";

    if (!CreateProcess(NULL, cmd, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        printf("CreateProcess failed (%lu).\n", GetLastError());
        return 1;
    }

    WaitForSingleObject(pi.hProcess, INFINITE);

    GetExitCodeProcess(pi.hProcess, &exitCode);

    // Parent exits with the same code
    CloseHandle(pi.hProcess); CloseHandle(pi.hThread);
    return exitCode;
}
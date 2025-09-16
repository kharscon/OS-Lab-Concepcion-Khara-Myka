#include <windows.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: child_echo <parent_pid>\n");
        return 1;
    }
    DWORD parent_pid = atoi(argv[1]);
    printf("Child PID: %lu\n", GetCurrentProcessId());
        Sleep(30000); // Wait for 30 seconds
    printf("Parent PID (from argument): %lu\n", parent_pid);
    return 0;
}
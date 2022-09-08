
#include <Windows.h>
#include <stdio.h>

HANDLE Run(char* options) {
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(STARTUPINFO));
    ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));

    if (!CreateProcess(
            NULL, options,
                NULL, NULL, FALSE, FALSE, NULL, NULL, &si, &pi)) {
        return (HANDLE)NULL;
    }

    return pi.hProcess;
}

int main() {
    printf(
        "Hello from parent process!\n"
        "PID: %lu\n\n",
        GetCurrentProcessId());

    char* path = "hello.exe";
    HANDLE hProcess = Run(path);
    if (!hProcess) {
        printf("Can't run new process! Error code: %lu\n", GetLastError());
        return 0;
    }

    printf(
        "Child process created!\n"
        "Handle: %llu | PID: %lu\n\n",
        hProcess, GetProcessId(hProcess));
    
    printf("Wait for child process exit...\n");
    WaitForSingleObject(hProcess, INFINITE);
    printf("Bye!\n");
    return 0;
}

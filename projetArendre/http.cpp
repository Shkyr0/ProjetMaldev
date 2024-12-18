#include <windows.h>
#include <winhttp.h>
#include <stdio.h>
#include "http.h"

#pragma comment(lib, "winhttp.lib")

HINTERNET Open() {
    return WinHttpOpen(L"A WinHTTP Example Program/1.0", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
}

HINTERNET Connect(HINTERNET hSession, const wchar_t host[], int port) {
    return WinHttpConnect(hSession, host, port, 0);
}

HINTERNET OpenRequest(HINTERNET hConnect, const wchar_t path[]) {
    return WinHttpOpenRequest(hConnect, L"GET", path, NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, 0);
}

BOOL SendRequest(HINTERNET hRequest) {
    return WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0);
}
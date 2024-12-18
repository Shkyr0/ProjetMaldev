#ifndef HTTP_H
#define HTTP_H

HINTERNET Open();
HINTERNET Connect(HINTERNET hSession, const wchar_t host[], int port);
HINTERNET OpenRequest(HINTERNET hConnect, const wchar_t path[]);

BOOL SendRequest(HINTERNET hRequest);

#endif 
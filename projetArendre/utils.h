#include <winhttp.h>
#ifndef UTILS_H
#define UTILS_H

LPSTR CheckData(HINTERNET hRequest);

LPSTR Data(const wchar_t host[], int port, const wchar_t path[]);


#endif 

#ifndef INJECT_H
#define INJECT_H

#include <windows.h>

BOOL InjectShellcode(LPVOID payload, SIZE_T payloadSize, DWORD processId);

#endif

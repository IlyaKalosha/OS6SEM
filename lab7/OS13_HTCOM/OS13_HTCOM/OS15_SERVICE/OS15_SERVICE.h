#pragma once
#include <Windows.h>
#include <iostream>
#include <fstream>

#define SERVICENAME L"OS15_SERVICE"
#define TRACEPATH "C:\\Service\\Service.trace"

VOID WINAPI ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv);
VOID WINAPI ServiceHandler(DWORD fdwControl);

void trace(const char* msg, int r = std::ofstream::app);
#include "debug.hpp"

#ifdef _WIN32
#include <windows.h>
#endif
#include <iostream>

void engine::debug::initTerminal() {
#ifdef _WIN32
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hConsole == INVALID_HANDLE_VALUE) return;
	DWORD dwMode = 0;
	if (!GetConsoleMode(hConsole, &dwMode)) return;
	dwMode |= ENABLE_PROCESSED_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING;;
	SetConsoleMode(hConsole, dwMode);
#endif
}
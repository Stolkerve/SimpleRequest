#ifdef _WIN32
#include <windows.h>
#endif // _WIN_32

#include "App.h"

using namespace simple;

int main()
{
#ifdef _WIN32
    // enable ANSI sequences for windows 10:
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD consoleMode;
    GetConsoleMode(console, &consoleMode);
    consoleMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(console, consoleMode);
#endif

	App app;
	app.Start();
}
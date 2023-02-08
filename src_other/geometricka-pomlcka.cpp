#include <Windows.h>

#define UNICODE
#define _UNICODE

DWORD WINAPI func(void* me) {
    auto hwnd = FindWindowW(NULL, L"Geometry Dash");
    SetWindowTextW(hwnd, L"Geometrická pomlčka 2.2 přímo od Roberta Topaly CZ/SK v hlavní roli v českém znění Petr Rychlý");
    return 0;
}

BOOL APIENTRY DllMain(HMODULE mod, DWORD reason, LPVOID) {
    if (reason == DLL_PROCESS_ATTACH)
        if (auto h = CreateThread(NULL, 0, &func, mod, 0, NULL))
            CloseHandle(h);
    return TRUE;
}
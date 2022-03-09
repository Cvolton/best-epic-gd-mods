#include <Windows.h>

DWORD WINAPI func(void* me) {
    auto proc = GetCurrentProcess();
    auto base = reinterpret_cast<char*>(GetModuleHandle(NULL));
    //This patches the maximum number for SetIDPopup to INT_MAX
    unsigned char patch[] = {0x68, 0xFF, 0xFF, 0xFF, 0x7F};
    WriteProcessMemory(proc, base + 0x1431F6, patch, 5, NULL);
    //This patches the amount of characters allowed in the text input in SetIDPopup to 6 characters
    unsigned char patch2[] = {0x06};
    WriteProcessMemory(proc, base + 0x14371C, patch2, 1, NULL);
    FreeLibraryAndExitThread(reinterpret_cast<HMODULE>(me), 0);
    return 0;
}

BOOL APIENTRY DllMain(HMODULE mod, DWORD reason, LPVOID) {
    if (reason == DLL_PROCESS_ATTACH)
        if (auto h = CreateThread(NULL, 0, &func, mod, 0, NULL))
            CloseHandle(h);
    return TRUE;
}
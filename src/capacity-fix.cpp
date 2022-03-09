#include <Windows.h>

const unsigned char node_offset = 0x1B;
const size_t base_address_21 = 0x10C582 - (node_offset * 0x10);

DWORD WINAPI func(void* me) {
    auto proc = GetCurrentProcess();
    auto base = reinterpret_cast<char*>(GetModuleHandle(NULL));
    //2.0 fix
    static const size_t addresses[] = {0x10C519, 0x10C534, 0x10C560};
    for(unsigned char i = 0; i < sizeof(addresses); i++){
        unsigned char patch[] = {static_cast<unsigned char>(i + 0x0D)};
        WriteProcessMemory(proc, base + addresses[i], patch, 1, NULL);
    }
    //2.1 fix
    for(unsigned char i = 0x10; i < 53; i++){
        size_t address = (node_offset * i) + base_address_21;
        unsigned char patch[] = {i};
        WriteProcessMemory(proc, base + address, patch, 1, NULL);
    }
    //change check that checks whether 2.0 nodes should even be loaded
    unsigned char patch[] = {0x9};
    WriteProcessMemory(proc, base + 0x10C35B, patch, 1, NULL);
    //change check that checks whether 2.1 nodes should even be loaded
    patch[0] = 0x35;
    WriteProcessMemory(proc, base + 0x10C558, patch, 1, NULL);

    FreeLibraryAndExitThread(reinterpret_cast<HMODULE>(me), 0);
    return 0;
}

BOOL APIENTRY DllMain(HMODULE mod, DWORD reason, LPVOID) {
    if (reason == DLL_PROCESS_ATTACH)
        if (auto h = CreateThread(NULL, 0, &func, mod, 0, NULL))
            CloseHandle(h);
    return TRUE;
}
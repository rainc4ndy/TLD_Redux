#include <memory>
#include "Plugin.h"
#include "d3dhook.h"
#include "CLog.h"
#include "Cheats/Bot/CBotManager.h"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved) {
    switch (dwReason) {
        case DLL_PROCESS_ATTACH: {
            DisableThreadLibraryCalls(hModule);
            auto& p = Plugin::getInstance();
            p.init();
            break;
        }

        case DLL_PROCESS_DETACH:
            DestoryD3D9Hooks();
            break;
        default:
            break;
    }
    return TRUE;
}

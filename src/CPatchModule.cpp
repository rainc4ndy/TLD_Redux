//
// Created by Administrator on 2025/2/15.
//

#include "CPatchModule.h"

CPatchModule::CPatchModule(std::string name, std::string nameEn) : CCheatModule(name, nameEn) {
}

void CPatchModule::patchAddress(void* address, const char* patchData, size_t size) {
    // Save the current protection settings of the memory page.
    DWORD oldProtect;
    VirtualProtect(address, size, PAGE_EXECUTE_READWRITE, &oldProtect);

    // Overwrite the memory with the patch data.
    memcpy(address, patchData, size);

    // Restore the original protection settings.
    VirtualProtect(address, size, oldProtect, &oldProtect);

    // Flush the instruction cache to ensure the changes take effect immediately.
    FlushInstructionCache(GetCurrentProcess(), address, size);
}

void CPatchModule::onEnable() {
    work();
}
void CPatchModule::onDisable() {
    work();
}


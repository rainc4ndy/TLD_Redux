//
// Created by Administrator on 2025/2/15.
//

#include "WepPatch.h"

namespace CheatModule::Weapon {
    AllAxisAim::AllAxisAim() : CPatchModule("全视角瞄准", "All Axis Aim") {}
    void AllAxisAim::work() {
        /* CTaskSimpleUseGun::ProcessPed */
        patchAddress(reinterpret_cast<void *>(0x62AE1B), enabled ? "\xEB" : "\x75", 0x1);
        patchAddress(reinterpret_cast<void *>(0x62AE30), enabled ? nullptr : "\x7A\x06", 0x2);
        patchAddress(reinterpret_cast<void *>(0x62AE5A), enabled ? nullptr : "\x74\x19", 0x2);
    }
} // CheatModule
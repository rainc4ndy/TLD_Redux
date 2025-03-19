//
// Created by Administrator on 2025/2/15.
//

#include "PedPatch.h"

#include <CCollision.h>
namespace CheatModule::Player {
    NoCameraCol::NoCameraCol() : CPatchModule("无镜头碰撞", "No Camera Col") {}
    void NoCameraCol::work() {
        /* CheckCameraCollisionPeds */
        patchAddress(reinterpret_cast<void *>(0x415320), enabled ? "\xC3\x90\x90" : "\x83\xEC\x18", 0x3);
        /* CheckCameraCollisionBuildings */
        patchAddress(reinterpret_cast<void *>(0x41A820), enabled ? "\xC3\x90\x90\x90" : "\x8B\x44\x24\x04", 0x4);
        /* CheckCameraCollisionVehicles */
        patchAddress(reinterpret_cast<void *>(0x41A990), enabled ? "\xC3\x90\x90\x90" : "\x8B\x44\x24\x08", 0x4);
        /* CheckCameraCollisionObjects */
        patchAddress(reinterpret_cast<void *>(0x41AB20), enabled ? "\xC3\x90\x90\x90" : "\x8B\x44\x24\x08", 0x4);
    }

    InfiniteRun::InfiniteRun() : CPatchModule("无限奔跑耐力", "Infinite Run") {}
    void InfiniteRun::work() {
        patchAddress(reinterpret_cast<void *>(0x60A570), enabled ? "\xEB" : "\x75", 0x1);
    }

    AntiFall::AntiFall() : CPatchModule("免疫摔伤", "Anti Fall") {}
    void AntiFall::work() {
        patchAddress(reinterpret_cast<void *>(0x680B7A), enabled ? "\xE9\xFB\xFE\xFF\xFF\x90" : "\x0F\x84\xFA\xFE\xFF\xFF", 0x6);
    }

    LongHP::LongHP() : CPatchModule("长血条", "Long HP") {}
    void LongHP::work() {
        patchAddress(reinterpret_cast<void *>(0xB793E1), enabled ? "\x00\x64" : "\x40\x0E", 0x2);
    }
} // CCheatModule
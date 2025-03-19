#include "NoSpread.h"

#include <sampapi/CEntity.h>
#include <sampapi/CLocalPlayer.h>
#include <sampapi/CNetGame.h>
#include <sampapi/CPlayerPool.h>

#include <RakHook/rakhook.hpp>

#include "../../samp.h"
#include "../../scripting.h"
#include "../../Utils.h"

namespace CheatModule {
namespace Weapon {

NoSpread::NoSpread() : CCheatModule("无后座", "No Spread") {
}

void NoSpread::onEnable() {
    for (int addr = 0xC8C450; addr < 0xC8C8B0; addr += 0x70)
	*(DWORD *)addr = 0x50000000;
}

void NoSpread::onDisable() {
    for (int addr = 0xC8C450; addr < 0xC8C8B0; addr += 0x70)
	*(DWORD *)addr = 0x3F800000;
}

}  // namespace Hack
}  // namespace CheatModule
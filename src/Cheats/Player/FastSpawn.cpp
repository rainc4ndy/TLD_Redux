#include "FastSpawn.h"

#include <sampapi/CEntity.h>
#include <sampapi/CLocalPlayer.h>
#include <sampapi/CNetGame.h>
#include <sampapi/CPlayerPool.h>

#include "../../samp.h"
#include "../../scripting.h"
#include "../../Utils.h"

namespace CheatModule {
namespace Player {
FastSpawn::FastSpawn() : CCheatModule("立刻复活", "Fast Spawn") {}

void FastSpawn::onProcess() {
    auto p = samp::RefNetGame()->GetPlayerPool()->GetLocalPlayer();
    if (p) {
        auto ped = p->GetPed();
        if (ped->GetHealth() <= 0.f)
            ped->SetHealth(100.f);
    }
}

}  // namespace Player
}  // namespace CheatModule
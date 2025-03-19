#include "RandomHealth.h"

#include <sampapi/CEntity.h>
#include <sampapi/CLocalPlayer.h>
#include <sampapi/CNetGame.h>
#include <sampapi/CPlayerPool.h>

#include "../../samp.h"
#include "../../scripting.h"
#include "../../Utils.h"

#include <algorithm>
namespace CheatModule {
namespace Player {

RandomHealth::RandomHealth() : CCheatModule("血量脉冲", "Pulsing Health") {}

void RandomHealth::onProcess() {
    auto p = samp::RefNetGame()->GetPlayerPool()->GetLocalPlayer()->GetPed();

    if (isReversed) {
        p->SetHealth(std::min(100.f, p->GetHealth()+1));
    } else {
        p->SetHealth(std::max(1.f, p->GetHealth()-1));
    }

    float f = p->GetHealth();
    if (f >= 100.f || f <= 1.0f )
        isReversed ^= 1;
}

}
}
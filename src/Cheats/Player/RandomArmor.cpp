
#include "RandomArmor.h"

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

RandomArmor::RandomArmor() : CCheatModule("护甲脉冲", "Pulsing Armor") {}

void RandomArmor::onProcess() {
    auto p = samp::RefNetGame()->GetPlayerPool()->GetLocalPlayer()->GetPed();

    if (isReversed) {
        p->SetArmour(std::min(100.f, p->GetArmour()+1));
    } else {
        p->SetArmour(std::max(1.f, p->GetArmour()-1));
    }

    float f = p->GetArmour();
    if (f >= 100.f || f <= 1.0f )
        isReversed ^= 1;
}

}
}
#include "NoReload.h"

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

NoReload::NoReload() : CCheatModule("无需装弹", "No Reload") {
}

void NoReload::onProcess() {
    auto me = samp::RefNetGame()->GetPlayerPool()->GetLocalPlayer();
    if (me->GetPed()->m_pGamePed) {
        auto weapon =  me->GetPed()->GetCurrentWeapon();
        int wID = weapon;
        if (wID == 22 || wID == 23 || wID == 24 || wID == 26 || wID == 27 || wID == 28 || wID == 29 ||
            wID == 30 || wID == 31 || wID == 32 || wID == 37 || wID == 38 || wID == 41 || wID == 42) {
            if (me->GetPed()->GetCurrentWeaponAmmo() == 1 /*|| pInfo->weapon[pInfo->weapon_slot].ammo == 1*/)
            {
                int tmp = me->GetPed()->GetCurrentWeapon();
                me->GetPed()->SetArmedWeapon(0);
                me->GetPed()->SetArmedWeapon(tmp);
            }
        }
    }
    else return;
}

}  // namespace Hack
}  // namespace CheatModule
#include "NPCKiller.h"

#include <BitStream.h>
#include <sampapi/CEntity.h>
#include <sampapi/CLocalPlayer.h>
#include <sampapi/CNetGame.h>
#include <sampapi/CPlayerPool.h>

#include <RakHook/rakhook.hpp>

#include "../../Utils.h"
#include "../../samp.h"

namespace CheatModule {
namespace Weapon {

NPCKiller::NPCKiller() : CCheatModule("NPC秒杀", "NPC Instant Kill") {}

bool CheatModule::Weapon::NPCKiller::onProcess(bool fromServer, int rpc,
                                               RakNet::BitStream* bs) {
    if (rpc == RPC_GiveTakeDamage && !fromServer) {
        bool bGiveOrTake;
        UINT16 wPlayerID;
        float damage_amount;
        UINT32 dWeaponID;
        UINT32 dBodypart;

        bs->SetReadOffset(0);
        bs->Read(bGiveOrTake);
        bs->Read(wPlayerID);
        bs->Read(damage_amount);
        bs->Read(dWeaponID);
        bs->Read(dBodypart);

        if (!bGiveOrTake) {
            damage_amount = 65535.0f;
        }

        bs->SetWriteOffset(0);
        bs->Write(bGiveOrTake);
        bs->Write(wPlayerID);
        bs->Write(damage_amount);
        bs->Write(dWeaponID);
        bs->Write(dBodypart);
    }
    return true;
}

void NPCKiller::killAll() {
    auto p = samp::RefNetGame()->GetPlayerPool();
    for (int i = 0; i < p->MAX_PLAYERS; i++) {
        if (!p->IsConnected(i) || !p->IsNPC(i)) continue;

        if (p->GetPlayer(i)->m_pPed) {
            RakNet::BitStream bs;
            bs.Reset();
            bool bGiveOrTake = false;
            UINT16 wPlayerID = i;
            float damage_amount = 65535.0f;
            UINT32 dWeaponID = samp::RefNetGame()
                                   ->GetPlayerPool()
                                   ->GetLocalPlayer()
                                   ->GetPed()
                                   ->GetCurrentWeapon();
            UINT32 dBodypart = 0;

            bs.SetWriteOffset(0);
            bs.Write(bGiveOrTake);
            bs.Write(wPlayerID);
            bs.Write(damage_amount);
            bs.Write(dWeaponID);
            bs.Write(dBodypart);

            rakhook::send_rpc(RPC_GiveTakeDamage, &bs, HIGH_PRIORITY,
                              RELIABLE_ORDERED, 0, false);
        }
    }
}

}  // namespace Weapon
}  // namespace CheatModule
#include "GodMode.h"

#include <common.h>
#include <sampapi/CLocalPlayer.h>
#include <sampapi/CPlayerPool.h>
#include <sampapi/CNetGame.h>
#include <sampapi/CEntity.h>

#include "../../samp.h"

namespace CheatModule {
namespace Player {
GodMode::GodMode() : CCheatModule("无敌", "God Mode") {}

#define ACTOR_FLAGS_INVULNERABLE	( 4 | 8 | 64 | 128 )

void GodMode::onProcess() {
    if (GetTickCount() - tick >= 1000) {
        auto ped = FindPlayerPed(-1);
        ped->m_nPhysicalFlags.bBulletProof = 1;
        ped->m_nPhysicalFlags.bCollisionProof = 1;
        ped->m_nPhysicalFlags.bExplosionProof = 1;
        ped->m_nPhysicalFlags.bFireProof = 1;
        ped->m_nPhysicalFlags.bMeleeProof = 1;
        tick = GetTickCount();
    }
}

bool GodMode::onProcess(bool fromServer, int rpc, RakNet::BitStream* bs) {
    if (rpc == RPC_SetPlayerHealth) {
        return false;
    }
    else if (rpc == RPC_SetPlayerArmour) {
        return false;
    }
    return true;
}

void GodMode::onEnable() {
    auto ped = FindPlayerPed(-1);
    ped->m_nPhysicalFlags.bBulletProof = 1;
    ped->m_nPhysicalFlags.bCollisionProof = 1;
    ped->m_nPhysicalFlags.bExplosionProof = 1;
    ped->m_nPhysicalFlags.bFireProof = 1;
    ped->m_nPhysicalFlags.bMeleeProof = 1;
}

void GodMode::onDisable() {
    auto ped = FindPlayerPed(-1);
    ped->m_nPhysicalFlags.bBulletProof = 0;
    ped->m_nPhysicalFlags.bCollisionProof = 0;
    ped->m_nPhysicalFlags.bExplosionProof = 0;
    ped->m_nPhysicalFlags.bFireProof = 0;
    ped->m_nPhysicalFlags.bMeleeProof = 0;
}

}  // namespace Player
}
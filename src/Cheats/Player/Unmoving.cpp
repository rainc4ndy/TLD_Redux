#include "Unmoving.h"

#include <BitStream.h>
#include <sampapi/CEntity.h>
#include <sampapi/CLocalPlayer.h>
#include <sampapi/CNetGame.h>
#include <sampapi/CPlayerPool.h>

#include "../../samp.h"

namespace CheatModule {
namespace Player {
Unmoving::Unmoving() : CCheatModule("位置保护", "Movement Protection") {}

bool Unmoving::onProcess(bool fromServer, int rpc, RakNet::BitStream* bs) {
    switch (rpc) {
        case RPC_SetPlayerPos:
        case RPC_SetPlayerPosFindZ:
        case RPC_SetPlayerVelocity:
        case RPC_SetPlayerInterior:
        case RPC_SetPlayerFacingAngle:
        case RPC_TogglePlayerControllable:
        case RPC_RemovePlayerFromVehicle:
        case RPC_ClearAnimations:
        case RPC_ApplyAnimation:
            return false;
        case RPC_SetVehiclePos: 
        case RPC_SetVehicleZAngle: {
            UINT16 wVehicleID;
            bs->Read(wVehicleID);
            auto p = samp::RefNetGame()->GetPlayerPool()->GetLocalPlayer();
            if (p->m_nCurrentVehicle == wVehicleID) {
                return false;
            }
        }
    }
    return true;
}

}  // namespace Player
}  // namespace CheatModule
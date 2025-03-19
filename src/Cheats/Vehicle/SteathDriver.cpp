#include "SteathDriver.h"

#include <BitStream.h>
#include <common.h>
#include <d3dx9.h>
#include <d3d9.h>
#include <RakHook/rakhook.hpp>

#include <sampapi/CLocalPlayer.h>
#include <sampapi/CNetGame.h>
#include <sampapi/CPlayerPool.h>

#include <CTaskSimpleCarSetPedOut.h>

#include "../../CarUtils.h"
#include "../../types.h"
#include "../../Utils.h"

namespace CheatModule::Vehicle {

    SteathDriver::SteathDriver() : CCheatModule("隐身驾驶", "Invisibility (Driver)") {}

    bool SteathDriver::onProcess(stInCarData* data) {
        //UNOCCUPIED
        stUnoccupiedData unoccupiedData{};
        unoccupiedData.byteSeatID = 0;
        unoccupiedData.sVehicleID = data->sVehicleID;
        unoccupiedData.fHealth = data->fVehicleHealth;

        MATRIX4X4 matrix;
        CUtils::GetMatrixFromQuaternion(data->fQuaternion, &matrix);

        memcpy(unoccupiedData.fPosition, &data->fPosition, sizeof(float) * 3);
        memcpy(unoccupiedData.fMoveSpeed, &data->fMoveSpeed, sizeof(float) * 3);
        memcpy(unoccupiedData.fDirection, &matrix.up, sizeof(float) * 3);
        memcpy(unoccupiedData.fRoll, &matrix.right, sizeof(float) * 3);

        RakNet::BitStream bsUnoccupied;
        bsUnoccupied.Write((BYTE)ID_UNOCCUPIED_SYNC);
        bsUnoccupied.Write((PCHAR)&unoccupiedData, sizeof(stOnFootData));
        rakhook::send(&bsUnoccupied, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0);

        //ONFOOT
        stOnFootData onfootData{};

        memcpy(onfootData.fPosition, data->fPosition, sizeof(float) * 3);
        onfootData.fPosition[2] += 0.4f;

        auto player = samp::RefNetGame()->GetPlayerPool()->GetLocalPlayer();
        onfootData.byteHealth = static_cast<BYTE>(player->GetPed()->GetHealth());
        onfootData.byteArmor = static_cast<BYTE>(player->GetPed()->GetArmour());

        onfootData.sSurfingVehicleID = data->sVehicleID;
        onfootData.fSurfingOffsets[2] = 65535.f;

        onfootData.fMoveSpeed[2] = -0.01f;

        RakNet::BitStream bsOnFoot;
        bsOnFoot.Write((BYTE)ID_PLAYER_SYNC);
        bsOnFoot.Write((PCHAR)&onfootData, sizeof(stOnFootData));
        rakhook::send(&bsOnFoot, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0);
        return false;
    }

    void SteathDriver::onDisable() {
        CarUtils::safelyBackInCar();
    }
}

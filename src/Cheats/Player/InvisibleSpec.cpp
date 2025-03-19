#include "InvisibleSpec.h"

#include <BitStream.h>
#include <RakHook/rakhook.hpp>
#include <sampapi/CVehicle.h>
#include <sampapi/CNetGame.h>

#include "../../Utils.h"
#include "../../samp.h"


namespace CheatModule {
namespace Player {

InvisibleSpec::InvisibleSpec() : CCheatModule("隐身", "Invisibility") {
}

bool InvisibleSpec::onProcess(stOnFootData *data) {
    switch (mode) {
        case Invis_Pos: {
            data->fPosition[2] = 65535.f;
            break;
        }
            
        case Invis_Spec: {
            RakNet::BitStream bsData;
            stSpectatorData InvisHack;

            CUtils::vect3_copy(data->fPosition, InvisHack.fPosition);
            InvisHack.sLeftRightKeys = data->sLeftRightKeys;
            InvisHack.sUpDownKeys = data->sUpDownKeys;
            InvisHack.sKeys = data->sKeys;
            InvisHack.stSampKeys = data->stSampKeys;

            bsData.Reset();
            bsData.Write((BYTE)ID_SPECTATOR_SYNC);
            bsData.Write((PCHAR)&InvisHack, sizeof(stSpectatorData));
            rakhook::send(&bsData, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0);
            return false;
        }
            
        case Invis_Deton: {
            if (!m_bInvisDetonState) {
                data->byteCurrentWeapon = 40;
                data->sKeys = 128;
                data->fMoveSpeed[0] = 0.2f;
            }
            m_bInvisDetonState = !m_bInvisDetonState;
            break;
        }
        case Invis_SurfVehicle: {
            auto id = samp::RefNetGame()->GetVehiclePool()->GetNearest();
            if (id != -1) {
                data->sSurfingVehicleID = id;
                data->fSurfingOffsets[0] = data->fSurfingOffsets[1] = data->fSurfingOffsets[2] = sqrt((float)-1);
            }
            break;
        }
        case Invis_SurfObject: {
            data->fSurfingOffsets[2] = 1.0f;
            data->sSurfingVehicleID = 2002;
            break;
        }
    }

    return true;
}

bool InvisibleSpec::onProcess(stAimData *data) {
    switch (mode) {
        case Invis_Deton: {
            if (!m_bInvisDetonState) {
            	data->byteCamMode = 51;
            }
            break;
        }
    }
    return true;
}

void InvisibleSpec::onSave(nlohmann::json &j) {
    saveData<decltype(mode)>(j, "mode", mode);
}

void InvisibleSpec::onLoad(const nlohmann::json &j) {
    loadData<decltype(mode)>(j, "mode", mode);
}

}  // namespace Player
}  // namespace CheatModule
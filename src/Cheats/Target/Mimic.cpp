//
// Created by Administrator on 2024/10/7.
//

#include "Mimic.h"

#include <BitStream.h>
#include <sampapi/CLocalPlayer.h>
#include <sampapi/CNetGame.h>
#include <sampapi/CPlayerPool.h>

#include <common.h>
#include <CVector.h>
#include "../../Plugin.h"
#include "../../CConfig.h"
#include "../../TargetSelector.h"
#include "../../Utils.h"

namespace CheatModule::Target {
    void Mimic::getOffsetedLocation(CVector ori, CVector &loc, float angle) {
        switch (offset_mode) {
            case EMO_FRONT: {
                loc = CUtils::GetOffsetedPosition(ori, CVector(0.0f, 2.0f, 0.0f), angle);
                break;
            }
            case EMO_BACK: {
                loc = CUtils::GetOffsetedPosition(ori, CVector(0.0f, -2.0f, 0.0f), angle);
                break;
            }
            case EMO_LEFT: {
                loc = CUtils::GetOffsetedPosition(ori, CVector(-2.0f, 0.0f, 0.0f), angle);
                break;
            }
            case EMO_RIGHT: {
                loc = CUtils::GetOffsetedPosition(ori, CVector(2.0f, 2.0f, 0.0f), angle);
                break;
            }
        }
    }

    void Mimic::onSave(nlohmann::json &j) {
        saveData(j, "offset_mode", offset_mode);
        saveData(j, "local", local);
    }

    void Mimic::onLoad(const nlohmann::json &j) {
        loadData(j, "offset_mode", offset_mode);
        loadData(j, "local", local);
    }

    Mimic::Mimic() : CCheatModule("模仿者", "Mimic") { tick = 0;}
    void Mimic::onProcess() {
        auto target = Plugin::getInstance().getTarget();
        if (GetTickCount() - tick >= 40) {
            if (target->targetPlayerPed) {
                auto remote_player = samp::RefNetGame()->GetPlayerPool()->GetPlayer(target->targetPlayerID);
                auto self = samp::RefNetGame()->GetPlayerPool()->GetLocalPlayer()->GetPed();

                stOnFootData copied {};
                auto d = remote_player->m_onfootData;
                copied.byteHealth = static_cast<uint8_t>(self->GetHealth());
                copied.byteArmor = static_cast<uint8_t>(self->GetArmour());

                CVector pos;
                getOffsetedLocation(CVector(d.m_position.x,  d.m_position.y, d.m_position.z), pos, target->targetPlayerPed->GetHeading());
                copied.fPosition[0] = pos.x;
                copied.fPosition[1] = pos.y;
                copied.fPosition[2] = pos.z;

                copied.sCurrentAnimationID = d.m_animation.m_nId;
                copied.sAnimFlags = d.m_animation.m_value;
                copied.byteSpecialAction = d.m_nSpecialAction;
                copied.fQuaternion[0] = d.m_fQuaternion[0];
                copied.fQuaternion[1] = d.m_fQuaternion[1];
                copied.fQuaternion[2] = d.m_fQuaternion[2];
                copied.fQuaternion[3] = d.m_fQuaternion[3];
                copied.fMoveSpeed[0] = d.m_speed.x;
                copied.fMoveSpeed[1] = d.m_speed.y;
                copied.fMoveSpeed[2] = d.m_speed.z;
                copied.sLeftRightKeys = d.m_controllerState.m_sLeftStickX;
                copied.sUpDownKeys = d.m_controllerState.m_sLeftStickY;
                copied.sKeys = d.m_controllerState.m_value;

                RakNet::BitStream bs;
                bs.Reset();
                bs.Write(static_cast<BYTE>(ID_PLAYER_SYNC));
                bs.Write(reinterpret_cast<PCHAR>(&copied), sizeof(stOnFootData));
                rakhook::send(&bs,  HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0);
                tick = GetTickCount();
            }
        }
        if (local) {
            if (target->targetPlayerPed) {
                auto self = FindPlayerPed(-1);
                getOffsetedLocation(target->targetPlayerPed->m_matrix->pos, self->m_matrix->pos, target->targetPlayerPed->GetHeading());
                self->m_matrix->up = target->targetPlayerPed->m_matrix->up;
                self->m_matrix->right = target->targetPlayerPed->m_matrix->right;
                self->m_vecMoveSpeed = target->targetPlayerPed->m_vecMoveSpeed;
            }
        }
    }

    bool Mimic::onProcess(stOnFootData *data) {
        auto target = Plugin::getInstance().getTarget();
        if (target->targetPlayerID == -1)
            return true;

        return false;
    }
}

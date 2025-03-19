//
// Created by Administrator on 2025/2/15.
//

#include "CTriggerBot.h"

#include <common.h>
#include <CCamera.h>
#include <CWorld.h>
#include "Utils.h"

namespace CheatModule::Weapon {
    CTriggerBot::CTriggerBot(): CCheatModule("自动扳机", "Trigger Bot") {
    }

    void CTriggerBot::onProcess() {

        CCheatModule::onProcess();

        static auto isPed = [](int model) -> bool {
            return model >= 0 && model <= 311;
        };

        auto self = FindPlayerPed(-1);
        auto ui16Mode = TheCamera.m_aCams[0].m_nMode;
        if (ui16Mode == 53 || ui16Mode == 55) {
            CVector vecCamera {}, vecPoint{};
            TheCamera.Find3rdPersonCamTargetVector(range, self->GetPosition(), &vecCamera, &vecPoint);
            CColPoint cColPoint;
            CEntity *pEntity{nullptr};
            if (CWorld::ProcessLineOfSight(vecCamera, vecPoint, cColPoint, pEntity, true, true, true, true, true, true, false, true ) &&
            pEntity != nullptr && isPed(pEntity->m_nModelIndex)) {
                CPad::GetPad(0)->PCTempJoyState.ButtonCircle = 255;
            }
        }
    }

    void CTriggerBot::onSave(nlohmann::json &j) {
        CCheatModule::onSave(j);
        saveData(j, "range", range);
    }

    void CTriggerBot::onLoad(const nlohmann::json &j) {
        CCheatModule::onLoad(j);
        loadData(j, "range", range);
    }
} // CCheatModule
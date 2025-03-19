//
// Created by Administrator on 2024/10/7.
//

#include "SupremeDashing.h"

#include <BitStream.h>
#include <sampapi/CNetGame.h>

#include <common.h>

#include "CarTrollSync.h"
#include "../../CarUtils.h"
#include "../../Utils.h"

namespace CheatModule::Vehicle {
    SupremeDashing::SupremeDashing(): CCheatModule("至尊冲击", "Supreme Dashing") {
    }

    void SupremeDashing::RestoreIncar() {
        CarUtils::safelyBackInCar();
        state = SD_WAIT_FOR_INCAR;
    }

    void SupremeDashing::onProcess() {
        if (state == SD_IDLE || state == SD_WORKING) {
            if (GetTickCount() - tick >= 40) {
                auto self = FindPlayerPed(-1);
                if (!self->m_nPedFlags.bInVehicle)
                    return;
                auto vehicle = self->m_pVehicle;
                int vehicleid = samp::RefNetGame()->GetVehiclePool()->Find(vehicle);
                if (vehicleid == -1)
                    return;

                auto pool = samp::RefNetGame()->GetPlayerPool();
                float distance = 16.0f;

                //Get the nearest target
                CPed *target = nullptr;
                for (int i = 0; i < samp::CPlayerPool::MAX_PLAYERS; i++) {
                    if (pool->IsNPC(i) || !pool->m_bNotEmpty[i] || pool->m_localInfo.m_nId == i)
                        continue;
                    auto ped = pool->GetPlayer(i)->m_pPed;
                    if (!ped)
                        continue;
                    float d = DistanceBetweenPoints(pool->GetLocalPlayer()->m_pPed->m_pGamePed->GetPosition(),
                                                    ped->m_pGamePed->GetPosition());
                    if (d < distance) {
                        distance = d;
                        target = pool->GetPlayer(i)->m_pPed->m_pGamePed;
                    }
                }

                if (target == nullptr) {
                    if (state == SD_WORKING) {
                        RestoreIncar();
                    }
                    tick = GetTickCount();
                    return;
                }
                targetPosition = target->GetPosition();

                RakNet::BitStream bs;
                stOnFootData onFoot{};
                onFoot.fPosition[0] = vehicle->m_matrix->pos.x;
                onFoot.fPosition[1] = vehicle->m_matrix->pos.y;
                onFoot.fPosition[2] = vehicle->m_matrix->pos.z;
                onFoot.byteArmor = static_cast<BYTE>(self->m_fArmour);
                onFoot.byteHealth = static_cast<BYTE>(self->m_fHealth);
                onFoot.fMoveSpeed[2] = 0.02f;
                onFoot.fSurfingOffsets[2] = -4.0f;
                onFoot.sSurfingVehicleID = vehicleid;
                bs.Reset();
                bs.Write((BYTE) ID_PLAYER_SYNC);
                bs.Write((PCHAR) &onFoot, sizeof(stOnFootData));
                rakhook::send(&bs, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0);

                CarTrollSync sync(vehicleid, CarTrollSync::SYNC_UNOCCUPIED, false);
                CVector ori = vehicle->GetPosition();
                CVector direction = targetPosition - ori;
                direction.Normalise(); // Normalize the direction
                sync.setVelocity(direction * speed);
                sync.setPosition(ori);
                sync.execute();

                state = SD_WORKING;
                tick = GetTickCount();
            }
        } else if (state == SD_WAIT_FOR_INCAR) {
            if (GetTickCount() - tick >= 500) {
                state = SD_IDLE;
                auto self = FindPlayerPed(-1);
                if (!self->m_nPedFlags.bInVehicle)
                    return;
                auto vehicle = self->m_pVehicle;

                int vehicleid = samp::RefNetGame()->GetVehiclePool()->Find(vehicle);
                if (vehicleid == -1)
                    return;

                CarTrollSync sync(vehicleid, CarTrollSync::SYNC_INCAR, false);
                sync.setPosition(vehicle->GetPosition());
                sync.setVelocity(CVector(0,0,0));
                sync.execute();
            }
        }
    }

    bool SupremeDashing::onProcess(stInCarData *data) {
        if (state != SD_IDLE)
            return false;
        return true;
    }

    void SupremeDashing::onEnable() {
        state = SD_IDLE;
    }

    void SupremeDashing::onDisable() {
        RestoreIncar();
    }

    void SupremeDashing::onSave(nlohmann::json &j) {
        saveData(j, "speed", speed);
    }

    void SupremeDashing::onLoad(const nlohmann::json &j) {
        loadData(j, "speed", speed);
    }
}

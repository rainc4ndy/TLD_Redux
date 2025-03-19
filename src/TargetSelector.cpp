#include "TargetSelector.h"
#include <sampapi/CNetGame.h>
#include <CVehicle.h>
#include <CPlayerPed.h>
#include <common.h>

#include "Plugin.h"
#include "samp.h"

void CTargetSelector::updateTick() {
    if (delay >= 0) {
        tick = GetTickCount();
    }
}


CTargetSelector::CTargetSelector() {
    targetPlayerPed = NULL;
    targetPlayerID = -1;
}


bool CTargetSelector::get() {
    auto pool = samp::RefNetGame()->GetPlayerPool();
    int me = pool->m_localInfo.m_nId;

    if (delay >= 0 && mode != TM_SINGLE) {
        if (GetTickCount() - tick < delay) {
            return false;
        }
    }
    switch (mode) {
        case TM_SINGLE: {
            targetPlayerID = specifiedPlayerID;
            auto samp_ped = pool->GetPlayer(specifiedPlayerID)->m_pPed;
            if (samp_ped) {
                targetPlayerPed = samp_ped->m_pGamePed;
            } else targetPlayerPed = nullptr;
            return true;
        }

        case TM_NEAREST: {
            float distance = std::numeric_limits<float>::max();
            for (int i = 0; i < samp::CPlayerPool::MAX_PLAYERS; i++) {
                if (i == me || (pool->IsNPC(i) && !NPCIncluded) || !pool->m_bNotEmpty[i])
                    continue;

                auto ped = pool->GetPlayer(i)->m_pPed;
                if (!ped)
                    continue;

                float d = DistanceBetweenPoints(pool->GetLocalPlayer()->m_pPed->m_pGamePed->GetPosition(),
                                                ped->m_pGamePed->GetPosition());
                if (d < distance) {
                    distance = d;
                    targetPlayerID = i;
                    targetPlayerPed = pool->GetPlayer(targetPlayerID)->m_pPed->m_pGamePed;
                }
            }
            updateTick();
            return targetPlayerID != -1;
        }

        case TM_RANDOM: {
            std::vector<int> sel;
            for (int i = 0; i < samp::CPlayerPool::MAX_PLAYERS; i++) {
                if (i == me || (pool->IsNPC(i) && !NPCIncluded) || !pool->m_bNotEmpty[i])
                    continue;
                auto ped = pool->GetPlayer(i)->m_pPed;
                if (!ped)
                    continue;

                sel.push_back(i);
            }
            if (!sel.empty()) {
                targetPlayerID = sel[rand() % sel.size()];
                targetPlayerPed = pool->GetPlayer(targetPlayerID)->m_pPed->m_pGamePed;
                updateTick();
                return true;
            }
            break;
        }

        case TM_RANDOM_SERVER: {
            std::vector<int> sel;
            for (int i = 0; i < samp::CPlayerPool::MAX_PLAYERS; i++) {
                if (!pool->m_bNotEmpty[i] || i == me || (pool->IsNPC(i) && !NPCIncluded))
                    continue;
                sel.push_back(i);
            }
            if (!sel.empty()) {
                targetPlayerID = sel[rand() % sel.size()];
                updateTick();
                return true;
            }
            break;
        }
    }
    targetPlayerID = -1;
    targetPlayerPed = nullptr;
    return false;
}

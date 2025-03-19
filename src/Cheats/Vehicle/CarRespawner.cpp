//
// Created by Administrator on 2024/10/6.
//

#include "CarRespawner.h"

#include <BitStream.h>
#include <RakHook/rakhook.hpp>
#include <sampapi/CNetGame.h>
#include <sampapi/CVehiclePool.h>

#include "../../CarUtils.h"
#include "../../Utils.h"

namespace CheatModule::Vehicle {
    void CarRespawner::hack() {
        RakNet::BitStream FkbsRPC;
        int count = 0;
        auto pool = samp::RefNetGame()->GetVehiclePool();
        for (int v = 0; v < pool->MAX_VEHICLES; v++) {
            if (!pool->m_bIsActive[v])
                continue;
            if (pool->m_pObject[v] == nullptr)
                continue;

            FkbsRPC.Reset();
            FkbsRPC.Write(v);
            rakhook::send_rpc(RPC_VehicleDestroyed,  &FkbsRPC, HIGH_PRIORITY, RELIABLE_ORDERED, 0, false);
            count++;
        }
        CUtils::chatprintf("---> %d vehicles were forced to respawn", count);
    }
}

#include "CarUtils.h"

#include <BitStream.h>
#include <sampapi/CNetGame.h>
#include <sampapi/CVehiclePool.h>



#include <CVehicle.h>
#include <CPlayerPed.h>
#include <CPools.h>
#include <common.h>

#include "CConfig.h"
#include "CLog.h"
#include "Plugin.h"
int CarUtils::last_hacking_vehicle = -1;
unsigned int CarUtils::enter_tick = 0;

int CarUtils::getHackingVehicle(float distance) {
    auto config = Plugin::getInstance().getConfig();

    if (config->gocVehicle != -1) {
        if (samp::RefNetGame()->GetVehiclePool()->Get(config->gocVehicle)) {
            return config->gocVehicle;
        }
        return -1;
    }

    int r = -1;
    auto pool = samp::RefNetGame()->GetVehiclePool();
    if (last_hacking_vehicle >= pool->MAX_VEHICLES)
        last_hacking_vehicle = 0;
    auto& pos = FindPlayerPed(-1)->GetPosition();
    for (auto& i = last_hacking_vehicle; i < pool->MAX_VEHICLES; i++) {
        if (!pool->m_bIsActive[i])
            continue;

        auto v = (CPools::GetVehicle(pool->GetRef(i)));
        if (
            v &&
            v->m_pDriver == nullptr &&
            DistanceBetweenPoints(v->GetPosition2D(), pos) < distance
        ) {
                r = i++;
                break;
            }
    }
    
    return r;
}

int CarUtils::getVehicleFromVector3D(const CVector& vec, float distance, const std::unordered_set<int>* filter) {
    auto pool = samp::RefNetGame()->GetVehiclePool();
    const auto& pos = vec;
    int vehicle = -1;
    float range = distance;
    for (int i = 0; i < pool->MAX_VEHICLES; i++) {
        if (!pool->m_bIsActive[i])
            continue;
        auto v = (CPools::GetVehicle(pool->GetRef(i)));
        float dis;
        if (
            v &&
            (v->m_pDriver == nullptr) &&
            (dis = DistanceBetweenPoints(v->GetPosition2D(), pos)) < range
        ) {
            if (filter && filter->find(i) != filter->end())
                continue;

            range = dis;
            vehicle = i;
        }
    }
    return vehicle;
}

CVehicle * CarUtils::getGTAVehicleFromSAMP(int id) {
    auto pool = samp::RefNetGame()->GetVehiclePool();
    return CPools::GetVehicle(pool->GetRef(id));
}

void CarUtils::safelyBackInCar() {
    auto self = FindPlayerPed(-1);
    if (!self->m_nPedFlags.bInVehicle)
        return;
    auto vehicle = self->m_pVehicle;

    int vehicleid = samp::RefNetGame()->GetVehiclePool()->Find(vehicle);
    if (vehicleid == -1)
        return;

    RakNet::BitStream FkbsRPC;
    UINT16 wVehicleID = vehicleid;
    UINT8 bIsPassenger = false;
    FkbsRPC.Reset();
    FkbsRPC.Write(wVehicleID);
    FkbsRPC.Write(bIsPassenger);
    rakhook::send_rpc(RPC_EnterVehicle, &FkbsRPC, HIGH_PRIORITY, RELIABLE_ORDERED, 0, false);

    enter_tick = GetTickCount();
}

bool CarUtils::areWeSafeToEnter() {
    return GetTickCount() - enter_tick >= 400;
}

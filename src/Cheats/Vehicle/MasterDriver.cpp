//
// Created by Administrator on 2024/10/7.
//

#include "MasterDriver.h"

#include <sampapi/CNetGame.h>

#include <common.h>

#include "CarTrollSync.h"
#include "../../CarUtils.h"

namespace CheatModule::Vehicle {
    MasterDriver::MasterDriver() : CCheatModule("引擎解锁", "Master Driver") {
        tick = 0;
    }

    void MasterDriver::onProcess() {
        if (GetTickCount() - tick >= 1000) {
            auto ped = FindPlayerPed(-1);
            if (ped->m_nPedFlags.bInVehicle) {
                ped->m_pVehicle->m_nVehicleFlags.bEngineBroken = false;
                ped->m_pVehicle->m_nVehicleFlags.bEngineOn = true;
            }
        }
    }
}

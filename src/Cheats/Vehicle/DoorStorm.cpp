#include "DoorStorm.h"

#include <sampapi/CNetGame.h>
#include <sampapi/CPlayerPool.h>

#include <CVehicle.h>
#include <CAutomobile.h>
#include <CPools.h>

#include "../../Utils.h"

namespace CheatModule::Vehicle {

    DoorStorm::DoorStorm() : CCheatModule("疯狂掉车壳", "Door Storm") { tick = 0; }

    void DoorStorm::onProcess() {
        auto ped = samp::RefNetGame()->GetPlayerPool()->GetLocalPlayer()->GetPed();
        if (!ped->IsInVehicle())
            return;

        auto veh = CPools::GetVehicle(ped->GetVehicleRef());

        if (veh) {
            CAutomobile *automobile = reinterpret_cast<CAutomobile *>(veh);
            if (GetTickCount() - tick > 16) {
                auto pos = automobile->GetPosition();
                if (!bDoorStormState) {
                    automobile->m_damageManager.m_anDoorsStatus[0] = 4;
                    automobile->m_damageManager.m_anDoorsStatus[1] = 4;
                    automobile->m_damageManager.m_anDoorsStatus[2] = 4;
                    automobile->m_damageManager.m_anDoorsStatus[3] = 4;
                    automobile->m_damageManager.m_anDoorsStatus[4] = 4;
                    automobile->m_damageManager.m_anDoorsStatus[5] = 4;
                    automobile->m_damageManager.m_anDoorsStatus[6] = 4;
                    veh->Fix();
                    for (int i = 0; i < 4; i++)
                        automobile->m_damageManager.SetDoorStatus(
                                    eDoors(i), 4),
                                automobile->m_damageManager.SetPanelStatus(
                                    ePanels(i), 3);
                    bDoorStormState = true;
                } else {
                    automobile->m_damageManager.m_anDoorsStatus[0] = 0;
                    automobile->m_damageManager.m_anDoorsStatus[1] = 0;
                    automobile->m_damageManager.m_anDoorsStatus[2] = 0;
                    automobile->m_damageManager.m_anDoorsStatus[3] = 0;
                    automobile->m_damageManager.m_anDoorsStatus[4] = 0;
                    automobile->m_damageManager.m_anDoorsStatus[5] = 0;
                    automobile->m_damageManager.m_anDoorsStatus[6] = 0;
                    veh->Fix();
                    for (int i = 0; i < 4; i++)
                        automobile->m_damageManager.SetDoorStatus(
                                    eDoors(i), 4),
                                automobile->m_damageManager.SetPanelStatus(
                                    ePanels(i), 3);
                    bDoorStormState = false;
                }
                tick = GetTickCount();
            }
        }
    }

}

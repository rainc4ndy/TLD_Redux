#include "AllVanish.h"

#include <BitStream.h>
#include <sampapi/CNetGame.h>
#include <sampapi/CPlayerPool.h>

#include <CVehicle.h>
#include <CAutomobile.h>
#include <CPools.h>

#include "../../Utils.h"
#include "../../CarUtils.h"

namespace CheatModule::Vehicle {

    AllVanish::AllVanish() : CCheatModule("所有车消失", "Vehicle Vanishing Exploit") { tick = 0; }

    void AllVanish::onProcess() {
        if (GetTickCount() - tick > 32) {
            auto id = CarUtils::getHackingVehicle(300.0f);
            if (id == -1)
                return;

            auto vehicle = CarUtils::getGTAVehicleFromSAMP(id);
            stUnoccupiedData unSync {};
            unSync.sVehicleID = id;
            unSync.fPosition[0] = vehicle->m_matrix->pos.x;
            unSync.fPosition[1] = vehicle->m_matrix->pos.y;
            unSync.fPosition[2] = vehicle->m_matrix->pos.z;

            unSync.fHealth = vehicle->m_fHealth;

            unSync.fDirection[0] = sqrt((float)-1);
            unSync.fDirection[1] = sqrt((float)-1);
            unSync.fDirection[2] = sqrt((float)-1);

            unSync.fRoll[0] = sqrt((float)-1);
            unSync.fRoll[1] = sqrt((float)-1);
            unSync.fRoll[2] = sqrt((float)-1);

            unSync.byteSeatID = 64;

            RakNet::BitStream bsUnoccupiedSync;
            bsUnoccupiedSync.Reset();
            bsUnoccupiedSync.Write((BYTE)ID_UNOCCUPIED_SYNC);
            bsUnoccupiedSync.Write((PCHAR)&unSync, sizeof(stUnoccupiedData));
            rakhook::send(&bsUnoccupiedSync, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0);

            vehicle->m_matrix->pos.x = vehicle->m_matrix->pos.y = vehicle->m_matrix->pos.z = 65535.0f;
            //move it out of stream area, cuz we don't want it go back to normal when we disable the cheat
            tick = GetTickCount();
        }
    }
}

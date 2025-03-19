
#include "CarStorm.h"

#include <common.h>

#include <CVector.h>
#include "../../CarUtils.h"
#include "../../Plugin.h"
#include "../../CConfig.h"

#include "CarTrollSync.h"
#include "../../Utils.h"

namespace CheatModule::Vehicle {
    CarStorm::CarStorm() : GocModule("车辆风暴", "Car Storm") {
        fStormAngle = 0.f;
        speed = 8.0f;
    }
    void CarStorm::onProcess() {
        if (ok_with_control()) {
            auto config = Plugin::getInstance().getConfig();
            auto id = CarUtils::getHackingVehicle(config->gocRange);
            if (id != -1) {

                CarTrollSync sync(id, config->gocMode, config->gocFix);

                auto ped = FindPlayerPed(-1);
                auto pos =  ped->m_matrix->pos;

                sync.setPosition(CUtils::GetOffsetedPosition(pos, CVector(1.0f, 10.0f, 0.0f), fStormAngle));
                sync.setVelocity(CVector(speed * cos(fStormAngle),speed * sin(fStormAngle), 0.0f));

                fStormAngle += DEGTORAD(115);
                sync.execute();
                sync.executeLocal();

                update_tick();
            }
        }
    }
}

#include "Elevator.h"


#include <CVector.h>
#include "../../CarUtils.h"
#include "../../Plugin.h"
#include "../../CConfig.h"
#include "../../TargetSelector.h"

#include "../Vehicle/CarTrollSync.h"

namespace CheatModule::Target {
    Elevator::Elevator() : GocModule("车辆升天R", "Target Fuck Up") { }
    void Elevator::onProcess() {
        using namespace Vehicle;
        if (ok_with_control()) {
            auto config = Plugin::getInstance().getConfig();
            auto target = Plugin::getInstance().getTarget();
            auto id = CarUtils::getHackingVehicle(config->gocRange);
            if (id != -1 && target->targetPlayerPed) {
                CarTrollSync sync(id, config->gocMode, config->gocFix);

                sync.setPosition(target->targetPlayerPed->m_matrix->pos);
                sync.setVelocity(CVector(0, 0, speed));
                
                sync.execute();
                sync.executeLocal();
                update_tick();
            }
        }
    }
}

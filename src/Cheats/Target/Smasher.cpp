//
// Created by Administrator on 2024/10/7.
//

#include "Smasher.h"

#include <CVehicle.h>

#include <CVector.h>
#include "../../CarUtils.h"
#include "../../Plugin.h"
#include "../../CConfig.h"
#include "../../TargetSelector.h"

#include "../Vehicle/CarTrollSync.h"

namespace CheatModule::Target {
    Smasher::Smasher() : GocModule("车辆砸地R", "Target Fuck Down") { }
    void Smasher::onProcess() {
        using namespace Vehicle;
        if (ok_with_control()) {
            auto config = Plugin::getInstance().getConfig();
            auto target = Plugin::getInstance().getTarget();
            auto id = CarUtils::getHackingVehicle(config->gocRange);
            if (id != -1 && target->targetPlayerPed) {
                CarTrollSync sync(id, config->gocMode, config->gocFix);

                sync.setPosition(target->targetPlayerPed->m_matrix->pos + CVector(0, 0, 5.0f + (float)(rand() % 10)));
                sync.setVelocity(CVector(0, 0, -speed));


                sync.execute();
                sync.executeLocal();
                update_tick();
            }
        }
    }
}

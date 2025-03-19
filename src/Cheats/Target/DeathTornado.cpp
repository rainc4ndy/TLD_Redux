//
// Created by Administrator on 2024/10/7.
//

#include "DeathTornado.h"

#include <CVector.h>
#include "../../CarUtils.h"
#include "../../Plugin.h"
#include "../../CConfig.h"
#include "../../TargetSelector.h"

#include "../Vehicle/CarTrollSync.h"

namespace CheatModule::Target {
    DeathTornado::DeathTornado() : GocModule("死亡龙卷风", "Tornado of Death") { }
    void DeathTornado::onProcess() {
        using namespace Vehicle;
        if (ok_with_control()) {
            auto config = Plugin::getInstance().getConfig();
            auto target = Plugin::getInstance().getTarget();
            auto id = CarUtils::getHackingVehicle(config->gocRange);
            if (id != -1 && target->targetPlayerPed) {
                CarTrollSync sync(id, CarTrollSync::SYNC_UNOCCUPIED, config->gocFix);

                sync.setPosition(target->targetPlayerPed->m_matrix->pos);
                sync.setTurnSpeed(CVector(0, 0, speed));
                sync.setVelocity(CVector(0, 0, 0));

                sync.execute();
                sync.executeLocal();
                update_tick();
            }
        }
    }
}

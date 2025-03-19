//
// Created by Administrator on 2024/10/6.
//

#include "CarLagger.h"

#include <CVehicle.h>
#include <common.h>

#include <CVector.h>
#include "../../CarUtils.h"
#include "../../Plugin.h"
#include "../../CConfig.h"
#include "../../Utils.h"

#include "CarTrollSync.h"

namespace CheatModule::Vehicle {
    CarLagger::CarLagger() : GocModule("集中拉车", "Car Lagger") { }
    void CarLagger::onProcess() {
        if (ok_with_control()) {
            auto config = Plugin::getInstance().getConfig();
            auto id = CarUtils::getHackingVehicle(config->gocRange);
            if (id != -1) {

                CarTrollSync sync(id, config->gocMode, config->gocFix);

                auto ped = FindPlayerPed(-1);
                auto pos = CUtils::GetOffsetedPosition(ped->m_matrix->pos, CVector(0.0f, 4.0f, 0.0f), ped->GetHeading());

                if (DistanceBetweenPoints(pos, old_pos) > 20.f) {
                    old_pos = pos;
                    sync.setPosition(pos);
                } else {
                    sync.setPosition(old_pos);
                }

                sync.setVelocity(CVector(0, 0, 0));
                sync.execute();
                sync.executeLocal();

                update_tick();
            }
        }
    }
}

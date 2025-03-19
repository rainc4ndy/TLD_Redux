#include "CarShooter.h"

#include <CVehicle.h>
#include <common.h>

#include <CVector.h>
#include "../../CarUtils.h"
#include "../../Plugin.h"
#include "../../CConfig.h"

#include "CarTrollSync.h"
#include "../../CLog.h"
#include "../../Utils.h"

namespace CheatModule::Vehicle {

    CarShooter::CarShooter() : GocModule("射车", "Car Shoot") {
        tick = 0;
        speed = 8.0f;
    }
    void CarShooter::onProcess() {
        if (ok_with_control()) {
            auto config = Plugin::getInstance().getConfig();
            auto id = CarUtils::getHackingVehicle(config->gocRange);

            if (id != -1) {
                const static float *cam_matrix = reinterpret_cast<float *>(0xB6F99C);
                float fRandomDistance = 6.5f + (rand() % 1000) / 100;

                CarTrollSync sync(id, config->gocMode, config->gocFix);

                auto ped = FindPlayerPed(-1);
                auto pos =  ped->m_matrix->pos;
                pos += CVector(cam_matrix[4] * fRandomDistance, cam_matrix[5] * fRandomDistance, cam_matrix[6] * fRandomDistance);

                sync.setPosition(pos);
                sync.setVelocity(CVector(cam_matrix[4] * speed, cam_matrix[5] * speed, cam_matrix[6] * speed));

                sync.execute();
                sync.executeLocal();

                update_tick();
            }
        }
    }
}

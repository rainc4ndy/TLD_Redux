
#include "CarWall.h"

#include <common.h>
#include <CVector.h>
#include "../../CarUtils.h"
#include "../../Plugin.h"
#include "../../CConfig.h"

#include "CarTrollSync.h"
#include "../../Utils.h"

namespace CheatModule::Vehicle {
    CarWall::CarWall() : GocModule("车辆之环", "Car Ring") {
        current_point = 0;
        speed = 3.0;
        delay = 20;
    }
    void CarWall::onProcess() {
        if (ok_with_control()) {
            auto config = Plugin::getInstance().getConfig();
            auto id = CarUtils::getHackingVehicle(config->gocRange);
            if (id != -1) {
                CarTrollSync sync(id, config->gocMode, config->gocFix);
                auto ped = FindPlayerPed(-1);
                float radius = 6.0f; // Distance from the player
                int total_points = 8; // Total points in the circle

                // Calculate the angle for the current point
                float angle = (2.0f * M_PI / total_points) * current_point;
                CVector pos(
                    ped->m_matrix->pos.x + radius * cos(angle),
                    ped->m_matrix->pos.y + radius * sin(angle),
                    ped->m_matrix->pos.z // Keep the same Z position
                );

                // Move to the next point for the next call
                current_point = (current_point + 1) % total_points;

                sync.setPosition(pos);
                sync.setVelocity(CVector(0, 0, speed));
                sync.execute();
                sync.executeLocal();
                update_tick();
            }
        }
    }
}

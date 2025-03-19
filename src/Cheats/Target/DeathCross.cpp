#include "DeathCross.h"

#include <CVector.h>
#include "../../CarUtils.h"
#include "../../Plugin.h"
#include "../../CConfig.h"
#include "../../TargetSelector.h"

#include "../Vehicle/CarTrollSync.h"

namespace CheatModule::Target {
    DeathCross::DeathCross() : GocModule("死亡十字R", "Cross of Death") {
        m_iState_DeadCross = 0;
    }

    void DeathCross::onProcess() {
        using namespace Vehicle;
        if (ok_with_control()) {
            auto config = Plugin::getInstance().getConfig();
            auto target = Plugin::getInstance().getTarget();
            auto id = CarUtils::getHackingVehicle(config->gocRange);
            if (id != -1 && target->targetPlayerPed) {
                CarTrollSync sync(id, config->gocMode, config->gocFix);

                switch (m_iState_DeadCross) {
                    case 0:
                        sync.setPosition(target->targetPlayerPed->GetPosition() + CVector(0, 4.0f, 0));
                    sync.setVelocity(CVector(0.0f, -speed, 0.0f));
                    break;

                    case 1:
                        sync.setPosition(target->targetPlayerPed->GetPosition() + CVector(4.0f, 0, 0));
                    sync.setVelocity(CVector(-speed, 0.0f, 0.0f));
                    break;

                    case 2:
                        sync.setPosition(target->targetPlayerPed->GetPosition() + CVector(0, -4.0f, 0));
                    sync.setVelocity(CVector(0.0f, speed, 0.0f));
                    break;

                    case 3:
                        sync.setPosition(target->targetPlayerPed->GetPosition() + CVector(-4.0f, 0, 0));
                    sync.setVelocity(CVector(speed, 0.0f, 0.0f));
                    break;
                }

                m_iState_DeadCross++;
                if (m_iState_DeadCross > 3) {
                    m_iState_DeadCross = 0;
                }

                sync.execute();
                sync.executeLocal();
                update_tick();
            }
        }
    }
}

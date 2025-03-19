//
// Created by Administrator on 2024/10/12.
//

#include "CBotFollow.h"
#include "CBotManager.h"
#include "CBot.h"
#include "../../Utils.h"

#include <CPed.h>
#include "Plugin.h"
#include <common.h>

#include "CarUtils.h"

namespace CheatModule {
    namespace Bot {
        CBotFollow::CBotFollow() : CCheatModule("Bot跟随", "Bot Follow") {
        }

        void CBotFollow::adjustPosition(float* position, float heading, int id, int total) {
            // Apply different strategies (alignment, circular formation, etc.)
            switch (followStrategy) {
                case FS_ALIGN: {
                    id++;
                    float offset = (id % 2 == 0) ? -1.0f : 1.0f; // left for even, right for odd
                    position[0] += (offset * cos(heading) * follow_range * (float)id);
                    position[1] += (offset * sin(heading) * follow_range * (float)id);
                    break;
                }
                case FS_ROUND: {
                    float angle = (float)id * (360.0f / total) * (M_PI / 180.0f); // Convert to radians
                    position[0] += cos(angle) * follow_range; // Adjust distance from player
                    position[1] += sin(angle) * follow_range;
                    break;
                }
            }
        }

        bool CBotFollow::onProcess(stOnFootData *data) {
            auto manager = Plugin::getInstance().getBotManager();
            auto self = samp::RefNetGame()->GetPlayerPool()->GetLocalPlayer();
            auto bots = manager->getAllBot();

            int weaponid = 0;
            if (manager->use_weapon) {
                if (manager->bot_weapon == 0) weaponid = self->GetPed()->GetCurrentWeapon();
                else
                    weaponid = manager->bot_weapon;
            }

            int numBots = bots.size();
            for (int i = 0; i < numBots; ++i) {
                if (bots[i]->getStatus() != CBot::SPAWNED)
                    continue;

                float position[] = {
                        self->m_onfootData.m_position.x,
                        self->m_onfootData.m_position.y,
                        self->m_onfootData.m_position.z
                };
                adjustPosition(position, self->GetPed()->m_pGamePed->GetHeading(), i, numBots);

                int bot_car = bots[i]->getVehicleID();
                if (bot_car != -1) {
                    bots[i]->clearVehicle();
                }

                stOnFootData *onfoot = bots[i]->getData();
                // Copy player's quaternion (orientation)
                memcpy(onfoot->fQuaternion, self->m_onfootData.m_fQuaternion, sizeof(onfoot->fQuaternion));
                // Copy other important data (keys, special actions)
                onfoot->sKeys = self->m_onfootData.m_controllerState.m_value;
                onfoot->byteSpecialAction = self->m_onfootData.m_nSpecialAction;
                onfoot->sCurrentAnimationID = self->m_animation.m_nId;
                onfoot->sAnimFlags = self->m_animation.m_value >> 16;
                onfoot->sLeftRightKeys = self->m_onfootData.m_controllerState.m_sLeftStickX;
                onfoot->sUpDownKeys = self->m_onfootData.m_controllerState.m_sLeftStickY;
                onfoot->fMoveSpeed[0] = self->m_onfootData.m_speed.x;
                onfoot->fMoveSpeed[1] = self->m_onfootData.m_speed.y;
                onfoot->fMoveSpeed[2] = self->m_onfootData.m_speed.z;
                onfoot->fPosition[0] = position[0];
                onfoot->fPosition[1] = position[1];
                onfoot->fPosition[2] = position[2];
                onfoot->byteCurrentWeapon = weaponid;
                bots[i]->sendOnfootSync(onfoot);
            }
            return true;
        }

        bool CBotFollow::onProcess(stInCarData *data)  {
            auto manager = Plugin::getInstance().getBotManager();
            auto self = samp::RefNetGame()->GetPlayerPool()->GetLocalPlayer();
            auto bots = manager->getAllBot();

            int weaponid = 0;
            if (manager->use_weapon) {
                if (manager->bot_weapon == 0) weaponid = self->GetPed()->GetCurrentWeapon();
                else
                    weaponid = manager->bot_weapon;
            }
            auto used_vehicle = CBot::getBotVehicleSet();
            int numBots = bots.size();
            for (int i = 0; i < numBots; ++i) {
                if (bots[i]->getStatus() != CBot::SPAWNED)
                    continue;
                float position[] = {
                    self->m_incarData.m_position.x,
                    self->m_incarData.m_position.y,
                    self->m_incarData.m_position.z
                };
                adjustPosition(position, self->GetPed()->m_pGamePed->GetHeading(), i, numBots);

                stOnFootData *onfoot = bots[i]->getData();
                onfoot->fPosition[0] = position[0];
                onfoot->fPosition[1] = position[1];
                onfoot->fPosition[2] = position[2];
                if (drive_with_me) {
                    int bot_car = bots[i]->getVehicleID();
                    if (bot_car != -1) {
                        // 检查车辆有效性
                        auto gta_veh = CarUtils::getGTAVehicleFromSAMP(bot_car);
                        if (gta_veh == nullptr) {
                            // 跳过本次车辆数据包，下次再寻找另外的车
                            bots[i]->clearVehicle();
                            goto onfoot_mode;
                        } else {
                            // 沿用上一次的车
                            stInCarData incar = *data;
                            incar.fVehicleHealth = 1000.0f;
                            incar.byteCurrentWeapon = weaponid;
                            incar.sVehicleID = bot_car;
                            incar.fPosition[0] = position[0];
                            incar.fPosition[1] = position[1];
                            incar.fPosition[2] = position[2];
                            bots[i]->sendInCar(&incar);
                            continue;
                        }
                    }
                    // 寻找新车辆
                    int vehicleid = CarUtils::getVehicleFromVector3D({position[0], position[1], position[2]}, 300.f, used_vehicle);
                    if (vehicleid != -1) {
                        stInCarData incar = *data;
                        incar.fVehicleHealth = 1000.0f;
                        incar.byteCurrentWeapon = weaponid;
                        incar.sVehicleID = vehicleid;
                        incar.fPosition[0] = position[0];
                        incar.fPosition[1] = position[1];
                        incar.fPosition[2] = position[2];
                        bots[i]->setVehicleID(vehicleid);
                        bots[i]->sendEnterVehicle(vehicleid);
                        bots[i]->sendInCar(&incar);
                        continue;
                    }
                }
onfoot_mode:
                memcpy(onfoot->fQuaternion, self->m_incarData.m_fQuaternion, sizeof(onfoot->fQuaternion));
                onfoot->fMoveSpeed[0] = self->m_incarData.m_speed.x;
                onfoot->fMoveSpeed[1] = self->m_incarData.m_speed.y;
                onfoot->fMoveSpeed[2] = self->m_onfootData.m_speed.z;
                onfoot->byteCurrentWeapon = weaponid;
                bots[i]->sendOnfootSync(onfoot);
            }
            return true;
        }

        bool CBotFollow::onProcess(stBulletData *data) {
            if (shoot_with_me) {
                auto manager = Plugin::getInstance().getBotManager();
                auto bots = manager->getAllBot();
                for (auto it : bots) {
                    stBulletData bot_bullet = *data;
                    auto pos =  it->getPosition();
                    bot_bullet.fOrigin[0] = pos.x,
                    bot_bullet.fOrigin[1] = pos.y,
                    bot_bullet.fOrigin[2] = pos.z;
                    it->sendBullet(&bot_bullet);
                }
            }
            return true;
        }
        bool CBotFollow::onProcess(bool fromServer, int rpc, RakNet::BitStream* bs) {
            if (!fromServer) {
                auto manager = Plugin::getInstance().getBotManager();
                auto bots = manager->getAllBot();
                if (all_rpc_follow) {
                    for (auto it : bots) {
                        it->sendRPC(rpc, bs);
                    }
                    return true;
                }

                if (!chat_with_me)
                    return true;

                if (rpc == RPC_ServerCommand) {
                    UINT32 len;
                    bs->ResetReadPointer();
                    bs->Read(len);
                    std::string text(len, 0);
                    bs->Read(text.data(), len);

                    for (auto it : bots) {
                        it->sendChat(text);
                    }
                } else if (rpc == RPC_Chat) {
                    UINT8 len;
                    bs->ResetReadPointer();
                    bs->Read(len);
                    std::string text(len, 0);
                    bs->Read(text.data(), len);

                    for (auto it : bots) {
                        it->sendChat(text);
                    }
                }
            }
            return true;
        }

        bool CBotFollow::onProcess(stAimData *data) {
            auto manager = Plugin::getInstance().getBotManager();
            auto bots = manager->getAllBot();
            for (auto it : bots) {
                it->sendAim(data);
            }
            return true;
        }

        void CBotFollow::onSave(nlohmann::json &j) {
            saveData(j, "follow_strategy", followStrategy);
            saveData(j, "follow_range", follow_range);
            saveData(j, "shoot_with_me", shoot_with_me);
            saveData(j, "drive_with_me", drive_with_me);
            saveData(j, "aim_with_me", aim_with_me);
            saveData(j, "chat_with_me", chat_with_me);
        }

        void CBotFollow::onLoad(const nlohmann::json &j) {
            loadData(j, "follow_strategy", followStrategy);
            loadData(j, "follow_range", follow_range);
            loadData(j, "shoot_with_me", shoot_with_me);
            loadData(j, "drive_with_me", drive_with_me);
            loadData(j, "aim_with_me", aim_with_me);
            loadData(j, "chat_with_me", chat_with_me);
        }

    } // Bot
} // CheatModule

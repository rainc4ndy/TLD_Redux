//
// Created by Administrator on 2024/10/7.
//

#include "DarkStab.h"

#include <BitStream.h>
#include <sampapi/CNetGame.h>
#include <sampapi/CPlayerPool.h>

#include <common.h>

#include "../../Plugin.h"
#include "../../TargetSelector.h"
#include "../../Utils.h"
#include "asmjit/arm/armoperand.h"

namespace CheatModule {
    namespace Target {
        bool DarkStab::can_be_darkstab(CPed *ped) {
            if (!ped)
                return false;

            //target在车里，过滤掉
            if (ped->m_pVehicle) {
                state = TARGET_ASSASSIN_NO;
                return false;
            }

            //target移动太快，过滤掉
            if (fabs(ped->m_vecMoveSpeed.x) > 0.01f ||
                fabs(ped->m_vecMoveSpeed.y) > 0.01f ||
                fabs(ped->m_vecMoveSpeed.z) > 0.01f) {
                state = TARGET_ASSASSIN_NO;
                return false;
                }

            //target不在surface，过滤掉
            if (ped->m_nPedFlags.bIsStanding == false) {
                state = TARGET_ASSASSIN_NO;
                return false;
            }

            auto self = FindPlayerPed(-1);
            if (use_own_weapon) {
                if(self->m_aWeapons[self->m_nActiveWeaponSlot].m_eWeaponType != WEAPON_KNIFE) {
                    return false;
                }
            }
            //target超出有效范围，过滤掉
            if (DistanceBetweenPoints(self->GetPosition(), ped->GetPosition()) > range) {
                state = TARGET_ASSASSIN_NO;
                return false;
            }

            return true;
        }

        DarkStab::DarkStab(): CCheatModule("暗杀方法", "Dark Stab") {
            state = 0;
            tick = 0;
        }

        void DarkStab::onProcess() {
            auto target = Plugin::getInstance().getTarget();
            if (GetTickCount() - tick > 400) {
                if (target->targetPlayerPed == nullptr)
                    return;

                if (can_be_darkstab(target->targetPlayerPed) == false)
                    return;

                auto self = FindPlayerPed(-1);

                stOnFootData data {};
                RakNet::BitStream bitStream;

                //初始化 (位置，武器，血量)
                memset(&data, 0, sizeof(stOnFootData));

                data.byteHealth = static_cast<BYTE>(self->m_fHealth);
                data.byteArmor = static_cast<BYTE>(self->m_fArmour);
                data.byteCurrentWeapon = 4;
                float offset[3] = {
                    0.0f, -0.4f, 0.0f
                };
                CVector pos = CUtils::GetOffsetedPosition(target->targetPlayerPed->m_matrix->pos,
                                                          CVector(0.0f, -0.4f, 0.0f),
                                                          target->targetPlayerPed->GetHeading());
                data.fPosition[0] = pos.x;
                data.fPosition[1] = pos.y;
                data.fPosition[2] = pos.z;

                auto remote_player = samp::RefNetGame()->GetPlayerPool()->GetPlayer(target->targetPlayerID);

                data.fQuaternion[0] = remote_player->m_onfootData.m_fQuaternion[0];
                data.fQuaternion[1] = remote_player->m_onfootData.m_fQuaternion[1];
                data.fQuaternion[2] = remote_player->m_onfootData.m_fQuaternion[2];
                data.fQuaternion[3] = remote_player->m_onfootData.m_fQuaternion[3];

                data.fMoveSpeed[2] = -0.01f; //强制同步

                switch (state) {
                    case TARGET_ASSASSIN_NO: {
                        //先传送到对方后面
                        data.sAnimFlags = 0;
                        data.sCurrentAnimationID = 0;
                        bitStream.Reset();
                        bitStream.Write((BYTE) ID_PLAYER_SYNC);
                        bitStream.Write((PCHAR) &data, sizeof(stOnFootData));
                        rakhook::send(&bitStream, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0);
                        state = TARGET_ASSASSIN_TELEPORTED;
                        break;
                    }
                    case TARGET_ASSASSIN_TELEPORTED: {
                        //已传送，现在按下瞄准
                        //发送legitmate的瞄准数据包
                        stAimData dataAim {};

                        dataAim.byteCamExtZoom = 63;
                        dataAim.byteCamMode = 4;

                        //把摄像机的位置转到目标头部
                        memcpy(&dataAim.vecAimPos, &data.fPosition, sizeof(float) * 3);
                        dataAim.vecAimPos[2] += 0.66f;
                        CVector vecPoint = target->targetPlayerPed->GetPosition();
                        vecPoint.z += 0.66f;

                        //注:"vecAimf1" 指 "vecFront"
                        // Get the aiming distance
                        CVector vecDistance = vecPoint - CVector(dataAim.vecAimPos[0], dataAim.vecAimPos[1], dataAim.vecAimPos[2]);
                        // Get the distance to the destination point
                        float fDistance = DistanceBetweenPoints(CVector(dataAim.vecAimPos[0], dataAim.vecAimPos[1], dataAim.vecAimPos[2]), vecPoint);
                        // Get the destination angle
                        vecDistance /= fDistance;

                        dataAim.vecAimf1[0] = vecDistance.x;
                        dataAim.vecAimf1[1] = vecDistance.y;
                        dataAim.vecAimf1[2] = vecDistance.z;

                        bitStream.Reset();
                        bitStream.Write((BYTE) ID_AIM_SYNC);
                        bitStream.Write((PCHAR) &dataAim, sizeof(stAimData));
                        rakhook::send(&bitStream, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0);

                        //鼠标瞄准
                        data.sKeys = 128;
                        data.stSampKeys.keys_aim = 1;
                        bitStream.Reset();
                        bitStream.Write((BYTE) ID_PLAYER_SYNC);
                        bitStream.Write((PCHAR) &data, sizeof(stOnFootData));
                        rakhook::send(&bitStream, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0);
                        state = TARGET_ASSASSIN_AIMED;
                        break;
                    }
                    case TARGET_ASSASSIN_AIMED: {
                        //已瞄准，现在攻击
                        //鼠标双键并按
                        data.sKeys = 132;
                        bitStream.Reset();
                        bitStream.Write((BYTE) ID_PLAYER_SYNC);
                        bitStream.Write((PCHAR) &data, sizeof(stOnFootData));
                        rakhook::send(&bitStream, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0);
                        state = TARGET_ASSASSIN_ATTACKED;
                        break;
                    }

                    case TARGET_ASSASSIN_ATTACKED: {
                        //已攻击，现在恢复
                        data.sKeys = 0;
                        data.stSampKeys.keys_aim = 0;
                        data.sAnimFlags = -32764;
                        data.sCurrentAnimationID = 1189;
                        bitStream.Reset();
                        bitStream.Write((BYTE) ID_PLAYER_SYNC);
                        bitStream.Write((PCHAR) &data, sizeof(stOnFootData));
                        rakhook::send(&bitStream, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0);
                        state = TARGET_ASSASSIN_NO;
                        break;
                    }
                }

                tick = GetTickCount();
            }
        }

        bool DarkStab::onProcess(stOnFootData *data) {
            auto target = Plugin::getInstance().getTarget();
            if (target->targetPlayerPed == nullptr)
                return true;

            return !can_be_darkstab(target->targetPlayerPed);
        }

        void DarkStab::onSave(nlohmann::json &j) {
            saveData(j, "use_own_weapon", use_own_weapon);
            saveData(j, "range", range);
        }

        void DarkStab::onLoad(const nlohmann::json &j) {
            loadData(j, "use_own_weapon", use_own_weapon);
            loadData(j, "range", range);
        }

        void DarkStab::onEnable() {
            state = TARGET_ASSASSIN_NO;
        }

        void DarkStab::onDisable() {
            state = TARGET_ASSASSIN_NO;
        }
    } // Target
} // CheatModule

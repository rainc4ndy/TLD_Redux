//
// Created by Administrator on 2024/10/7.
//

#include "ZombieAutomata.h"

#include <sampapi/CNetGame.h>
#include <common.h>
#include "../../weapon_config.h"
#include "../../Utils.h"

#include "matrix.h"
#include <glm/gtc/matrix_transform.hpp> // for rotate
#include <glm/gtc/type_ptr.hpp> // for value_ptr

#include "encoding.h"

namespace CheatModule::Hack {
    void ZombieAutomata::setCurrentStashPosition() {
        auto ped = FindPlayerPed(-1);
        stash_position.x = ped->m_matrix->pos.x;
        stash_position.y = ped->m_matrix->pos.y;
        stash_position.z = ped->m_matrix->pos.z;
    }

    ZombieAutomata::ZombieAutomata() : CCheatModule("自动僵尸", "Zombie Automata"){
        tick = 0;
        stash_state = SS_IDLE;
    }

    void ZombieAutomata::sync_all_money() {
        static const auto center = glm::vec3(-391.5811,2204.5793,49.2436);
        auto pool = samp::RefNetGame()->GetPickupPool();
        for (int i = 0; i < pool->MAX_PICKUPS;i++) {
            if (pool->m_handle[i]) {
                if ( pool->m_object[i].m_nModel == 1212) {
                    if (glm::distance(center, glm::vec3(
                                          pool->m_object[i].m_position.x,
                                          pool->m_object[i].m_position.y,
                                          pool->m_object[i].m_position.z)) < 300.0f) {

                        RakNet::BitStream bs;
                        bs.Reset();
                        INT32 pid = i;
                        bs.Write(pid);
                        rakhook::send_rpc(RPC_PickedUpPickup, &bs, HIGH_PRIORITY,
                                          RELIABLE_ORDERED, 0, false);
                        /*CUtils::chatprintf("Collect cash on: %0.2f, %0.2f, %0.2f (id:%d)", pool->m_object[i].m_position.x,
                        pool->m_object[i].m_position.y,
                        pool->m_object[i].m_position.z, i);*/
                    }
                }
            }
        }
    }

    void ZombieAutomata::furniture_process() {
        auto self = FindPlayerPed(-1);
        RakNet::BitStream bs;
        bs.Reset();
        stOnFootData data {};
        data.byteHealth = static_cast<BYTE>(self->m_fHealth);
        data.byteArmor = static_cast<BYTE>(self->m_fArmour);
        data.fPosition[0] = stash_position.x;
        data.fPosition[1] = stash_position.y;
        data.fPosition[2] = stash_position.z;
        data.stSampKeys.keys_aim = 1;
    }

    bool ZombieAutomata::is_skin_satisfied(int skin) {
        if (pattern == ZP_ZOMBIE_ONLY) {
            return skin == 162;
        } else if (pattern == ZP_BOSS_ONLY) {
            return skin == 22;
        }
        return true;
    }

    bool ZombieAutomata::onProcess(stOnFootData *data) {
        return false;
    }

    bool ZombieAutomata::onProcess(stAimData *data) {
        return false;
    }

    void ZombieAutomata::onProcess() {
        auto self = FindPlayerPed(-1);

        int weaponid = self->m_aWeapons[self->m_nActiveWeaponSlot].m_eWeaponType;
        unsigned int rate = static_cast<unsigned int>(WeaponConfig::GetMaxWeaponShootRate(weaponid) * rate_factor);

        // 玩家同步位置
        if (GetTickCount() - tick_onfoot >= 40) {
            if (stash_state == SS_WAIT_FOR_OBJECT) {
                // 藏家具
                furniture_process();
                tick_onfoot = GetTickCount();
                return;
            }
            CPed *target = nullptr;
            int target_id = -1;
            auto pool = samp::RefNetGame()->GetPlayerPool();
            float distance = 300.0f;
            // 选择一个最近的NPC目标
            for (int i = 0; i < samp::CPlayerPool::MAX_PLAYERS; i++) {
                if (!pool->m_bNotEmpty[i] || !pool->IsNPC(i))
                    continue;
                auto ped = pool->GetPlayer(i)->m_pPed;
                if (!ped)
                    continue;
                if (ped->IsDead())
                    continue;

                if (!is_skin_satisfied(ped->m_pGamePed->m_nModelIndex))
                    continue;

                float d = DistanceBetweenPoints(pool->GetLocalPlayer()->m_pPed->m_pGamePed->GetPosition(),
                                                ped->m_pGamePed->GetPosition());
                if (d < distance) {
                    distance = d;
                    target = pool->GetPlayer(i)->m_pPed->m_pGamePed;
                    target_id = i;
                }
            }

            //先初始化，不管有没有找到目标都要进行同步
            RakNet::BitStream bs;
            bs.Reset();
            stOnFootData data {};
            data.byteHealth = static_cast<BYTE>(self->m_fHealth);
            data.byteArmor = static_cast<BYTE>(self->m_fArmour);
            data.byteCurrentWeapon = weaponid;

            if (use_invis) {
                /*auto obj_pool = samp::RefNetGame()->GetObjectPool();
                auto obj = obj_pool->Get(obj_pool->m_nLargestId-1);
                if (obj) {
                    data.sSurfingVehicleID = 2000 + obj_pool->m_nLargestId-1;
                    data.fSurfingOffsets[0] = 1000.f;
                }*/
                data.fSurfingOffsets[2] = 1.0f;
                data.sSurfingVehicleID = 2002;
            }

            if (target == nullptr) {
                // 无效的目标
                target_cache = -1;
                data.fPosition[0] = self->m_matrix->pos.x;
                data.fPosition[1] = self->m_matrix->pos.y;
                data.fPosition[2] = self->m_matrix->pos.z;
                bs.Write((BYTE) ID_PLAYER_SYNC);
                bs.Write((PCHAR) &data, sizeof(stOnFootData));
                rakhook::send(&bs, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0);
            } else {
                target_cache = target_id;
                float dis = WeaponConfig::GetWeaponRange(weaponid);
                // 如果距离非常远，那么不必使用那么远的距离
                // 位于区域外进行攻击可能会无效
                if (dis > 20.f)
                    dis = 20.f;

                // 一直在左边
                auto pos = CUtils::GetOffsetedPosition(target->m_matrix->pos, CVector(dis, 0, 0.0f), target->GetHeading());
                data.fPosition[0] = pos.x;
                data.fPosition[1] = pos.y;
                data.fPosition[2] = pos.z;

                // 顺时针旋转90，使得对准目标
                const auto mat = glm::rotate(ConvertGtaMatrixToMat(target->m_matrix), glm::radians(90.0f), glm::vec3(0, 0, 1));
                const auto q = glm::normalize(glm::conjugate(glm::quat(mat)));
                data.fQuaternion[0] = q.w;
                data.fQuaternion[1] = q.x;
                data.fQuaternion[2] = q.y;
                data.fQuaternion[3] = q.z;

                data.fMoveSpeed[0] = CUtils::randomFloat(-0.03f, 0.03f);
                data.fMoveSpeed[1] = CUtils::randomFloat(-0.03f, 0.03f);
                data.sKeys = 132;
                data.stSampKeys.keys_aim = 1;
                bs.Write((BYTE) ID_PLAYER_SYNC);
                bs.Write((PCHAR) &data, sizeof(stOnFootData));
                rakhook::send(&bs, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0);
            }
            tick_onfoot = GetTickCount();
        }

        if (GetTickCount() - tick >= rate) {
            if (target_cache == -1)
                return;

            RakNet::BitStream bs;
            bs.Reset();
            bool bGiveOrTake = false;
            UINT16 wPlayerID = target_cache;
            float damage_amount = WeaponConfig::GetWeaponDamage(override_damage ? WEAPON_DESERT_EAGLE : weaponid);

            UINT32 dWeaponID = weaponid;
            UINT32 dBodypart = 0;
            bs.SetWriteOffset(0);
            bs.Write(bGiveOrTake);
            bs.Write(wPlayerID);
            bs.Write(damage_amount);
            bs.Write(dWeaponID);
            bs.Write(dBodypart);

            rakhook::send_rpc(RPC_GiveTakeDamage, &bs, HIGH_PRIORITY,
                              RELIABLE_ORDERED, 0, false);

            tick = GetTickCount();
        }

        if (money_hack) {
            if (GetTickCount() - pickup_tick >= 250) {
                sync_all_money();
                pickup_tick = GetTickCount();
            }
        }
    }

    bool ZombieAutomata::onProcess(bool fromServer, int rpc, RakNet::BitStream *bs) {
        if (rpc == RPC_ShowDialog) {
            if (!furniture_stash)
                return true;

            bs->ResetReadPointer();
            INT16 wDialogID;
            UINT8 bDialogStyle, bTitleLength;
            bs->Read(wDialogID);
            bs->Read(bDialogStyle);
            bs->Read(bTitleLength);
            if (bDialogStyle == DIALOG_STYLE_MSGBOX) {
                auto title_converted = EncodingUtil::readAndConvertUTF8(bs, bTitleLength);

                if (stash_state == SS_IDLE) {
                    if (std::strstr(title_converted.c_str(), "通知")) {
                        stash_state = SS_WAIT_FOR_OBJECT;
                        CUtils::chatprintf("A furniture was detected, stashing it");
                    }
                } else if (stash_state == SS_WAIT_FOR_OBJECT) {
                    if (std::strstr(title_converted.c_str(), "家具控制")) {
                        RakNet::BitStream response;
                        response.Write(wDialogID);
                        response.Write(static_cast<UINT8>(0));
                        response.Write(static_cast<INT16>(-1));
                        response.Write(static_cast<UINT8>(0));

                        rakhook::send_rpc(RPC_DialogResponse, &response, HIGH_PRIORITY,
                                          RELIABLE_ORDERED, 0, false);
                        CUtils::chatprintf("Sending object placement request");
                        return false;
                    }
                }
            }
        }
        else if (rpc == RPC_ClientMessage) {
            if (furniture_stash && stash_state == SS_WAIT_FOR_OBJECT) {
                bs->ResetReadPointer();
                bs->IgnoreBits(sizeof(UINT32) * 8);
                UINT32 len;
                bs->Read(len);

                auto message = EncodingUtil::readAndConvertUTF8(bs,len);
                if (message.find("=== 家具规则") != std::string::npos) {
                    stash_state = SS_IDLE;
                    CUtils::chatprintf("Object stash finished");
                }
            }
        }
        return true;
    }

    void ZombieAutomata::onSave(nlohmann::json &j) {
        saveData(j, "pattern", pattern);
        saveData(j, "override_damage", override_damage);
        saveData(j, "use_invis", use_invis);
        saveData(j, "money_hack", money_hack);
        saveData(j, "object_stash", furniture_stash);
        saveData(j, "rate_factor", rate_factor);
        //saveData(j, "protect_against_vw", )
    }

    void ZombieAutomata::onLoad(const nlohmann::json &j) {
        loadData(j, "pattern", pattern);
        loadData(j, "override_damage", override_damage);
        loadData(j, "use_invis", use_invis);
        loadData(j, "money_hack", money_hack);
        loadData(j, "object_stash", furniture_stash);
        loadData(j, "rate_factor", rate_factor);
    }

    void ZombieAutomata::onEnable() {
        stash_state = SS_IDLE;
    }

    void ZombieAutomata::onDisable() {
        stash_state = SS_IDLE;
    }
}

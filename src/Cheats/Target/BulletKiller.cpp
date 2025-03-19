//
// Created by Administrator on 2024/10/7.
//

#include "BulletKiller.h"

#include <glm/fwd.hpp>

#include "matrix.h"
#include <glm/gtc/matrix_transform.hpp> // for rotate
#include <glm/gtc/type_ptr.hpp> // for value_ptr

#include "Plugin.h"
#include "TargetSelector.h"
#include "weapon_config.h"
#include "asmjit/core/osutils.h"
#include "Cheats/Weapon/FastShot.h"

namespace CheatModule {
    namespace Target {
        BulletKiller::BulletKiller() : CCheatModule("自动子弹攻击", "Bullet Killer") {
        }

        float BulletKiller::GetWeaponRange(int weaponid) {
            return use_legal_range ? WeaponConfig::GetWeaponRange(weaponid) : (float) 0x3f3f3f3f;
        }

        bool BulletKiller::IsTargetAvailable() {
            auto target = Plugin::getInstance().getTarget();
            if (auto target_ped = target->targetPlayerPed) {
                auto pool = samp::RefNetGame()->GetPlayerPool();
                auto localPed = pool->GetLocalPlayer()->m_pPed;
                int weapon_id = use_legal_delay
                                    ? override_weapon
                                    : static_cast<int>(target_ped->m_aWeapons[target_ped->m_nActiveWeaponSlot].
                                        m_eWeaponType);
                float weapon_range = GetWeaponRange(override_weapon);
                if (DistanceBetweenPoints(target_ped->GetPosition(), localPed->m_pGamePed->GetPosition()) <
                    weapon_range) {
                    return true;
                }
            }
            return false;
        }

        bool BulletKiller::onProcess(stOnFootData *data) {
            return CCheatModule::onProcess(data);
        }

        void BulletKiller::onProcess() {
            CCheatModule::onProcess();
            if (GetTickCount() - tick > delay) {
                if (IsTargetAvailable()) {
                    auto target_ped = Plugin::getInstance().getTarget()->targetPlayerPed;
                    auto pool = samp::RefNetGame()->GetPlayerPool();
                    auto localPed = pool->GetLocalPlayer()->m_pPed;
                    int weapon_id = use_legal_delay
                                        ? override_weapon
                                        : static_cast<int>(target_ped->m_aWeapons[target_ped->m_nActiveWeaponSlot].
                                            m_eWeaponType);
                    float weapon_range = GetWeaponRange(override_weapon);

                    // Send Onfoot data
                    if (GetTickCount() - tick_onfoot > 60) {
                        auto& pos = localPed->m_pGamePed->GetPosition();
                        stOnFootData pck{};
                        pck.byteArmor = static_cast<byte>(localPed->GetArmour());
                        pck.byteHealth = static_cast<byte>(localPed->GetHealth());
                        pck.byteCurrentWeapon = weapon_id;
                        pck.fPosition[0] = pos.x;
                        pck.fPosition[1] = pos.y;
                        pck.fPosition[2] = pos.z;

                        // Calculate direction vector from local player to target player
                        glm::vec3 localPosition = glm::vec3(pos.x, pos.y, pos.z);
                        glm::vec3 targetPosition = glm::vec3(
                            target_ped->m_matrix->pos.x,
                            target_ped->m_matrix->pos.y,
                            target_ped->m_matrix->pos.z);

                        glm::vec3 direction = glm::normalize(targetPosition - localPosition);

                        // Calculate the angle between the local player and target player
                        float angle = atan2(direction.y, direction.x); // in radians

                        // Face the local player to the target player
                        const auto mat = glm::rotate(
                            ConvertGtaMatrixToMat(localPed->m_pGameEntity->m_matrix),
                            angle, // Rotate by the calculated angle
                            glm::vec3(0, 0, 1)
                        );
                        const auto q = glm::normalize(glm::conjugate(glm::quat(mat)));
                        pck.fQuaternion[0] = q.w;
                        pck.fQuaternion[1] = q.x;
                        pck.fQuaternion[2] = q.y;
                        pck.fQuaternion[3] = q.z;

                        RakNet::BitStream bs;
                        bs.Write((BYTE) ID_PLAYER_SYNC);
                        bs.Write((PCHAR) &bs, sizeof(pck));
                        rakhook::send(&bs, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0);
                        tick_onfoot = GetTickCount();
                    }

                    // Send bullet data
                    stBulletData pck{};
                    pck.byteWeaponID = weapon_id;
                    pck.byteType = 1;
                    pck.fOrigin[0] = localPed->m_pGameEntity->m_matrix->pos.x;
                    pck.fOrigin[1] = localPed->m_pGameEntity->m_matrix->pos.y;
                    pck.fOrigin[2] = localPed->m_pGameEntity->m_matrix->pos.z;

                    pck.fTarget[0] = target_ped->m_matrix->pos.x;
                    pck.fTarget[1] = target_ped->m_matrix->pos.y;
                    pck.fTarget[2] = target_ped->m_matrix->pos.z;

                    pck.sTargetID = Plugin::getInstance().getTarget()->targetPlayerID;

                    RakNet::BitStream bs;
                    bs.Write((BYTE) ID_BULLET_SYNC);
                    bs.Write((PCHAR) &bs, sizeof(pck));
                    rakhook::send(&bs, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0);
                }
                tick = GetTickCount();
            }
        }
    } // Target
} // CheatModule

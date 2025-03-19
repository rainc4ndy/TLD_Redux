#include "FastShot.h"

#include <sampapi/CEntity.h>
#include <sampapi/CLocalPlayer.h>
#include <sampapi/CNetGame.h>
#include <sampapi/CPlayerPool.h>

#include <RakHook/rakhook.hpp>

#include "../../samp.h"
#include "../../scripting.h"
#include "../../Utils.h"

namespace CheatModule {
namespace Weapon {

FastShot::FastShot() : CCheatModule("快速射击", "Fast Shot") {
}

void FastShot::onProcess() {
    if (GetTickCount() - tick >= 20) {
        auto me = samp::RefNetGame()->GetPlayerPool()->GetLocalPlayer()->GetPed();
        if (me->m_pGamePed) {
            int actorid = CUtils::ScriptActorId(me->m_pGamePed);
            float v = speed + 1.f;
            ScriptCommand(&player_perform_animation_at_x_times_normal_rate, actorid, "python_crouchfire", v);
            ScriptCommand(&player_perform_animation_at_x_times_normal_rate, actorid, "python_fire", v);
            ScriptCommand(&player_perform_animation_at_x_times_normal_rate, actorid, "python_fire_poor", v);
            ScriptCommand(&player_perform_animation_at_x_times_normal_rate, actorid, "buddy_crouchfire", v);
            ScriptCommand(&player_perform_animation_at_x_times_normal_rate, actorid, "buddy_fire", v);
            ScriptCommand(&player_perform_animation_at_x_times_normal_rate, actorid, "buddy_fire_poor", v);
            ScriptCommand(&player_perform_animation_at_x_times_normal_rate, actorid, "2guns_crouchfire", v);
            ScriptCommand(&player_perform_animation_at_x_times_normal_rate, actorid, "colt45_crouchfire", v);
            ScriptCommand(&player_perform_animation_at_x_times_normal_rate, actorid, "colt45_fire", v);
            ScriptCommand(&player_perform_animation_at_x_times_normal_rate, actorid, "colt45_fire_2hands", v);
            ScriptCommand(&player_perform_animation_at_x_times_normal_rate, actorid, "RIFLE_crouchfire", v);
            ScriptCommand(&player_perform_animation_at_x_times_normal_rate, actorid, "RIFLE_fire", v);
            ScriptCommand(&player_perform_animation_at_x_times_normal_rate, actorid, "RIFLE_fire_poor", v);
            ScriptCommand(&player_perform_animation_at_x_times_normal_rate, actorid, "shotgun_crouchfire", v);
            ScriptCommand(&player_perform_animation_at_x_times_normal_rate, actorid, "shotgun_fire", v);
            ScriptCommand(&player_perform_animation_at_x_times_normal_rate, actorid, "shotgun_fire_poor", v);
            ScriptCommand(&player_perform_animation_at_x_times_normal_rate, actorid, "SilenceCrouchfire", v);
            ScriptCommand(&player_perform_animation_at_x_times_normal_rate, actorid, "Silence_fire", v);
            ScriptCommand(&player_perform_animation_at_x_times_normal_rate, actorid, "TEC_crouchfire", v);
            ScriptCommand(&player_perform_animation_at_x_times_normal_rate, actorid, "TEC_fire", v);
            ScriptCommand(&player_perform_animation_at_x_times_normal_rate, actorid, "UZI_crouchfire", v);
            ScriptCommand(&player_perform_animation_at_x_times_normal_rate, actorid, "UZI_fire", v);
            ScriptCommand(&player_perform_animation_at_x_times_normal_rate, actorid, "UZI_fire_poor", v);
        }
        tick = GetTickCount();
    }
}

void FastShot::onSave(nlohmann::json &j) {
    saveData(j, "speed", speed);
}

void FastShot::onLoad(const nlohmann::json &j) {
    loadData(j, "speed", speed);
}

}  // namespace Hack
}  // namespace CheatModule
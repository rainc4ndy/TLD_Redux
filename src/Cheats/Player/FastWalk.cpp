#include "FastWalk.h"

#include <sampapi/CEntity.h>
#include <sampapi/CLocalPlayer.h>
#include <sampapi/CNetGame.h>
#include <sampapi/CPlayerPool.h>

#include "../../samp.h"
#include "../../scripting.h"
#include "../../Utils.h"

namespace CheatModule {
namespace Player {
FastWalk::FastWalk() : CCheatModule("暴走", "Fast Walk") {
}

void FastWalk::onProcess() {
    if (GetTickCount() - tick > 32) {
        auto p = samp::RefNetGame()->GetPlayerPool()->GetLocalPlayer();
        
        int iActor = CUtils::ScriptActorId(p->GetPed()->m_pGamePed);
        ScriptCommand(&player_perform_animation_at_x_times_normal_rate, iActor,
                      "WOMAN_RUN", speed);
        ScriptCommand(&player_perform_animation_at_x_times_normal_rate, iActor,
                      "WOMAN_RUNBUSY", speed);
        ScriptCommand(&player_perform_animation_at_x_times_normal_rate, iActor,
                      "WOMAN_RUNPANIC", speed);
        ScriptCommand(&player_perform_animation_at_x_times_normal_rate, iActor,
                      "WOMAN_RUNSEXY", speed);
        ScriptCommand(&player_perform_animation_at_x_times_normal_rate, iActor,
                      "SPRINT_CIVI", speed);
        ScriptCommand(&player_perform_animation_at_x_times_normal_rate, iActor,
                      "SPRINT_PANIC", speed);
        ScriptCommand(&player_perform_animation_at_x_times_normal_rate, iActor,
                      "SWAT_RUN", speed);
        ScriptCommand(&player_perform_animation_at_x_times_normal_rate, iActor,
                      "WOMAN_RUN", speed);
        ScriptCommand(&player_perform_animation_at_x_times_normal_rate, iActor,
                      "WOMAN_RUNBUSY", speed);
        ScriptCommand(&player_perform_animation_at_x_times_normal_rate, iActor,
                      "WOMAN_RUNPANIC", speed);
        ScriptCommand(&player_perform_animation_at_x_times_normal_rate, iActor,
                      "WOMAN_RUNSEXY", speed);
        ScriptCommand(&player_perform_animation_at_x_times_normal_rate, iActor,
                      "FATSPRINT", speed);

        ScriptCommand(&player_perform_animation_at_x_times_normal_rate, iActor,
                      "WOMAN_RUN", speed);
        ScriptCommand(&player_perform_animation_at_x_times_normal_rate, iActor,
                      "WOMAN_RUNBUSY", speed);
        ScriptCommand(&player_perform_animation_at_x_times_normal_rate, iActor,
                      "WOMAN_RUNPANIC", speed);
        ScriptCommand(&player_perform_animation_at_x_times_normal_rate, iActor,
                      "WOMAN_RUNSEXY", speed);
        ScriptCommand(&player_perform_animation_at_x_times_normal_rate, iActor,
                      "SPRINT_CIVI", speed);
        ScriptCommand(&player_perform_animation_at_x_times_normal_rate, iActor,
                      "SPRINT_PANIC", speed);
        ScriptCommand(&player_perform_animation_at_x_times_normal_rate, iActor,
                      "SWAT_RUN", speed);
        ScriptCommand(&player_perform_animation_at_x_times_normal_rate, iActor,
                      "WOMAN_RUN", speed);
        ScriptCommand(&player_perform_animation_at_x_times_normal_rate, iActor,
                      "WOMAN_RUNBUSY", speed);
        ScriptCommand(&player_perform_animation_at_x_times_normal_rate, iActor,
                      "WOMAN_RUNPANIC", speed);
        ScriptCommand(&player_perform_animation_at_x_times_normal_rate, iActor,
                      "WOMAN_RUNSEXY", speed);
        ScriptCommand(&player_perform_animation_at_x_times_normal_rate, iActor,
                      "FATSPRINT", speed);
        tick = GetTickCount();
    }
}

void FastWalk::onSave(nlohmann::json &j) {
    saveData<decltype(speed)>(j, "speed", speed);
}

void FastWalk::onLoad(const nlohmann::json &j) {
    loadData<decltype(speed)>(j, "speed", speed);
}
}  // namespace Player
}  // namespace CheatModule
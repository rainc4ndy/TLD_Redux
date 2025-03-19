//
// Created by Administrator on 2024/10/7.
//

#ifndef ZOMBIEAUTOMATA_H
#define ZOMBIEAUTOMATA_H


#include <glm/vec3.hpp>

#include "../../CCheatModule.h"

namespace CheatModule {
namespace Hack {

class ZombieAutomata : public CCheatModule{
private:
    unsigned int tick, tick_onfoot;
    int target_cache;
    unsigned int pickup_tick;
    void sync_all_money();
    void furniture_process();
    bool is_skin_satisfied(int skin);

    enum eStashState {
        SS_IDLE,
        SS_WAIT_FOR_OBJECT
    };
    int stash_state;
public:
    enum eZombiePattern {
        ZP_ZOMBIE_ONLY,
        ZP_BOSS_ONLY,
        ZP_ALL
    };
    int pattern; //过滤模式
    bool override_damage; //用沙鹰的伤害覆盖
    bool use_invis; //是否使用隐身
    bool money_hack;
    float rate_factor;
    bool furniture_stash;

    glm::vec3 stash_position;

    void setCurrentStashPosition();

    ZombieAutomata();
    bool onProcess(stOnFootData *data) override;
    bool onProcess(stAimData *data) override;
    void onProcess() override;
    bool onProcess(bool fromServer, int rpc, RakNet::BitStream *bs) override;

    void onSave(nlohmann::json &j) override;
    void onLoad(const nlohmann::json &j) override;

    void onEnable() override;
    void onDisable() override;
};

} // Hack
} // CheatModule

#endif //ZOMBIEAUTOMATA_H

//
// Created by Administrator on 2024/10/7.
//

#ifndef BULLETKILLER_H
#define BULLETKILLER_H
#include "CCheatModule.h"

namespace CheatModule {
namespace Target {

class BulletKiller : public CCheatModule {
private:
    unsigned int tick;
    unsigned int tick_onfoot;
public:
    bool use_vehicle;
    unsigned int delay;
    bool use_override_weapon;
    int override_weapon;

    bool use_legal_delay;
    bool use_legal_range;

    bool follow_target;
    BulletKiller();

    float GetWeaponRange(int weaponid);

    bool IsTargetAvailable();

    bool onProcess(stOnFootData *data) override;
    void onProcess() override;
};

} // Target
} // CheatModule

#endif //BULLETKILLER_H

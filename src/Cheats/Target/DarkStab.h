//
// Created by Administrator on 2024/10/7.
//

#ifndef DARKSTAB_H
#define DARKSTAB_H

#include <CPed.h>
#include "../../CCheatModule.h"

namespace CheatModule::Target {
    class DarkStab : public CCheatModule {
    private:
        enum eState {
            TARGET_ASSASSIN_NO,
            TARGET_ASSASSIN_TELEPORTED,
            TARGET_ASSASSIN_AIMED,
            TARGET_ASSASSIN_ATTACKED,
        };

        int tick;
        int state;


        bool can_be_darkstab(CPed *ped);

    public:

        float range;
        bool use_own_weapon;

        DarkStab();

        void onProcess() override;
        bool onProcess(stOnFootData *data) override;

        void onSave(nlohmann::json &j) override;
        void onLoad(const nlohmann::json &j) override;

        void onEnable() override;
        void onDisable() override;
    };
}

#endif //DARKSTAB_H

//
// Created by Administrator on 2025/2/15.
//

#ifndef CTRIGGERBOT_H
#define CTRIGGERBOT_H

#include "../../CCheatModule.h"

namespace CheatModule::Weapon {
    class CTriggerBot : public CCheatModule {
    private:
    public:
        float range;

        CTriggerBot();

        void onProcess() override;
        void onSave(nlohmann::json &j) override;
        void onLoad(const nlohmann::json &j) override;
    };
}

#endif //CTRIGGERBOT_H

//
// Created by Administrator on 2024/10/7.
//

#ifndef SUPREMEDASHING_H
#define SUPREMEDASHING_H
#include <CPed.h>

#include "../../CCheatModule.h"


namespace CheatModule::Vehicle {
    class SupremeDashing : public CCheatModule {
    private:
        int tick;
        int state;
        int packet_cnt;
        CVector targetPosition;
    public:
        enum eState {
            SD_IDLE,
            SD_WORKING,
            SD_WAIT_FOR_INCAR
        };
        SupremeDashing();

        void RestoreIncar();

        int delay;
        float speed;
        void onProcess() override;
        bool onProcess(stInCarData *data) override;
        void onEnable() override;
        void onDisable() override;

        void onSave(nlohmann::json &j) override;
        void onLoad(const nlohmann::json &j) override;
    };
}


#endif //SUPREMEDASHING_H

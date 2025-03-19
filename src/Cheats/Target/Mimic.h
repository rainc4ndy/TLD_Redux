//
// Created by Administrator on 2024/10/7.
//

#ifndef MIMIC_H
#define MIMIC_H
#include <CPed.h>
#include <CVector.h>

#include "../../CCheatModule.h"
namespace CheatModule::Target {
    class Mimic : public CCheatModule {
    private:
        int tick;
        void getOffsetedLocation(CVector ori, CVector &loc, float angle);
    public:
        Mimic();

        enum eMimicOffset {
            EMO_FRONT,
            EMO_BACK,
            EMO_LEFT,
            EMO_RIGHT
        };

        int offset_mode;
        bool local;

        void onProcess() override;
        bool onProcess(stOnFootData *data) override;

        void onSave(nlohmann::json &j) override;
        void onLoad(const nlohmann::json &j) override;
    };
}

#endif //MIMIC_H

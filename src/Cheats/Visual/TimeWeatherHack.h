//
// Created by Administrator on 2025/2/15.
//

#ifndef TLD_REDUX_TIMEWEATHERHACK_H
#define TLD_REDUX_TIMEWEATHERHACK_H

#include "CCheatModule.h"

namespace CheatModule::Visual {
    class TimeWeatherHack : public CCheatModule {
    public:
        TimeWeatherHack();
        void onProcess() override;
        void onSave(nlohmann::json &j) override;
        void onLoad(const nlohmann::json &j) override;

        int time, weather;
    };

} // CCheatModule

#endif //TLD_REDUX_TIMEWEATHERHACK_H

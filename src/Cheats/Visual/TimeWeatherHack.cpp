//
// Created by Administrator on 2025/2/15.
//

#include "TimeWeatherHack.h"
#include "CWeather.h"

namespace CheatModule::Visual {
    TimeWeatherHack::TimeWeatherHack() : CCheatModule("控制天气时间","Time Weather") {}

    void TimeWeatherHack::onProcess() {
        CWeather::ForceWeatherNow( static_cast<short>(weather));
        *(uint8_t *)0x00B70153 = ( uint8_t ) time;
    }
    void TimeWeatherHack::onSave(nlohmann::json &j) {
        saveData(j,"time",time);
        saveData(j,"weather",weather);
    };
    void TimeWeatherHack::onLoad(const nlohmann::json &j) {
        loadData(j,"time",time);
        loadData(j,"weather",weather);
    }
} // CCheatModule
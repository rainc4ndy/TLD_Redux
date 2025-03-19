//
// Created by Administrator on 2024/10/6.
//

#include "GocModule.h"

#include <common.h>
#include <RakHook/rakhook.hpp>

#include "../../CMenu.h"
#include "../../Plugin.h"
#include "../../CConfig.h"
#include "../../Utils.h"
#include "../../keyhook.h"


namespace CheatModule::Vehicle {
    bool GocModule::ok_with_control() {
        if (GetTickCount() - tick >= delay) {
            if (KEY_DOWN(VK_LBUTTON) &&
                !Plugin::getInstance().getMenu()->isEnabled()) {
                return true;
                }
        }
        return false;
    }

    void GocModule::update_tick() {
        tick = GetTickCount();
    }

    GocModule::GocModule(std::string name, std::string nameEn) : CCheatModule(name, nameEn) {

    }

    void GocModule::onSave(nlohmann::json &j) {
        saveData<decltype(speed)>(j, "speed", speed);
        saveData<decltype(delay)>(j, "delay", delay);
    }

    void GocModule::onLoad(const nlohmann::json &j) {
        loadData<decltype(speed)>(j, "speed", speed);
        loadData<decltype(delay)>(j, "delay", delay);

    }
}

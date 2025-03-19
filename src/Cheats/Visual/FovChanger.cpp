//
// Created by Administrator on 2025/2/17.
//

#include "FovChanger.h"

#include <CCamera.h>

namespace CheatModule::Visual {
    FovChanger::FovChanger() : CCheatModule("视野调整", "Fov Changer") {

    }

    void FovChanger::onProcess() {
        TheCamera.LerpFOV(fov,fov,1000,true);
    }

    void FovChanger::onSave(nlohmann::json &j) {
        saveData(j, "fov", fov);
    }

    void FovChanger::onLoad(const nlohmann::json &j) {
        loadData(j, "fov", fov);
    }

    void FovChanger::onDisable() {
        TheCamera.LerpFOV(1.0,1.0,1000,true);
    }
}

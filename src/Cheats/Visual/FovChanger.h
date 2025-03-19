//
// Created by Administrator on 2025/2/17.
//

#ifndef FOVCHANGER_H
#define FOVCHANGER_H

#include "CCheatModule.h"

namespace CheatModule::Visual {

class FovChanger : public CCheatModule {
public:
    float fov;
    FovChanger();
    void onProcess() override;
    void onSave(nlohmann::json &j) override;
    void onLoad(const nlohmann::json &j) override;

    void onDisable() override;
};

} // CheatModule

#endif //FOVCHANGER_H

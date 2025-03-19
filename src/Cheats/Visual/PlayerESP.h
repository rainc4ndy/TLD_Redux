//
// Created by Administrator on 2024/10/9.
//

#ifndef PLAYERESP_H
#define PLAYERESP_H
#include <imgui.h>

#include "CCheatModule.h"

namespace CheatModule::Visual {

class PlayerESP : public CCheatModule {
public:
    bool box, bone, line, info;
    D3DCOLOR color;
    float thickness;
    bool rainbow;
    PlayerESP();

    ImColor color_cache;

    static void Draw3DBoxAroundPed(CPed *pPed, float width, float height, float depth, D3DCOLOR color, float thickness = 1.0);
    static void DrawPedBones(CPed *pPed, D3DCOLOR color, float thickness = 1.0);
    static void DrawLineToPed(CPed *localPed, CPed *targetPed, D3DCOLOR color, float thickness = 1.0);
    static void DrawPedInfo(CPed *localPed, CPed *pPed, D3DCOLOR color);

    void onProcessVisual() override;
    void onSave(nlohmann::json &j) override;
    void onLoad(const nlohmann::json &j) override;
};
}


#endif //PLAYERESP_H

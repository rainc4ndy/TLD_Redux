//
// Created by Administrator on 2024/10/8.
//

#include "TargetRenderer.h"

#include <CPed.h>

#include "CRenderer.h"
#include "d3drender.h"
#include "matrix.h"
#include "Plugin.h"
#include "TargetSelector.h"
#include "Utils.h"

namespace CheatModule::Target {
    TargetRenderer::TargetRenderer() : CCheatModule("透视目标", "Target Render") {}
    void TargetRenderer::drawTargetOnPed(CPed *pPed) {
        if (!pPed)
            return;
        auto renderer = Plugin::getInstance().getRenderer();
        D3DXVECTOR3 target_pos_world, target_pos_screen;
        target_pos_world.x = pPed->m_matrix->pos.x;
        target_pos_world.y = pPed->m_matrix->pos.y;
        target_pos_world.z = pPed->m_matrix->pos.z + 0.5f;
        CalcScreenCoors(&target_pos_world, &target_pos_screen);

        if (target_pos_screen.z < 1.f)
            return;

        D3DCOLOR color = D3DCOLOR_ARGB(255, 255, 0, 0);
        constexpr int offset = 12;
        constexpr float thickness = 2.0f; // Line thickness for the cross target

        // Draw the cross target with thickness
        renderer->renderer->D3DLine(
            static_cast<int>(target_pos_screen.x) - offset,
            static_cast<int>(target_pos_screen.y) + offset,
            static_cast<int>(target_pos_screen.x) + offset,
            static_cast<int>(target_pos_screen.y) - offset,
            color, thickness);

        renderer->renderer->D3DLine(
            static_cast<int>(target_pos_screen.x) + offset,
            static_cast<int>(target_pos_screen.y) + offset,
            static_cast<int>(target_pos_screen.x) - offset,
            static_cast<int>(target_pos_screen.y) - offset,
            color, thickness);

        /*// Draw a circle around the ped
        renderer->renderer->D3DCircle(
            target_pos_screen.x,
            target_pos_screen.y,
            circle_radius,
            color,
            num_segments);*/

    }

    void TargetRenderer::onProcessVisual() {
        auto target = Plugin::getInstance().getTarget();
        if (target->targetPlayerPed) {
            drawTargetOnPed(target->targetPlayerPed);
        }
    }
}

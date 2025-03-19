//
// Created by Administrator on 2024/10/9.
//

#include "PlayerESP.h"

#include <CPed.h>

#include "CRenderer.h"
#include "d3drender.h"
#include "matrix.h"
#include "colors.h"
#include "Plugin.h"
#include "CAnimManager.h"
#include "../../../test/imgui_test/HueCycler.h"

namespace CheatModule::Visual {
    PlayerESP::PlayerESP() : CCheatModule("玩家透视", "Player ESP") {
    }

    void PlayerESP::Draw3DBoxAroundPed(CPed *pPed, float width, float height, float depth, D3DCOLOR color,
                                       float thickness) {
        D3DXVECTOR3 corners[8]; // To hold the 8 corners of the box
        D3DXVECTOR3 screenCorners[8]; // To hold the screen space coordinates of the corners

        // Get the position and rotation of the ped
        D3DXVECTOR3 pedPos = {pPed->m_matrix->pos.x, pPed->m_matrix->pos.y, pPed->m_matrix->pos.z};
        float pedRotation = pPed->m_fCurrentRotation; // Assuming this holds the ped's rotation in radians

        // Define the half-size of the box
        D3DXVECTOR3 halfSize(width / 2.0f, height / 2.0f, depth / 2.0f);

        // Create a rotation matrix for rotating the box around the Z-axis (assuming this is the axis of rotation)
        D3DXMATRIX rotationMatrix;
        D3DXMatrixRotationZ(&rotationMatrix, pedRotation);

        // Calculate the 8 corners of the bounding box in local space (before rotation)
        D3DXVECTOR3 localCorners[8] = {
            {-halfSize.x, halfSize.y, halfSize.z}, // Top-left-front
            {halfSize.x, halfSize.y, halfSize.z}, // Top-right-front
            {-halfSize.x, -halfSize.y, halfSize.z}, // Bottom-left-front
            {halfSize.x, -halfSize.y, halfSize.z}, // Bottom-right-front
            {-halfSize.x, halfSize.y, -halfSize.z}, // Top-left-back
            {halfSize.x, halfSize.y, -halfSize.z}, // Top-right-back
            {-halfSize.x, -halfSize.y, -halfSize.z}, // Bottom-left-back
            {halfSize.x, -halfSize.y, -halfSize.z} // Bottom-right-back
        };

        // Apply the rotation and translate to world space by adding the ped's position
        for (int i = 0; i < 8; ++i) {
            // Rotate the corner point
            D3DXVec3TransformCoord(&corners[i], &localCorners[i], &rotationMatrix);
            // Translate the rotated point by the ped's position
            corners[i] += pedPos;
        }

        // Convert world coordinates to screen coordinates
        for (int i = 0; i < 8; ++i) {
            CalcScreenCoors(&corners[i], &screenCorners[i]);
            if (screenCorners[i].z < 1.0f) {
                return; // Skip drawing if any corner is off-screen or behind the camera
            }
        }

        // Draw the edges of the box
        auto renderer = Plugin::getInstance().getRenderer();
        // Front face
        renderer->renderer->D3DLine(static_cast<int>(screenCorners[0].x), static_cast<int>(screenCorners[0].y),
                                    static_cast<int>(screenCorners[1].x), static_cast<int>(screenCorners[1].y),
                                    color, thickness); // Top-left to Top-right
        renderer->renderer->D3DLine(static_cast<int>(screenCorners[1].x), static_cast<int>(screenCorners[1].y),
                                    static_cast<int>(screenCorners[3].x), static_cast<int>(screenCorners[3].y),
                                    color, thickness); // Top-right to Bottom-right
        renderer->renderer->D3DLine(static_cast<int>(screenCorners[3].x), static_cast<int>(screenCorners[3].y),
                                    static_cast<int>(screenCorners[2].x), static_cast<int>(screenCorners[2].y),
                                    color, thickness); // Bottom-right to Bottom-left
        renderer->renderer->D3DLine(static_cast<int>(screenCorners[2].x), static_cast<int>(screenCorners[2].y),
                                    static_cast<int>(screenCorners[0].x), static_cast<int>(screenCorners[0].y),
                                    color, thickness); // Bottom-left to Top-left

        // Back face
        renderer->renderer->D3DLine(static_cast<int>(screenCorners[4].x), static_cast<int>(screenCorners[4].y),
                                    static_cast<int>(screenCorners[5].x), static_cast<int>(screenCorners[5].y),
                                    color, thickness); // Top-left to Top-right
        renderer->renderer->D3DLine(static_cast<int>(screenCorners[5].x), static_cast<int>(screenCorners[5].y),
                                    static_cast<int>(screenCorners[7].x), static_cast<int>(screenCorners[7].y),
                                    color, thickness); // Top-right to Bottom-right
        renderer->renderer->D3DLine(static_cast<int>(screenCorners[7].x), static_cast<int>(screenCorners[7].y),
                                    static_cast<int>(screenCorners[6].x), static_cast<int>(screenCorners[6].y),
                                    color, thickness); // Bottom-right to Bottom-left
        renderer->renderer->D3DLine(static_cast<int>(screenCorners[6].x), static_cast<int>(screenCorners[6].y),
                                    static_cast<int>(screenCorners[4].x), static_cast<int>(screenCorners[4].y),
                                    color, thickness); // Bottom-left to Top-left

        // Connecting front and back faces
        renderer->renderer->D3DLine(static_cast<int>(screenCorners[0].x), static_cast<int>(screenCorners[0].y),
                                    static_cast<int>(screenCorners[4].x), static_cast<int>(screenCorners[4].y),
                                    color, thickness); // Front top-left to Back top-left
        renderer->renderer->D3DLine(static_cast<int>(screenCorners[1].x), static_cast<int>(screenCorners[1].y),
                                    static_cast<int>(screenCorners[5].x), static_cast<int>(screenCorners[5].y),
                                    color, thickness); // Front top-right to Back top-right
        renderer->renderer->D3DLine(static_cast<int>(screenCorners[2].x), static_cast<int>(screenCorners[2].y),
                                    static_cast<int>(screenCorners[6].x), static_cast<int>(screenCorners[6].y),
                                    color, thickness); // Front bottom-left to Back bottom-left
        renderer->renderer->D3DLine(static_cast<int>(screenCorners[3].x), static_cast<int>(screenCorners[3].y),
                                    static_cast<int>(screenCorners[7].x), static_cast<int>(screenCorners[7].y),
                                    color, thickness); // Front bottom-right to Back bottom-right
    }


    void PlayerESP::DrawPedBones(CPed *pPed, D3DCOLOR color, float thickness) {
        auto renderer = Plugin::getInstance().getRenderer();
        // pPed->m_apBones[0-19]->m_vecOffset;
        D3DXVECTOR3 bonePosWorld, bonePosScreen;
        // Define bones to draw connections between (spine, arms, legs, etc.)
        const static std::vector<std::pair<int, int> > bonePairs = {
            {BONE_PELVIS1, BONE_SPINE1}, // Pelvis to spine
            {BONE_SPINE1, BONE_UPPERTORSO}, // Spine to upper torso
            {BONE_UPPERTORSO, BONE_NECK}, // Upper torso to neck
            {BONE_NECK, BONE_HEAD}, // Neck to head
            {BONE_RIGHTSHOULDER, BONE_RIGHTELBOW}, // Right shoulder to right elbow
            {BONE_RIGHTELBOW, BONE_RIGHTWRIST}, // Right elbow to right wrist
            {BONE_LEFTSHOULDER, BONE_LEFTELBOW}, // Left shoulder to left elbow
            {BONE_LEFTELBOW, BONE_LEFTWRIST}, // Left elbow to left wrist
            {BONE_LEFTHIP, BONE_LEFTKNEE}, // Left hip to left knee
            {BONE_LEFTKNEE, BONE_LEFTANKLE}, // Left knee to left ankle
            {BONE_RIGHTHIP, BONE_RIGHTKNEE}, // Right hip to right knee
            {BONE_RIGHTKNEE, BONE_RIGHTANKLE}, // Right knee to right ankle

            {BONE_RIGHTSHOULDER, BONE_LEFTSHOULDER}, //connected
            {BONE_RIGHTHIP, BONE_LEFTHIP} //connected
        };

        for (auto &bonePair: bonePairs) {
            // Get the bone positions in world coordinates
            RwV3d pos;
            pPed->GetBonePosition(pos, bonePair.first, false);
            D3DXVECTOR3 bone1PosWorld = {pos.x, pos.y, pos.z};
            pPed->GetBonePosition(pos, bonePair.second, false);
            D3DXVECTOR3 bone2PosWorld = {pos.x, pos.y, pos.z};

            // Transform bone positions to screen coordinates
            CalcScreenCoors(&bone1PosWorld, &bonePosScreen);
            if (bonePosScreen.z > 1.0f) {
                D3DXVECTOR3 bone1ScreenPos = bonePosScreen;
                CalcScreenCoors(&bone2PosWorld, &bonePosScreen);
                if (bonePosScreen.z > 1.0f) {
                    D3DXVECTOR3 bone2ScreenPos = bonePosScreen;

                    // Draw a line between the two bones
                    renderer->renderer->D3DLine(static_cast<int>(bone1ScreenPos.x), static_cast<int>(bone1ScreenPos.y),
                             static_cast<int>(bone2ScreenPos.x), static_cast<int>(bone2ScreenPos.y),
                             color, thickness);
                }
            }
        }
    }

    void PlayerESP::DrawLineToPed(CPed *localPed, CPed *targetPed, D3DCOLOR color, float thickness) {
        auto renderer = Plugin::getInstance().getRenderer();

        // Get local player's position
        D3DXVECTOR3 localPos = {localPed->m_matrix->pos.x, localPed->m_matrix->pos.y, localPed->m_matrix->pos.z};
        D3DXVECTOR3 localScreenPos;

        // Convert local player position to screen coordinates
        CalcScreenCoors(&localPos, &localScreenPos);
        if (localScreenPos.z > 1.0f) {
            // Get target player's position
            D3DXVECTOR3 targetPos = {
                targetPed->m_matrix->pos.x, targetPed->m_matrix->pos.y, targetPed->m_matrix->pos.z
            };
            D3DXVECTOR3 targetScreenPos;

            // Convert target ped position to screen coordinates
            CalcScreenCoors(&targetPos, &targetScreenPos);
            if (targetScreenPos.z > 1.0f) {
                // Draw a line from the local player to the target player
                renderer->renderer->D3DLine(static_cast<int>(localScreenPos.x), static_cast<int>(localScreenPos.y),
                            static_cast<int>(targetScreenPos.x), static_cast<int>(targetScreenPos.y),
                            color, thickness);
            }
        }
    }

    void PlayerESP::DrawPedInfo(CPed *localPed, CPed *pPed, D3DCOLOR color) {
        auto renderer = Plugin::getInstance().getRenderer();
        D3DXVECTOR3 targetPos = {pPed->m_matrix->pos.x, pPed->m_matrix->pos.y, pPed->m_matrix->pos.z};
        D3DXVECTOR3 targetScreenPos;
        char buffer[256]{};
        CalcScreenCoors(&targetPos, &targetScreenPos);
        if (targetScreenPos.z > 1.0f) {
            sprintf(buffer, "Range:%0.2f\nSkin: %d\nRot:%0.2f\nSpeed:%0.2f\nLoc:%s",
                    DistanceBetweenPoints(localPed->GetPosition(), pPed->GetPosition()),
                    pPed->m_nModelIndex,
                    pPed->m_fCurrentRotation,
                    pPed->m_fMovingSpeed,
                    pPed->m_nPedFlags.bIsStanding ? "On Ground" : "Flying"
            );
            renderer->infoFont->PrintShadow(targetScreenPos.x, targetScreenPos.y, color, buffer);
        }
    }


    void PlayerESP::onProcessVisual() {
        auto pool = samp::RefNetGame()->GetPlayerPool();
        auto localPed = pool->GetLocalPlayer()->m_pPed; // Get local player ped

        D3DCOLOR now_color = color;
        if (rainbow) {
            auto cycler = Plugin::getInstance().getHueCycler();
            auto cl = cycler->currentRgba(255);
            now_color = D3DCOLOR_RGBA(static_cast<int>(cl.r * 255), static_cast<int>(cl.g * 255), static_cast<int>(cl.b * 255), static_cast<int>(cl.a * 255));
        }

        for (int i = 0; i < pool->m_nLargestId; i++) {
            if (pool->m_bNotEmpty[i]) {
                auto ped = pool->GetPlayer(i)->m_pPed;
                if (!ped)
                    continue;

                if (box) {
                    Draw3DBoxAroundPed(ped->m_pGamePed, 1.1f, 1.1f, 2.1f, now_color, thickness);
                }
                if (bone) {
                    DrawPedBones(ped->m_pGamePed, now_color, thickness);
                }
                if (line) {
                    DrawLineToPed(localPed->m_pGamePed, ped->m_pGamePed, now_color, thickness);
                }
                if (info) {
                    DrawPedInfo(localPed->m_pGamePed, ped->m_pGamePed, now_color);
                }
            }
        }
    }

    void PlayerESP::onSave(nlohmann::json &j) {
        saveData(j, "bone", bone);
        saveData(j, "box", box);
        saveData(j, "line", line);
        saveData(j, "info", info);
        saveData(j, "rainbow", rainbow);
        saveData(j, "thickness", thickness);
        saveData(j, "color", color);
    }

    void PlayerESP::onLoad(const nlohmann::json &j) {
        loadData(j, "bone", bone);
        loadData(j, "box", box);
        loadData(j, "line", line);
        loadData(j, "info", info);
        loadData(j, "rainbow", rainbow);
        loadData(j, "thickness", thickness);
        loadData(j, "color", color);
        color_cache = D3DColorToImColor(color);
    }
}

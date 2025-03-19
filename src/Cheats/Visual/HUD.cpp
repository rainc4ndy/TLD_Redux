#include "HUD.h"

#include <imgui.h>
#include <sampapi/CNetGame.h>
#include <sampapi/CInput.h>

#include <string>
#include <vector>

#include "../../CCheatManager.h"
#include "../../CCheatModule.h"
#include "../../CMenu.h"
#include "../../CRenderer.h"
#include "../../HueCycler.h"
#include "../../Plugin.h"
#include "../../d3drender.h"
#include "../../samp.h"
#include "colors.h"

namespace CheatModule {
    namespace Visual {
        HUD::HUD() : CCheatModule("自定义HUD颜色", "HUD Color") {
        }

        void HUD::col_hud(DWORD color) {
            *(DWORD *) (0xBAB22C) = color;
            *(DWORD *) (0xBAB230) = color;
            *(DWORD *) (0xBAB234) = color;
            *(DWORD *) (0xBAB238) = color;
            *(DWORD *) (0xBAB23C) = color;
            *(DWORD *) (0xBAB260) = color;
            *(DWORD *) (0xBAB240) = color;
            *(DWORD *) (0xBAB244) = color;
            *(DWORD *) (0xBAB248) = color;
            *(DWORD *) (0xBAB24C) = color;
            *(DWORD *) (0xBAB250) = color;
            *(DWORD *) (0xBAB254) = color;
            *(DWORD *) (0xBAB258) = color;
            *(DWORD *) (0xBAB25C) = color;
            *(DWORD *) (0xBAB260) = color;
        }

        void HUD::col_textdraws(ImVec4 *color) {
            auto td_pool = samp::RefNetGame()->m_pPools->m_pTextDraw;
            for (int i = 0; i < td_pool->MAX_TEXTDRAWS; i++) {
                if (td_pool->m_bNotEmpty[i]) {
                    if (td_pool->m_pObject[i] != nullptr) {
                        td_pool->m_pObject[i]->m_data.m_letterColor = D3DCOLOR_ARGB(
                            255, (BYTE)(color->x * 255.f), (BYTE)(color->y * 255.f),
                            (BYTE)(color->z * 255.f));
                    }
                }
            }

            for (int i = 0; i < td_pool->MAX_LOCAL_TEXTDRAWS; i++) {
                int y = i + td_pool->MAX_TEXTDRAWS;
                if (td_pool->m_bNotEmpty[y]) {
                    if (td_pool->m_pObject[y] != nullptr) {
                        td_pool->m_pObject[y]->m_data.m_letterColor = D3DCOLOR_ARGB(
                            255, (BYTE)(color->x * 255.f), (BYTE)(color->y * 255.f),
                            (BYTE)(color->z * 255.f));
                    }
                }
            }
        }

        void HUD::col_menu(ImVec4 *color) {
            // menu
            *(BYTE *) (0x579A8B + 1) = (BYTE) (color->x * 255.f);
            *(BYTE *) (0x579A89 + 1) = (BYTE) (color->y * 255.f);
            *(BYTE *) (0x579A87 + 1) = (BYTE) (color->z * 255.f);

            *(BYTE *) (0x579A6C + 1) = (BYTE) (color->x * 255.f);
            *(BYTE *) (0x579A67 + 1) = (BYTE) (color->y * 255.f);
            *(BYTE *) (0x579A62 + 1) = (BYTE) (color->z * 255.f);

            *(BYTE *) (0x579A8B + 1) = (BYTE) (color->x * 255.f);
            *(BYTE *) (0x579A89 + 1) = (BYTE) (color->y * 255.f);
            *(BYTE *) (0x579A87 + 1) = (BYTE) (color->z * 255.f);

            *(BYTE *) (0x57B9DB + 1) = (BYTE) (color->x * 255.f);
            *(BYTE *) (0x57B9D6 + 1) = (BYTE) (color->y * 255.f);
            *(BYTE *) (0x57B9D1 + 1) = (BYTE) (color->z * 255.f);
            // mouse
            *(BYTE *) (0x57C0A3 + 1) = (BYTE) (color->x * 255.f);
            *(BYTE *) (0x57C09D + 1) = (BYTE) (color->y * 255.f);
            *(BYTE *) (0x57C09B + 1) = (BYTE) (color->z * 255.f);

            *(BYTE *) (0x57C112 + 1) = (BYTE) (color->x * 255.f);
            *(BYTE *) (0x57C109 + 1) = (BYTE) (color->y * 255.f);
            *(BYTE *) (0x57C104 + 1) = (BYTE) (color->z * 255.f);

            *(BYTE *) (0x57C112 + 1) = (BYTE) (color->x * 255.f);
            *(BYTE *) (0x57C109 + 1) = (BYTE) (color->y * 255.f);
            *(BYTE *) (0x57C104 + 1) = (BYTE) (color->z * 255.f);

            *(BYTE *) (0x57C199 + 1) = (BYTE) (color->x * 255.f);
            *(BYTE *) (0x57C193 + 1) = (BYTE) (color->y * 255.f);
            *(BYTE *) (0x57C191 + 1) = (BYTE) (color->z * 255.f);

            *(BYTE *) (0x57C208 + 1) = (BYTE) (color->x * 255.f);
            *(BYTE *) (0x57C1FF + 1) = (BYTE) (color->y * 255.f);
            *(BYTE *) (0x57C1FA + 1) = (BYTE) (color->z * 255.f);

            // Arrow
            *(BYTE *) (0x57A5CC + 1) = (BYTE) (color->x * 255.f);
            *(BYTE *) (0x57A5C0 + 1) = (BYTE) (color->y * 255.f);
            *(BYTE *) (0x57A5BB + 1) = (BYTE) (color->z * 255.f);

            // MAP
            *(BYTE *) (0x5865BC + 1) = (BYTE) (color->x * 255.f);
            *(BYTE *) (0x5865B7 + 1) = (BYTE) (color->y * 255.f);
            *(BYTE *) (0x5865B2 + 1) = (BYTE) (color->z * 255.f);

            *(BYTE *) (0x5865DA + 1) = (BYTE) (color->x * 255.f);
            *(BYTE *) (0x5865D5 + 1) = (BYTE) (color->y * 255.f);
            *(BYTE *) (0x5865D0 + 1) = (BYTE) (color->z * 255.f);

            *(BYTE *) (0x5865F8 + 1) = (BYTE) (color->x * 255.f);
            *(BYTE *) (0x5865F3 + 1) = (BYTE) (color->y * 255.f);
            *(BYTE *) (0x5865EE + 1) = (BYTE) (color->z * 255.f);

            *(BYTE *) (0x586616 + 1) = (BYTE) (color->x * 255.f);
            *(BYTE *) (0x586611 + 1) = (BYTE) (color->y * 255.f);
            *(BYTE *) (0x58660C + 1) = (BYTE) (color->z * 255.f);

            *(BYTE *) (0x5759A7 + 1) = (BYTE) (color->x * 255.f);
            *(BYTE *) (0x5759A2 + 1) = (BYTE) (color->y * 255.f);
            *(BYTE *) (0x575999 + 1) = (BYTE) (color->z * 255.f);
        }

        void HUD::col_weapon(ImVec4 *color) {
            *(BYTE *) (0x58D973 + 1) = (BYTE) (color->x * 255.f);
            *(BYTE *) (0x58D96E + 1) = (BYTE) (color->y * 255.f);
            *(BYTE *) (0x58D969 + 1) = (BYTE) (color->z * 255.f);

            *(BYTE *) (0x58D8AF + 1) = (BYTE) (color->x * 255.f);
            *(BYTE *) (0x58D8AA + 1) = (BYTE) (color->y * 255.f);
            *(BYTE *) (0x58D89F + 1) = (BYTE) (color->z * 255.f);
        }

        void HUD::col_gameInput(ImVec4 *color, DWORD color_x) {
            DWORD ChatAddr = *(DWORD *) (&samp::RefInputBox());
            DWORD ChatAddr2 = *(DWORD *) ((ChatAddr + 0x8));

            *(DWORD *) (ChatAddr2 + 0x127) = D3DCOLOR_ARGB(255, (BYTE)(color->x * 255.f), (BYTE)(color->y * 255.f),
                                                           (BYTE)(color->z * 255.f));

            *(DWORD *) (((&samp::RefInputBox()) + 0x8) + 0x127) = color_x;
        }

        void HUD::col_carLight(ImVec4 *color) {
            *(byte *) (0x6E155F + 1) = (BYTE) (color->x * 255.f);
            *(byte *) (0x6E155D + 1) = (BYTE) (color->y * 255.f);
            *(byte *) (0x6E1555 + 1) = (BYTE) (color->z * 255.f);

            *(byte *) (0x6E1688 + 1) = (BYTE) (color->x * 255.f);
            *(byte *) (0x6E1686 + 1) = (BYTE) (color->y * 255.f);
            *(byte *) (0x6E1684 + 1) = (BYTE) (color->z * 255.f);
        }

        void HUD::col_crossHair(ImVec4 *color) {
            *(byte *) (0x58E47C) = (BYTE) (color->x * 255.f);
            *(byte *) (0x58E473) = (BYTE) (color->y * 255.f);
            *(byte *) (0x58E466) = (BYTE) (color->z * 255.f);

            *(byte *) (0x58E432 + 1) = (BYTE) (color->x * 255.f);
            *(byte *) (0x58E429 + 1) = (BYTE) (color->y * 255.f);
            *(byte *) (0x58E424 + 1) = (BYTE) (color->z * 255.f);

            *(byte *) (0x58E3D9 + 1) = (BYTE) (color->x * 255.f);
            *(byte *) (0x58E3D0 + 1) = (BYTE) (color->y * 255.f);
            *(byte *) (0x58E3C7 + 1) = (BYTE) (color->z * 255.f);

            *(byte *) (0x58E300 + 1) = (BYTE) (color->x * 255.f);
            *(byte *) (0x58E2F5 + 1) = (BYTE) (color->y * 255.f);
            *(byte *) (0x58E2F0 + 1) = (BYTE) (color->z * 255.f);
        }

        void HUD::col_radar(ImVec4 *color) {
            *(BYTE *) (0x58A9A2) = (BYTE) (color->x * 255.f);
            *(BYTE *) (0x58A99A) = (BYTE) (color->y * 255.f);
            *(BYTE *) (0x58A996) = (BYTE) (color->x * 255.f);

            *(BYTE *) (0x58A8EE) = (BYTE) (color->x * 255.f);
            *(BYTE *) (0x58A8E6) = (BYTE) (color->y * 255.f);
            *(BYTE *) (0x58A8DE) = (BYTE) (color->x * 255.f);

            *(BYTE *) (0x58A89A) = (BYTE) (color->x * 255.f);
            *(BYTE *) (0x58A896) = (BYTE) (color->y * 255.f);
            *(BYTE *) (0x58A894) = (BYTE) (color->x * 255.f);

            *(BYTE *) (0x58A798) = (BYTE) (color->x * 255.f);
            *(BYTE *) (0x58A790) = (BYTE) (color->y * 255.f);
            *(BYTE *) (0x58A78E) = (BYTE) (color->x * 255.f);

            // Alea la avion
            *(BYTE *) (0x58A514) = (BYTE) (color->x * 255.f);
            *(BYTE *) (0x58A50D) = (BYTE) (color->y * 255.f);
            *(BYTE *) (0x58A506) = (BYTE) (color->x * 255.f);

            *(BYTE *) (0x58A753) = (BYTE) (color->x * 255.f);
            *(BYTE *) (0x58A74A) = (BYTE) (color->y * 255.f);
            *(BYTE *) (0x58A741) = (BYTE) (color->x * 255.f);
            // Centre
            *(BYTE *) (0x588690 + 1) = (BYTE) (color->x * 255.f);
            *(BYTE *) (0x58868B + 1) = (BYTE) (color->y * 255.f);
            *(BYTE *) (0x588686 + 1) = (BYTE) (color->x * 255.f);
        }

        void HUD::onProcessVisual() {
            auto cycler = Plugin::getInstance().getHueCycler();
            auto holder = cycler->currentRgb();
            ImVec4 color = bVisRainbow ? ImVec4(holder.r, holder.g, holder.b, 255.f) : vVisColor;
            DWORD color_x = dVisColorHex;
            if (bVisRainbow) {
                color_x = D3DCOLOR_RGBA(static_cast<int>(holder.r * 255), static_cast<int>(holder.g * 255), static_cast<int>(holder.b * 255), 255);
            }

            if (bVisHUD) col_hud(color_x);
            if (bVisTextdraw) col_textdraws(&color);
            if (bVisWeaponSlot) col_weapon(&color);
            if (bVisMenu) col_menu(&color);
            if (bVisInput) col_gameInput(&color, color_x);
            if (bVisCrossHair) col_crossHair(&color);
            if (bVisCarLight) col_carLight(&color);
            if (bVisRadar) col_radar(&color);
        }

        void HUD::onSave(nlohmann::json &j) {
            saveData(j, "bVisRainbow", bVisRainbow);
            saveData(j, "bVisHUD", bVisHUD);
            saveData(j, "bVisTextdraw", bVisTextdraw);
            saveData(j, "bVisWeaponSlot", bVisWeaponSlot);
            saveData(j, "bVisMenu", bVisMenu);
            saveData(j, "bVisInput", bVisInput);
            saveData(j, "bVisCarLight", bVisCarLight);
            saveData(j, "bVisCrossHair", bVisCrossHair);
            saveData(j, "bVisRadar", bVisRadar);
            saveData(j, "dVisColorHex", dVisColorHex);
        }

        void HUD::onLoad(const nlohmann::json &j) {
            loadData(j, "bVisRainbow", bVisRainbow);
            loadData(j, "bVisHUD", bVisHUD);
            loadData(j, "bVisTextdraw", bVisTextdraw);
            loadData(j, "bVisWeaponSlot", bVisWeaponSlot);
            loadData(j, "bVisMenu", bVisMenu);
            loadData(j, "bVisInput", bVisInput);
            loadData(j, "bVisCarLight", bVisCarLight);
            loadData(j, "bVisCrossHair", bVisCrossHair);
            loadData(j, "bVisRadar", bVisRadar);
            loadData(j, "dVisColorHex", dVisColorHex);
            vVisColor = D3DColorToImColor(dVisColorHex);
        }
    } // namespace Visual
} // namespace CheatModule

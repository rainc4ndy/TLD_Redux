#include "CMenu.h"

#include <common.h>

#include <D3dx9tex.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <imgui.h>
#include <imgui_impl_dx9.h>
#include <imgui_impl_win32.h>

#include <sampapi/CGame.h>
#include <sampapi/CNetGame.h>
#include <sampapi/CVehiclePool.h>

#include <initializer_list>
#include <string>

#include "Plugin.h"
#include "CCheatManager.h"
#include "CConfig.h"
#include "colors.h"
#include "CTexture.h"
#include "CTextureManager.h"
#include "HueCycler.h"
#include "default_style.h"
#include "encoding.h"
#include "samp.h"
#include "TargetSelector.h"
#include "Utils.h"
#include "imTypes.h"
#include "LicenseStuff.h"

#include "Cheats/Bot/CBotManager.h"
#include "Cheats/Vehicle/CarRespawner.h"



CMenu::CMenu(/* args */) {
    currentCategory = Category_Player;
    enabled = false;
}

CMenu::~CMenu() {
}

void CMenu::setEnable(bool enable) { enabled = enable; }
bool CMenu::isEnabled() { return enabled; }

void CMenu::enableCursor(bool enable) {
    samp::RefGame()->SetCursorMode(enable ? 3 : 0, !enable);
}

void CMenu::renderModuleCheckbox(CCheatModule *module) {
    bool tmp = module->isEnabled();
    if (ImGui::Checkbox(module->getName().c_str(), &tmp)) {
        module->setEnabled(tmp);
    }
}

void CMenu::renderGoc() {
    auto config = Plugin::getInstance().getConfig();
    static const char *items[] = {"车内", "车内 & 步行", "空车", "拖车 (适合远距离射车)", "自己车内 (慎用!)", "仅本地"};
    ImGui::Combo("同步模式", &config->gocMode, items, IM_ARRAYSIZE(items));
    if (ImGui::InputScalar("延迟 (全局)", ImGuiDataType_U32, &config->gocDelay, &u32_one, &u32_fifty, "%u")) {
        auto r = CCheatManager::getAll();
        for (auto &it: r) {
            if (auto goc = dynamic_cast<CheatModule::Vehicle::GocModule *>(it)) {
                goc->delay = config->gocDelay;
            }
        }
    }
    if (ImGui::InputFloat("速度 (全局)", &config->gocSpeed)) {
        auto r = CCheatManager::getAll();
        for (auto &it: r) {
            if (auto goc = dynamic_cast<CheatModule::Vehicle::GocModule *>(it)) {
                goc->speed = config->gocSpeed;
            }
        }
    }
    if (ImGui::InputFloat("范围 (全局)", &config->gocRange)) {
    }

    ImGui::InputInt("特定载具", &config->gocVehicle);
    if (ImGui::Button("设置为附近最近的载具")) {
        config->gocVehicle = samp::RefNetGame()->GetVehiclePool()->GetNearest();
    }
    ImGui::SameLine();
    if (ImGui::Button("清除")) {
        config->gocVehicle = -1;
    }

    ImGui::Checkbox("同步的时候修复载具", &config->gocFix);
    if (config->gocSpeed < 0.f) config->gocSpeed = 0.f;
}

void CMenu::RenderSidebarButtons(std::initializer_list<std::string> v) {
    ImGuiStyle *style = &ImGui::GetStyle();

    float length = ImGui::GetWindowSize().x - (style->WindowPadding.x * 2) - (style->FramePadding.x * 2);
    ImVec2 sz(length, length);

    int i = 0;
    for (auto &it: v) {
        auto tex = Plugin::getInstance().getTexs()->getTexture(it);
        ImTextureID textureID = reinterpret_cast<ImTextureID>(tex->getTexture());
        // bool changed = false;
        // if (currentCategory == i) {
        //     static auto col = ImVec4(ImColor(165,23,69));
        //     ImGui::PushStyleColor(ImGuiCol_Button, col);
        //     ImGui::PushStyleColor(ImGuiCol_ButtonActive, col);
        //     ImGui::PushStyleColor(ImGuiCol_ButtonHovered, col);
        //     changed = true;
        // }
        //if (ImGui::Button(it.c_str(), sz)) {
        if (ImGui::ImageButton(it.c_str(), textureID, sz)) {
            currentCategory = i;
        }
        // if (changed) {
        //     ImGui::PopStyleColor(3);
        // }
        i++;
    }
}

void DrawRainbowBar(float height) {
    auto cycler = Plugin::getInstance().getHueCycler();
    ImDrawList *drawList = ImGui::GetWindowDrawList();
    ImGuiStyle *style = &ImGui::GetStyle();

    auto sz = ImGui::GetWindowSize();
    auto pos = ImGui::GetCursorScreenPos();
    auto c = cycler->currentRgb();
    // Draw each band
    drawList->AddRectFilled(
        ImVec2(pos.x - style->WindowPadding.x, pos.y - style->WindowPadding.x),
        ImVec2(pos.x + sz.x, pos.y + height),
        ImColor(ImVec4(c.r, c.g, c.b, 1.0f)));

    ImGui::SetCursorScreenPos(
        ImVec2(pos.x, pos.y + height + style->WindowPadding.y));
}

bool style_editor = false;
void CMenu::render() {
    auto drawlist = ImGui::GetBackgroundDrawList();
    auto tex = Plugin::getInstance().getTexs();

    static bool menu_open = true;
    ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(450, 250));
    ImGui::SetNextWindowBgAlpha(menuTransparency);
    ImGui::Begin(("The Last Dancer - Redux " + Plugin::CurrentVersion).c_str(), &menu_open,
                 ImGuiWindowFlags_NoScrollbar |
                 ImGuiWindowFlags_NoScrollWithMouse);
    ImGui::PopStyleVar();

    backgroundImage.draw();

    const int min_size_h1 = 64;

    ImGuiStyle *style = &ImGui::GetStyle();
    auto window_size = ImGui::GetWindowSize();

    float area_y = window_size.y - style->FramePadding.y * 2 - style->WindowPadding.y * 4;
    ImVec2 button_area(80, area_y);
    ImVec2 main_area(
        window_size.x - button_area.x - style->FramePadding.x * 2 - style->WindowPadding.x * 2,
        area_y
    );
    //sz.x -= style->WindowPadding.x * 2 + style->FramePadding.x;
    //sz.y -= style->WindowPadding.y * 2 + style->FramePadding.y;

    ImGui::BeginChild("horizon_1", button_area, ImGuiChildFlags_Border, ImGuiWindowFlags_NoScrollbar);
    RenderSidebarButtons({
        "player.png", "vehicle.png", "weapon.png", "target.png",
        "visual.png", "hack.png", "bot.png", "settings.png"
    });
    ImGui::EndChild();

    static const char *titles[] = {
        "玩家模块", "车辆模块", "武器模块", "目标模块",
        "显示模块", "Hacking 模块", "Bot模块", "设置"
    };

    ImGui::SameLine();
    ImGui::BeginChild(titles[currentCategory], main_area, ImGuiChildFlags_Border,ImGuiWindowFlags_NoScrollbar);

    ImGui::SeparatorText(titles[currentCategory]);

    auto cheat = Plugin::getInstance().getCheat();
    switch (currentCategory) {
        case Category_Player: {
            renderModuleCheckbox(&cheat->invis);
            ImGui::SameLine(); {
                const char *items[] = {
                    "坐标", "观察模式", "遥控器bug", "车辆",
                    "OBJ"
                };
                ImGui::Combo("模式##1", &cheat->invis.mode, items,
                             IM_ARRAYSIZE(items));
            }

            renderModuleCheckbox(&cheat->twist);
            ImGui::SameLine(); {
                const char *items[] = {"扭曲全身", "MJ走路", "倒立", "旋转"};
                ImGui::Combo("模式##2", &cheat->twist.mode, items,
                             IM_ARRAYSIZE(items));
            }
            ImGui::Text("装逼的功能只有别人可以看到");

            renderModuleCheckbox(&cheat->god);
            ImGui::SameLine();
            renderModuleCheckbox(&cheat->unmoving);

            renderModuleCheckbox(&cheat->fastwalk);
            ImGui::SameLine();
            ImGui::SliderFloat("速度", &cheat->fastwalk.speed, 1.0f, 16.0f, "%0.3f");

            renderModuleCheckbox(&cheat->fastSpawn);
            renderModuleCheckbox(&cheat->randomArmor);
            ImGui::SameLine();
            renderModuleCheckbox(&cheat->randomHealth);
            renderModuleCheckbox(&cheat->noCameraCol);
            renderModuleCheckbox(&cheat->infiniteRun);
            renderModuleCheckbox(&cheat->antiFall);
            renderModuleCheckbox(&cheat->longHP);

            break;
        }
        case Category_Vehicle: {
            if (ImGui::CollapsingHeader("R人类")) {
                std::initializer_list<CheatModule::Vehicle::GocModule *> gocs = {
                    &cheat->carShooter,
                    &cheat->carLagger,
                    &cheat->carStorm,
                    &cheat->carWall
                };

                for (auto it: gocs) {
                    ImGui::Columns(3);
                    renderModuleCheckbox(it);
                    ImGui::NextColumn();
                    ImGui::Text("延迟");
                    ImGui::SameLine();
                    ImGui::InputScalar(("##delay" + it->getNameEn()).c_str(), ImGuiDataType_U32, &it->delay, &u32_one,
                                       &u32_fifty, "%u");
                    ImGui::NextColumn();
                    ImGui::Text("速度");
                    ImGui::SameLine();
                    ImGui::InputFloat(("##speed" + it->getNameEn()).c_str(), &it->speed);
                    ImGui::Columns(1);
                }

                renderGoc();
                ImGui::Separator();
            }
            renderModuleCheckbox(&cheat->doorStorm);
            renderModuleCheckbox(&cheat->steathDriver);
            renderModuleCheckbox(&cheat->allVanish);

            ImGui::SameLine();
            if (ImGui::Button("强制刷新附近车辆")) {
                CheatModule::Vehicle::CarRespawner::hack();
            }

            renderModuleCheckbox(&cheat->supremeDashing);
            ImGui::SameLine();
            ImGui::SliderFloat("速度##2", &cheat->supremeDashing.speed, 1.0f, 50.0f, "%0.3f");

            renderModuleCheckbox(&cheat->masterDriver);
            break;
        }
        case Category_Weapon: {
            renderModuleCheckbox(&cheat->fastShot);
            ImGui::InputFloat("速度", &cheat->fastShot.speed, 0.1f, 0.3f, "%0.1f");

            renderModuleCheckbox(&cheat->noReload);
            renderModuleCheckbox(&cheat->noSpread);
            renderModuleCheckbox(&cheat->npcKiller);
            ImGui::SameLine();
            if (ImGui::Button("杀死附近所有NPC")) {
                cheat->npcKiller.killAll();
            }

            renderModuleCheckbox(&cheat->triggerBot);
            ImGui::InputFloat("距离", &cheat->triggerBot.range, 0.5f, 1.5f, "%0.1f");

            break;
        }
        case Category_Target: {
            auto target = Plugin::getInstance().getTarget();

            static int last_target = 0x3f3f3f3f;
            static char szTargetName[SAMP_MAX_PLAYER_NAME * 4];
            if (last_target != target->targetPlayerID) {
                if (target->targetPlayerID == -1) {
                    strcpy(szTargetName, "不可用");
                    last_target = target->targetPlayerID;
                } else {
                    char *unicoded = EncodingUtil::g2u(
                        samp::RefNetGame()->GetPlayerPool()->GetAt(target->targetPlayerID)->m_szNick.c_str());
                    strcpy(szTargetName, unicoded);
                    delete[] unicoded;
                    last_target = target->targetPlayerID;
                }
            }
            ImGui::Text("当前目标: %s", szTargetName);

            if (ImGui::CollapsingHeader("目标策略")) {
                static const char *items[] = {"最近的", "随机的", "全服随机", "固定"};
                ImGui::Combo("选择策略", &target->mode, items, IM_ARRAYSIZE(items));

                if (ImGui::InputInt("固定目标 (ID)", &target->specifiedPlayerID, 1, 1,
                                    ImGuiInputTextFlags_EnterReturnsTrue)) {
                    if (target->specifiedPlayerID < 0)
                        target->specifiedPlayerID = 0;
                    if (target->specifiedPlayerID >= SAMP_MAX_PLAYERS)
                        target->specifiedPlayerID = SAMP_MAX_PLAYERS - 1;
                }

                ImGui::Checkbox("包含NPC", &target->NPCIncluded);
                ImGui::SameLine();
                ImGui::InputScalar("延迟##2", ImGuiDataType_U32, &target->delay, &u32_one, &u32_fifty, "%u");
            }

            if (ImGui::CollapsingHeader("R人类")) {
                std::initializer_list<CheatModule::Vehicle::GocModule *> gocs = {
                    &cheat->smasher,
                    &cheat->elevator,
                    &cheat->deathCross,
                    &cheat->deathTornado
                };

                for (auto it: gocs) {
                    ImGui::Columns(3);
                    renderModuleCheckbox(it);
                    ImGui::NextColumn();
                    ImGui::Text("延迟");
                    ImGui::SameLine();
                    ImGui::InputScalar(("##delay" + it->getNameEn()).c_str(), ImGuiDataType_U32, &it->delay, &u32_one,
                                       &u32_fifty, "%u");
                    ImGui::NextColumn();
                    ImGui::Text("速度");
                    ImGui::SameLine();
                    ImGui::InputFloat(("##speed" + it->getNameEn()).c_str(), &it->speed);
                    ImGui::Columns(1);
                }
                renderGoc();
                ImGui::Separator();
            }

            renderModuleCheckbox(&cheat->targetRenderer);
            renderModuleCheckbox(&cheat->mimic);
            static const char *oim[] = {"前面", "后面", "左边", "右边"};
            ImGui::Columns(2);
            ImGui::Combo("位置偏移", &cheat->mimic.offset_mode, oim, IM_ARRAYSIZE(oim));
            ImGui::NextColumn();
            ImGui::Checkbox("在本地也应用位置", &cheat->mimic.local);
            ImGui::Columns(1);

            renderModuleCheckbox(&cheat->darkStab);
            ImGui::Columns(2);
            ImGui::InputFloat("有效距离", &cheat->darkStab.range); ImGui::SameLine();
            ImGui::NextColumn();
            ImGui::Checkbox("需要手里拿刀", &cheat->darkStab.use_own_weapon);
            ImGui::Columns(1);
            break;
        }
        case Category_Visual: {
            if (ImGui::CollapsingHeader("HUD颜色")) {
                renderModuleCheckbox(&cheat->hud);

                ImGui::Checkbox("HUD", &cheat->hud.bVisHUD);
                ImGui::Checkbox("武器栏", &cheat->hud.bVisWeaponSlot);
                ImGui::Checkbox("文字TD", &cheat->hud.bVisTextdraw);
                ImGui::Checkbox("界面", &cheat->hud.bVisMenu);
                ImGui::Checkbox("打字输入", &cheat->hud.bVisInput);
                ImGui::Checkbox("准星", &cheat->hud.bVisCrossHair);
                ImGui::Checkbox("车灯", &cheat->hud.bVisCarLight);
                ImGui::Checkbox("雷达", &cheat->hud.bVisRadar);

                ImGui::Checkbox("使用彩虹渐变", &cheat->hud.bVisRainbow);
                if (ImGui::ColorEdit4("自定义颜色##2", (float *) &cheat->hud.vVisColor, ImGuiColorEditFlags_DisplayRGB)) {
                    cheat->hud.dVisColorHex = ImColorToD3DColor(cheat->hud.vVisColor);
                }
            }
            if (ImGui::CollapsingHeader("透视")) {
                renderModuleCheckbox(&cheat->playerESP);
                if (ImGui::TreeNode("详细设置")) {
                    ImGui::Checkbox("方框", &cheat->playerESP.box);
                    ImGui::SameLine();
                    ImGui::Checkbox("骨骼", &cheat->playerESP.bone);
                    ImGui::Checkbox("线条", &cheat->playerESP.line);
                    ImGui::SameLine();
                    ImGui::Checkbox("信息", &cheat->playerESP.info);

                    ImGui::Checkbox("彩虹渐变", &cheat->playerESP.rainbow);
                    if (ImGui::ColorEdit4("自定义颜色##3", (float *) &cheat->playerESP.color_cache, ImGuiColorEditFlags_DisplayRGB)) {
                        cheat->playerESP.color = ImColorToD3DColor(cheat->playerESP.color_cache);
                    }
                    ImGui::SliderFloat("线条粗细", &cheat->playerESP.thickness, 1.0f, 5.0f, "%0.3f");

                    ImGui::TreePop();
                }
            }
            renderModuleCheckbox(&cheat->timeWeatherHack);
            ImGui::SliderInt("时间", &cheat->timeWeatherHack.time, 0, 23); // Slider for selecting hour in range [0, 23]
            ImGui::InputInt("天气", &cheat->timeWeatherHack.weather);

            renderModuleCheckbox(&cheat->cheat_list);
            renderModuleCheckbox(&cheat->console);
            ImGui::SameLine();
            ImGui::Checkbox("总是显示", &cheat->console.always_on_top);

            renderModuleCheckbox(&cheat->fovChanger);
            ImGui::SameLine();
            ImGui::SliderFloat("##fov", &cheat->fovChanger.fov, 50, 150); // Slider for selecting hour in range [0, 23]

            break;
        }
        case Category_Hack: {
            if (ImGui::CollapsingHeader("对话框Hack")) {
                renderModuleCheckbox(&cheat->NoDialog);
                renderModuleCheckbox(&cheat->dialogSpam);
                if (ImGui::TreeNode("刷对话框 - 选项")) {
                    ImGui::Checkbox("右边按钮", &cheat->dialogSpam.r_button);
                    ImGui::InputText("内容", cheat->dialogSpam.content, sizeof(cheat->dialogSpam.content));
                    ImGui::InputInt("对话框选项", &cheat->dialogSpam.item);
                    ImGui::InputScalar("延迟##3", ImGuiDataType_U32, &cheat->dialogSpam.delay, &u32_one, &u32_fifty,
                                       "%u");
                    ImGui::TreePop();
                }
            }
            if (ImGui::CollapsingHeader("自由城自动刷僵尸")) {
                renderModuleCheckbox(&cheat->zombieAutomata);
                ImGui::SameLine();
                {
                    static const char *combos[] = {"只攻击僵尸", "只攻击Boss", "全屏攻击"};
                    ImGui::Combo("选择策略", &cheat->zombieAutomata.pattern, combos, IM_ARRAYSIZE(combos));
                }
                ImGui::SliderFloat("频率系数", &cheat->zombieAutomata.rate_factor, 1.0f, 6.0f, "%0.3f");

                ImGui::Columns(3);
                ImGui::Checkbox("强化效果", &cheat->zombieAutomata.override_damage);
                ImGui::NextColumn();
                ImGui::Checkbox("自动吸钱", &cheat->zombieAutomata.money_hack);
                ImGui::NextColumn();
                ImGui::Checkbox("使用隐形模式", &cheat->zombieAutomata.use_invis);
                ImGui::Columns(1);
                if (ImGui::TreeNode("自动拿家具")) {
                    ImGui::Checkbox("开启", &cheat->zombieAutomata.furniture_stash);

                    float vec[3] = {
                        cheat->zombieAutomata.stash_position.x,
                        cheat->zombieAutomata.stash_position.y,
                        cheat->zombieAutomata.stash_position.z
                    };
                    ImGui::InputFloat3("要放置的位置", vec);
                    if (ImGui::Button("设置为目前位置")) {
                        cheat->zombieAutomata.setCurrentStashPosition();
                    }
                    ImGui::TreePop();
                }
            }
            renderModuleCheckbox(&cheat->captchaSolver);
            ImGui::SameLine();
            ImGui::Text("缺陷: 双开时，只有前一个玩家可以过检测");
            renderModuleCheckbox(&cheat->autoLogin); ImGui::SameLine();
            ImGui::Text("密码自己在config设置");

            renderModuleCheckbox(&cheat->fakeMobile);
            renderModuleCheckbox(&cheat->rpcAnalysis);
            break;
        }
        case Category_Bot: {
            auto bot = Plugin::getInstance().getBotManager();
            bot->renderBotManager();
            ImGui::Separator();

            break;
        }
        case Category_Setting: {
            if (ImGui::TreeNode("背景图片")) {
                ImGui::SliderFloat("缩放", &backgroundImage.zoom, 0.1f, 3.0f);
                ImGui::SliderFloat("中心 - X", &backgroundImage.center_x, -1000.0f, 1000.0f);
                ImGui::SliderFloat("中心 - Y", &backgroundImage.center_y, -1000.0f, 1000.0f);
                ImGui::SliderFloat("不透明度", &backgroundImage.transparency, 0.0f, 1.0f);
                ImGui::SliderFloat("窗体不透明度", &menuTransparency, 0.0f, 1.0f);
                ImGui::TreePop();
            }

            auto config = Plugin::getInstance().getConfig();
            static const char *items[] = {"黑暗", "亮色", "恶魔", "糖果", "初音", "丛雨"};
            if (ImGui::Combo("主题颜色", &config->color_theme, items, IM_ARRAYSIZE(items))) {
                SetupImGuiStyle(config->color_theme);
            }
            ImGui::Checkbox("样式编辑器", &style_editor);

            if (ImGui::Button("保存当前的TLD设置")) {
               config->save(
                    Plugin::getInstance().getDirectory() + PLUGIN_CONFIG_FILE);
            }
            ImGui::SameLine();
            if (ImGui::Button("检查更新")) {
                Plugin::getInstance().getLicenseChecker()->GetUpdate();
            }

            ImGui::Bullet();
            ImGui::Text("字体设置需要自己去 config.json 改");
            break;
        }
    }

    ImGui::EndChild();
    ImGui::End();

    if (style_editor) {
        ImGui::ShowStyleEditor();
    }
}

void CMenu::render_menu_bar() {
    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("Menu")) {
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Examples")) {
            ImGui::SeparatorText("Mini apps");

            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Tools")) {
            ImGui::Separator();
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
}

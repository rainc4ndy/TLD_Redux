#include <imgui.h>

#include "CBotManager.h"
#include "CBot.h"
#include "../../CMenu.h"
#include "../../CCheatManager.h"
#include "CNicknameGenerator.h"
#include "CProxyManager.h"
#include "encoding.h"
#include "imTypes.h"
#include "Plugin.h"
#include "Utils.h"
#include "imgui_stdlib.h"


namespace CheatModule::Bot {
    CBotManager::CBotManager() : CCheatModule("机器人管理", "Bot Manager") {
        resetProxyInfo();
        CBot::setManager(this);
        largest_botID =0;
    }

    CBotManager::~CBotManager() {
        //clear();
    }

    void CBotManager::resetProxyInfo() {
        memset(menu_proxy_host, 0, sizeof(menu_proxy_host));
        menu_proxy_port = 10808;
        strcpy(menu_proxy_host, "127.0.0.1");
        memset(menu_proxy_user, 0, sizeof(menu_proxy_user));
        memset(menu_proxy_pass, 0, sizeof(menu_proxy_pass));
    }

    void CBotManager::onSave(nlohmann::json &j) {
        saveData(j, "name_strategy", name_strategy);
        saveData(j, "name_len", name_len);
        saveData(j, "suffix_len", suffix_len);
        saveData(j, "bot_cnt_adder", bot_cnt_adder);
        saveData(j, "proxy_mode", proxy_mode);
        saveData(j, "auto_recon", auto_recon);
        saveData(j, "auto_disconnect", auto_disconnect);
        saveData(j, "fake_spawn", fake_spawn);
        saveData(j, "recon_delay", recon_delay);
        saveData(j, "auto_login", auto_login);
        saveData(j, "bot_password", bot_password);
        saveData(j, "fake_mobile", fake_mobile);
        saveData(j, "use_weapon", use_weapon);
        saveData(j, "bot_weapon", bot_weapon);
    }

    void CBotManager::onLoad(const nlohmann::json &j) {
        loadData(j, "name_strategy", name_strategy);
        loadData(j, "name_len", name_len);
        loadData(j, "suffix_len", suffix_len);
        loadData(j, "bot_cnt_adder", bot_cnt_adder);
        loadData(j, "proxy_mode", proxy_mode);
        loadData(j, "auto_recon", auto_recon);
        loadData(j, "auto_disconnect", auto_disconnect);
        loadData(j, "fake_spawn", fake_spawn);
        loadData(j, "recon_delay", recon_delay);
        loadData(j, "auto_login", auto_login);
        loadData(j, "bot_password", bot_password);
        loadData(j, "fake_mobile", fake_mobile);
        loadData(j, "use_weapon", use_weapon);
        loadData(j, "bot_weapon", bot_weapon);
    }

    CBot *CBotManager::create(std::string name) {
        if (name.size() < 3 || name.size() >= SAMP_MAX_PLAYER_NAME) {
            CUtils::errorToConsole("无法创建机器人 %s", name.c_str());
            return nullptr;
        }
        for (int i = 0; i < MAX_BOT; ++i) {
            if (!pool[i]) {
                // 如果该位置没有机器人
                pool[i] = std::make_unique<CBot>(name); // 创建新的机器人
                occupied_name.insert(name);
                // 更新 largest_botID
                if (i > largest_botID) {
                    largest_botID = i;
                }
                return pool[i].get(); // 返回新创建机器人的指针
            }
        }
        return nullptr; // 如果池已满，返回 nullptr
    }

    // 删除指定 ID 的机器人
    void CBotManager::remove(int botID) {
        if (botID >= 0 && botID < MAX_BOT && pool[botID]) {
            // 确保 botID 有效且存在机器人
            occupied_name.erase(pool[botID]->getName());
            pool[botID]->disconnect();
            pool[botID].reset(); // 删除机器人
            if (botID == largest_botID) {
                // 更新 largest_botID 为当前最大存在的机器人 ID
                while (largest_botID >= 0 && !pool[largest_botID]) {
                    --largest_botID;
                }
            }
            bot_selection.find_erase_unsorted(botID);
        }
    }

    // 处理机器人接收数据包和同步作弊
    void CBotManager::process() {
        for (int i = 0; i <= largest_botID; ++i) {
            if (pool[i]) {
                pool[i]->process();
            }
        }
    }

    void CBotManager::clear() {
        bot_selection.clear();
        for (int i = 0; i <= largest_botID; ++i) {
            if (pool[i]) {
                remove(i);
            }
        }
    }

    std::vector<CBot *> CBotManager::getAllBot() {
        std::vector<CBot *> result;
        for (int i = 0; i <= largest_botID; ++i) {
            if (pool[i]) {
                result.push_back(pool[i].get());
            }
        }
        return std::move(result);
    }

    void CBotManager::spawnAll() {
        for (int i = 0; i <= largest_botID; ++i) {
            if (pool[i]) {
                if (pool[i]->isConnected()) {
                    pool[i]->sendSpawnRequest();
                    pool[i]->sendSpawn();
                }
            }
        }
    }

    void CBotManager::clearAll() {
    }

    bool CBotManager::isNicknameAvailable(std::string nickname) {
        return occupied_name.find(nickname) == occupied_name.end();
    }

    std::string CBotManager::gen_name(int strategy, int name_len, int name_suffix_len) {
        using namespace CheatModule::Bot;
        switch (strategy) {
            case 0:
                return CNicknameGenerator::FromRandomBinary(name_len);
            case 1:
                return CNicknameGenerator::FromRandomASCII(name_len);
            case 2:
                return CNicknameGenerator::FromSelf(name_suffix_len);
            case 3:
                return CNicknameGenerator::FromOther(name_suffix_len);
            case 4:
                return CNicknameGenerator::FromList();
            case 5:
                return CNicknameGenerator::FromList() + "_" + CNicknameGenerator::FromRandomASCII(2);
            default:
                return std::string("Unknown");
        }
    };

    void CBotManager::renderBotManager() {
        // Proxy list section (same as before)
        if (ImGui::TreeNode("Socks5 代理列表")) {
            ImGui::Bullet();
            ImGui::Text("代理存放在配置文件夹下");
            static ImGuiTableFlags flags = ImGuiTableFlags_Resizable | ImGuiTableFlags_Hideable |
                                           ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV;
            if (ImGui::BeginTable("proxy##2", 3, flags, ImVec2(0.0f, 0.0f))) {
                ImGui::TableSetupColumn("服务器地址");
                ImGui::TableSetupColumn("端口");
                ImGui::TableSetupColumn("是否需要认证");
                ImGui::TableHeadersRow();
                const auto &all = Plugin::getInstance().getProxyManager()->getAll();
                for (const auto &p: all) {
                    auto proxyInfo = p.proxyInfo;
                    auto authInfo = p.authInfo;
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text(proxyInfo.first.c_str());
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text(std::to_string(proxyInfo.second).c_str());
                    ImGui::TableSetColumnIndex(2);
                    ImGui::Text(authInfo.first.empty() || authInfo.second.empty() ? "否" : "是");
                }
                ImGui::EndTable();
            }
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("固定代理设置")) {
            if (ImGui::BeginTable("##or", 2)) {
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::Text("代理");
                ImGui::SameLine();
                ImGui::InputText("##host", menu_proxy_host, sizeof(menu_proxy_host));
                ImGui::TableNextColumn();
                ImGui::Text("端口");
                ImGui::SameLine();
                ImGui::InputScalar("##port", ImGuiDataType_U32, &menu_proxy_port, NULL, NULL, "%u",
                                   ImGuiInputTextFlags_None);

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::Text("用户");
                ImGui::SameLine();
                ImGui::InputText("##user", menu_proxy_user, sizeof(menu_proxy_user));
                ImGui::TableNextColumn();
                ImGui::Text("密码");
                ImGui::SameLine();
                ImGui::InputText("##pass", menu_proxy_pass, sizeof(menu_proxy_pass), ImGuiInputTextFlags_Password);
                ImGui::EndTable();
            }
            if (ImGui::Button("清除")) {
                resetProxyInfo();
            }
            ImGui::TreePop();
        } {
            if (ImGui::TreeNode("名称设置")) {
                static const char *combos[] = {"随机二进制", "随机ASCII", "用我的名字", "用其他人的名字", "从文件中读取", "从文件中随机读取"};
                ImGui::Combo("名称策略", &name_strategy, combos, IM_ARRAYSIZE(combos));
                if (ImGui::InputScalar("名字长度", ImGuiDataType_U32, &name_len, &u32_one, NULL, "%u",
                                       ImGuiInputTextFlags_None)) {
                    if (name_len < 3) name_len = 3;
                    if (name_len > SAMP_MAX_PLAYER_NAME - suffix_len) name_len = SAMP_MAX_PLAYER_NAME - suffix_len;
                }
                if (ImGui::InputScalar("名字后缀长度", ImGuiDataType_U32, &suffix_len, &u32_one, NULL, "%u",
                                       ImGuiInputTextFlags_None)) {
                    if (suffix_len > SAMP_MAX_PLAYER_NAME - name_len) suffix_len = SAMP_MAX_PLAYER_NAME - name_len;
                }
                ImGui::TreePop();
            }
        } {
            static const char *combos[] = {"不使用代理", "固定代理", "自动代理", "自动代理 (严格模式)"};
            ImGui::Combo("代理模式", &proxy_mode, combos, IM_ARRAYSIZE(combos));
        }

        auto clicked = ImGui::Button("连接");
        ImGui::SameLine();
        ImGui::InputScalar("个数", ImGuiDataType_U32, &bot_cnt_adder, &u32_one, nullptr, "%u", ImGuiInputTextFlags_None);
        // Connect multiple bots
        if (clicked) {
            for (int i = 0; i < bot_cnt_adder; ++i) {
                const std::string botName = gen_name(name_strategy, name_len, suffix_len);
                if (botName.size() == 0 || !isNicknameAvailable(botName)) {
                    CUtils::errorToConsole("Name %s is not available", botName.c_str());
                    continue;
                }
                const auto proxyManager = Plugin::getInstance().getProxyManager();
                if (proxy_mode == PM_SINGLE) {
                    auto bot = create(botName);
                    if (strlen(menu_proxy_user) > 0 && strlen(menu_proxy_pass) > 0)
                        bot->setupProxy(menu_proxy_host, menu_proxy_port, menu_proxy_user, menu_proxy_pass);
                    else
                        bot->setupProxy(menu_proxy_host, menu_proxy_port);
                    bot->connect();
                } else if (proxy_mode == PM_AUTO || proxy_mode == PM_AUTO_STRICT) {
                    ProxyData *proxy = PM_AUTO_STRICT
                                       ? proxyManager->getNextAvailableProxy()
                                       : proxyManager->getNextProxy();
                    if (proxy) {
                        auto bot = create(botName);
                        if (proxy->authInfo.first.size() > 0 && proxy->authInfo.second.size() > 0) {
                            bot->setupProxy(proxy->proxyInfo.first, proxy->proxyInfo.second,
                                            proxy->authInfo.first, proxy->authInfo.second);
                        } else {
                            bot->setupProxy(proxy->proxyInfo.first, proxy->proxyInfo.second);
                        }
                        proxy->refCount++;
                        bot->connect();
                    } else {
                        CUtils::logToConsole("Failed to add bot %s because no proxy is available", botName.c_str());
                    }
                } else {
                    auto bot = create(botName);
                    bot->connect();
                }
            }
        }

        ImGui::SeparatorText("机器人列表");
        static ImGuiTableFlags botListFlags = ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg |
                                              ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV |
                                              ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable |
                                              ImGuiTableFlags_Hideable;
        // Track selected bot
        if (ImGui::BeginTable("机器人列表##2", 5, botListFlags, ImVec2(0.0f, ImGui::GetTextLineHeightWithSpacing() * 10))) {
            ImGui::TableSetupColumn("ID");
            ImGui::TableSetupColumn("玩家ID");
            ImGui::TableSetupColumn("名字");
            ImGui::TableSetupColumn("状态");
            ImGui::TableSetupColumn("代理");
            ImGui::TableHeadersRow();

            for (int i = 0; i <= largest_botID; i++) {
                auto bot = pool[i].get();
                if (bot) {
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    const bool item_is_selected = bot_selection.contains(i);
                    ImGuiSelectableFlags selectable_flags =
                            ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowOverlap;
                    if (ImGui::Selectable(std::to_string(i).c_str(), item_is_selected, selectable_flags,
                                          ImVec2(0, 0))) {
                        if (ImGui::GetIO().KeyCtrl) {
                            if (item_is_selected)
                                bot_selection.find_erase_unsorted(i);
                            else
                                bot_selection.push_back(i);
                        } else {
                            bot_selection.clear();
                            bot_selection.push_back(i);
                        }
                    }
                    if (ImGui::BeginPopupContextItem()) // <-- use last item id as popup id
                    {
                        if (!item_is_selected) {
                            bot_selection.clear();
                            bot_selection.push_back(i);
                        }
                        renderBotContextMenu(bot, i);
                        ImGui::EndPopup();
                    }

                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", bot->getPlayerID());
                    ImGui::TableSetColumnIndex(2);
                    auto content_converted = std::unique_ptr<char[]>(EncodingUtil::g2u(bot->getName().c_str()));
                    ImGui::Text("%s", content_converted.get());
                    ImGui::TableSetColumnIndex(3);
                    switch (bot->getStatus()) {
                        case CBot::DISCONNECTED:
                            ImGui::TextColored(ImColor(ImVec4(1, 0, 0, 1)), "已断开");
                            break;
                        case CBot::CONNECTING:
                            ImGui::Text("尝试连接中");
                            break;
                        case CBot::CONNECTED:
                            ImGui::Text("已连接");
                            break;
                        case CBot::SPAWNED:
                            ImGui::Text("工作中");
                            break;
                        case CBot::WAIT_FOR_JOIN:
                            ImGui::Text("加入游戏中");
                            break;
                    }
                    ImGui::TableSetColumnIndex(4);
                    if (bot->isUsingProxy()) {
                        ImGui::TextColored(ImColor(ImVec4(0, 1, 0, 1)), "启用");
                    } else {
                        ImGui::TextColored(ImColor(ImVec4(1, 0, 0, 1)), "禁用");
                    }
                }
            }
            ImGui::EndTable();
            if (ImGui::Button("重生全部机器人"))
                spawnAll();
            ImGui::SameLine();
            if (ImGui::Button("删掉全部机器人"))
                clear();
            ImGui::SameLine();
            ImGui::TextColored(ImColor(ImVec4(1, 0, 0, 1)), "(退出游戏前记得,不然卡)");

            ImGui::Checkbox("自动重连", &auto_recon);
            ImGui::SameLine();
            ImGui::InputScalar("重连延迟", ImGuiDataType_U32, &recon_delay, &u32_fifty, NULL, "%u",
                               ImGuiInputTextFlags_None);

            if (ImGui::TreeNode("试图炸服")) {
                ImGui::Checkbox("进服后马上断开", &auto_disconnect);
                ImGui::Checkbox("自动换名字", &auto_name);
                ImGui::TreePop();
            }

            if (ImGui::TreeNode("登录选项")) {
                ImGui::Checkbox("自动输入密码", &auto_login);
                ImGui::SameLine();
                ImGui::InputText("Bot的密码", &bot_password, ImGuiInputTextFlags_Password);
                ImGui::Checkbox("模拟手机端 (建议在自由城开启)", &fake_mobile);
                ImGui::Checkbox("假Spawn", &fake_spawn);
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("集群控制")) {
                auto follower = &Plugin::getInstance().getCheat()->botFollow;
                Plugin::getInstance().getMenu()->renderModuleCheckbox(follower);
                {
                    static const char *combos[] = {"直线", "环状"};
                    ImGui::Combo("跟随模式", &follower->followStrategy, combos, IM_ARRAYSIZE(combos));
                    ImGui::SliderFloat("距离", &follower->follow_range, 1.0f, 30.f);
                }

                ImGui::Checkbox("跟着我一起说话", &follower->chat_with_me);
                ImGui::SameLine();
                ImGui::Checkbox("跟着我一起开车 (最近载具)", &follower->drive_with_me);

                ImGui::Checkbox("跟着我一起瞄准", &follower->aim_with_me);
                ImGui::SameLine();
                ImGui::Checkbox("跟着我一起射击", &follower->shoot_with_me);
                ImGui::Checkbox("复制我所有的RPC数据包", &follower->all_rpc_follow);

                ImGui::Checkbox("拿武器", &use_weapon);
                ImGui::SameLine();
                ImGui::InputInt("ID", &bot_weapon);

                static char tmp_cmd[256] {};
                if (ImGui::Button("打字")) {
                    for (auto& bot : getAllBot()) {
                        bot->sendChat(tmp_cmd);
                    }
                }
                ImGui::InputText("##cmd", tmp_cmd, sizeof(tmp_cmd));
                ImGui::TreePop();
            }
        }
    }

    void CBotManager::renderBotContextMenu(CBot *bot, int bot_id) {
        ImGui::MenuItem("操作", NULL, false, false);
        auto status = bot->getStatus();
        if (ImGui::BeginMenu("连接状态")) {
            if (ImGui::MenuItem("连接")) {
                for (auto it: bot_selection) {
                    if (!pool[it]->isConnected())
                        pool[it]->connect();
                }
            }
            if (ImGui::MenuItem("断开/重连")) {
                for (auto it: bot_selection) {
                    if (pool[it]->isConnected())
                        pool[it]->disconnect();
                }
            }
            if (ImGui::MenuItem("删除")) {
                for (auto it: bot_selection) {
                    remove(it);
                }
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("指令")) {
            if (ImGui::MenuItem("说话/命令")) {

            }
            ImGui::EndMenu();
        }
        if (ImGui::MenuItem("重生")) {
            for (auto it: bot_selection) {
                if (pool[it]->isConnected()) {
                    pool[it]->sendSpawnRequest();
                    pool[it]->sendSpawn();
                }
            }
        }
    }
}

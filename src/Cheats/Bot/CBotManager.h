//
// Created by Administrator on 2024/10/10.
//

#ifndef CBOTMANAGER_H
#define CBOTMANAGER_H

#include <array>
#include <memory>
#include <imgui.h>
#include "CCheatModule.h"

namespace CheatModule::Bot {
    class CBot;
    class CBotManager : CCheatModule {
        static constexpr int MAX_BOT = 256;
        std::array<std::unique_ptr<CBot>, MAX_BOT> pool;
        int largest_botID;
        std::unordered_set<std::string> occupied_name;

        // 用于单个机器人的proxy
        char menu_proxy_host[256];
        unsigned int menu_proxy_port;
        char menu_proxy_user[256];
        char menu_proxy_pass[256];

        char bot_custom_name[SAMP_MAX_PLAYER_NAME];

        ImVector<int> bot_selection;
    public:
        CBotManager();
        ~CBotManager();

        int name_strategy;
        unsigned int name_len;
        unsigned int suffix_len;
        unsigned int bot_cnt_adder;
        bool auto_recon;
        unsigned int recon_delay;

        bool invulnerable, ignore_location_rpc;
        bool fake_spawn;
        bool fake_mobile;
        bool auto_login;
        bool auto_disconnect;
        bool auto_name;
        bool use_weapon;
        int bot_weapon;
        std::string bot_password;

        enum eProxyMode {
            PM_NONE,
            PM_SINGLE,
            PM_AUTO,
            PM_AUTO_STRICT
        };

        int proxy_mode;

        void resetProxyInfo();

        void onSave(nlohmann::json &j) override;
        void onLoad(const nlohmann::json &j) override;
        /**
         *
         * @param name bot的名字
         * @return 新建bot的指针，如果满了则返回nullptr
         */
        CBot* create(std::string name);

        /**
         * 
         * @param botID 要删除的机器人ID
         */
        void remove(int botID);

        /*
         * 处理机器人接收数据包和同步作弊
         */
        void process();
        void clear();
        std::vector<CBot*> getAllBot();
        std::string gen_name(int strategy, int name_len, int name_suffix_len);

        void spawnAll();
        void clearAll();

        bool isNicknameAvailable(std::string nickname);

        void renderBotManager();
        void renderBotContextMenu(CBot* bot, int bot_id);
    };
}

#endif //CBOTMANAGER_H

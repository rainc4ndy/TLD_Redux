#include "Plugin.h"

#include <sampapi/CChat.h>
#include <StringCompressor.h>
#include <RakHook/rakhook.hpp>

#include "CLog.h"
#include "CMenu.h"
#include "CConfig.h"
#include "CTextureManager.h"
#include "CCheatManager.h"
#include "CRenderer.h"
#include "CProxyManager.h"
#include "HueCycler.h"
#include "TargetSelector.h"

#include "d3dhook.h"
#include "encoding.h"
#include "samp.h"
#include "scripting.h"
#include "keyhook.h"
#include "LicenseStuff.h"
#include "Cheats/Bot/CNicknameGenerator.h"
#include "Cheats/Bot/CBotManager.h"
#include "Cheats/Vehicle/CarTrollSync.h"

// 初始化静态成员
HMODULE Plugin::s_hModule = nullptr;
const std::string Plugin::CurrentVersion = "3.0";
const std::string Plugin::MasterServer = "8.138.96.41:38900";

// 获取单例实例
Plugin& Plugin::getInstance() {
    // 创建并返回静态实例
    static Plugin instance;
    return instance;
}

// 私有构造函数
Plugin::Plugin() {
}

void Plugin::init() {
    pluginDirectory = std::filesystem::current_path().generic_string() + "/TLD_Redux";
    p_log = std::make_unique<CLog>(pluginDirectory.c_str());
    p_log->log("Working directory: %s ", pluginDirectory.c_str());
    p_menu = std::make_unique<CMenu>();

    p_tex = std::make_unique<CTextureManager>();
    p_cheat = std::make_unique<CCheatManager>();
    p_hue = std::make_unique<HueCycler>(600);
    p_render = std::make_unique<CRenderer>();
    p_target = std::make_unique<CTargetSelector>();
    p_proxyManager = std::make_unique<CProxyManager>();
    p_proxyManager->loadFromFile(pluginDirectory + "/proxy.txt");
    p_botManager = std::make_unique<CheatModule::Bot::CBotManager>();
    p_licenseChecker = std::make_unique<CLicenseChecker>();

    p_licenseChecker->ProcessLicenseCheck();
    p_AsyncHTTPWorker = std::make_unique<AsyncHTTPWorker>();

    p_config = std::make_unique<CConfig>();
    p_config->load(pluginDirectory + PLUGIN_CONFIG_FILE);

    InitScripting();

    CheatModule::Bot::CNicknameGenerator::LoadFromList(pluginDirectory + "/nickname.txt");

    using namespace std::placeholders;
    hookCTimerUpdate.set_cb(std::bind(&Plugin::mainloop, this, _1));
    hookCTimerUpdate.install();

    p_log->log("Plugin inited");
}

// 私有析构函数
Plugin::~Plugin() {
    // No need to manually delete unique_ptrs; they will be automatically cleaned up
}

void Plugin::mainloop(const decltype(hookCTimerUpdate)& hook) {
    static bool inited = false, d3d_inited = false;
    auto r = Plugin::getInstance().getCheat()->getAll();

    if (!d3d_inited) {
        SetD3D9Hooks();
        d3d_inited = true;
    }

    if (!inited && samp::RefChat() != nullptr && rakhook::initialize()) {
        using namespace std::placeholders;
        samp::RefChat()->AddMessage(0xFFFFFFFF, "The Last Dancer - Redux was loaded successfully");

        unsigned int tt = p_licenseChecker->GetRemainTime(); // in seconds
        unsigned int days = tt / (24 * 3600); // 1 day = 24 hours = 86400 seconds
        unsigned int hours = (tt % (24 * 3600)) / 3600; // remaining hours after removing days
        std::stringstream ss;
        ss << "-> 剩余时长 " << days << "天" << hours << "小时";
        auto content_converted = std::unique_ptr<char[]>(EncodingUtil::u2g(ss.str().c_str()));
        samp::RefChat()->AddMessage(0xFFFFFFFF, content_converted.get());

        p_licenseChecker->GetDayMessage();
        p_licenseChecker->GetUpdate();

        getLog()->log("SAMP initialized");
        StringCompressor::AddReference();
        rakhook::on_receive_rpc += std::bind(&PluginRPC::onServerMessage, &RPC, _1, _2);
        rakhook::on_send_rpc += std::bind(&PluginRPC::onClientMessage, &RPC, _1, _2, _3, _4, _5, _6);
        rakhook::on_send_packet += std::bind(&PluginRPC::onOutcomingPacket, &RPC, _1, _2, _3, _4);
        rakhook::on_receive_packet += std::bind(&PluginRPC::onIncomingPacket, &RPC, _1);

        for (auto& it : r) {
            if (it->isPending()) it->setEnabled(true);
        }
        inited = true;
    }

    if (inited) {
        getTarget()->get();
    }
    for (auto& it : r) {
        if (it->isEnabled()) it->onProcess();
    }
    Plugin::getInstance().getBotManager()->process();

    CheatModule::Vehicle::CarTrollSync::processPacketQueue();

    p_AsyncHTTPWorker->retriveAndCall();
    if (d3d_inited) {
        keyhook_run();
    }
    return hook.get_trampoline()();
}

CLog* Plugin::getLog() { return p_log.get(); }
CMenu* Plugin::getMenu() { return p_menu.get(); }
CConfig* Plugin::getConfig() { return p_config.get(); }
CTextureManager* Plugin::getTexs() { return p_tex.get(); }
CCheatManager* Plugin::getCheat() { return p_cheat.get(); }
HueCycler* Plugin::getHueCycler() { return p_hue.get(); }
CRenderer* Plugin::getRenderer() { return p_render.get(); }
CTargetSelector* Plugin::getTarget() { return p_target.get(); }
CProxyManager* Plugin::getProxyManager() { return p_proxyManager.get(); }
CheatModule::Bot::CBotManager* Plugin::getBotManager() { return p_botManager.get(); }


CLicenseChecker * Plugin::getLicenseChecker() {
    return p_licenseChecker.get();
}

AsyncHTTPWorker * Plugin::getAsyncHTTPWorker() {
    return p_AsyncHTTPWorker.get();
}

std::string Plugin::getDirectory() { return pluginDirectory; }

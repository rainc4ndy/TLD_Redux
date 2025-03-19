#pragma once


#include <kthook/kthook.hpp>
#include "PluginRPC.h"
#include "types.h"
#include <string>
#include <memory> // Include for std::unique_ptr

#include "AsyncHTTPWorker.h"

using CTimerProto = void( __cdecl* )();

class Plugin {
public:
    static Plugin& getInstance();
    static const std::string CurrentVersion;
    static const std::string MasterServer;

    void init();
    CLog* getLog();
    CMenu* getMenu();
    CConfig* getConfig();
    CTextureManager* getTexs();
    CCheatManager* getCheat();
    HueCycler* getHueCycler();
    CRenderer* getRenderer();
    CTargetSelector* getTarget();
    CProxyManager* getProxyManager();
    CheatModule::Bot::CBotManager* getBotManager();
    CLicenseChecker* getLicenseChecker();
    AsyncHTTPWorker* getAsyncHTTPWorker();

    // 禁止拷贝和赋值
    Plugin(const Plugin&) = delete;
    Plugin& operator=(const Plugin&) = delete;

    std::string getDirectory();

private:
    Plugin();  // 私有构造函数
    ~Plugin();  // 私有析构函数

    static HMODULE s_hModule;  // 静态成员保存 hModule
    std::string pluginDirectory;

    std::unique_ptr<CLog> p_log;
    std::unique_ptr<CMenu> p_menu;
    std::unique_ptr<CConfig> p_config;
    std::unique_ptr<CTextureManager> p_tex;
    std::unique_ptr<CCheatManager> p_cheat;
    std::unique_ptr<HueCycler> p_hue;
    std::unique_ptr<CRenderer> p_render;
    std::unique_ptr<CTargetSelector> p_target;
    std::unique_ptr<CProxyManager> p_proxyManager; // Assuming you want this as well
    std::unique_ptr<CheatModule::Bot::CBotManager> p_botManager; // Assuming you want this as
    std::unique_ptr<CLicenseChecker> p_licenseChecker;
    std::unique_ptr<AsyncHTTPWorker> p_AsyncHTTPWorker;

    kthook::kthook_simple<CTimerProto> hookCTimerUpdate{ reinterpret_cast<void*>(0x561B10) };
    void mainloop(const decltype(hookCTimerUpdate)& hook);

    PluginRPC RPC;
};

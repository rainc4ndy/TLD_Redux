#include "CConfig.h"

#include "CLog.h"
#include "Plugin.h"
#include "Utils.h"
#include "CMenu.h"
#include "TargetSelector.h"

#include "CCheatManager.h"
#include "default_style.h"
#include "Cheats/Bot/CBotManager.h"


CConfig::CConfig() {
    gocVehicle = -1;
    gocFix = false;
    gocMode = 0;
    gocDelay = 80;
    gocRange = 300;
    gocSpeed = 8;
}

CConfig::~CConfig() {
}

void CConfig::load(const std::string file) {
    using namespace nlohmann;
    std::ifstream ifs(file);
    if (ifs.is_open()) {
        try {
            json jf = json::parse(ifs);
            from_json(jf, *this);
        } catch (const std::exception &ex) {
            Plugin::getInstance().getLog()->log(ex.what());
            throw std::runtime_error(ex.what());
        }
        ifs.close();
    } else {
        Plugin::getInstance().getLog()->log("%s does not exist... try creating it", file.c_str());

        this->menu_key_str = "Insert";
        this->menu_key = CUtils::mapStringToVK(this->menu_key_str);

        this->font_str = "font.ttf";
        this->font_size = 17;
        this->color_theme = THEME_MIKU;

        auto menu = Plugin::getInstance().getMenu();

        menu->backgroundImage.zoom = 1;
        menu->backgroundImage.center_x = 0;
        menu->backgroundImage.center_y = 0;
        menu->backgroundImage.transparency = 0.75f;
        menu->menuTransparency = 1.0f;

        applyGocVariables();

        auto target_selector = Plugin::getInstance().getTarget();
        target_selector->delay = 120;
        target_selector->mode = 0;
        target_selector->NPCIncluded = false;

        auto cc = Plugin::getInstance().getCheat();
        cc->darkStab.use_own_weapon = false;
        cc->darkStab.range = 32.0f;
        cc->supremeDashing.speed = 25.0f;
        cc->masterDriver.setEnabled(true);
        cc->zombieAutomata.money_hack = true;
        cc->zombieAutomata.pattern = cc->zombieAutomata.ZP_ALL;
        cc->zombieAutomata.use_invis = true;
        cc->zombieAutomata.rate_factor = 2.2f;
        cc->bulletKiller.delay = 40;
        cc->bulletKiller.use_override_weapon = false;
        cc->captchaSolver.setEnabled(true);
        cc->triggerBot.range = 64;

        auto bot = Plugin::getInstance().getBotManager();
        bot->name_strategy = 0;
        bot->name_len = 10;
        bot->suffix_len = 3;
        bot->proxy_mode = 0;
        bot->bot_cnt_adder = 1;
        bot->recon_delay = 3000;

        cc->fovChanger.setEnabled(true);
        cc->fovChanger.fov = 90;

        cc->timeWeatherHack.time = 7;
        cc->timeWeatherHack.time = 2;

        cc->noCameraCol.setEnabled(true);
        cc->playerESP.color = D3DCOLOR_XRGB(255, 255, 255);
        cc->playerESP.thickness = 1.8f;

        save(file);
    }
    //applyGocVariables();
}

void CConfig::save(const std::string file) {
    using namespace nlohmann;
    json jf(*this);
    auto data = jf.dump(4);

    std::ofstream ofs(file);
    if (ofs.is_open()) {
        ofs << data;
        ofs.close();
    } else {
        throw std::runtime_error("Cannot open config file " + file);
    }
}

void CConfig::to_json(nlohmann::json &j, const CConfig &p) {
    auto menu = Plugin::getInstance().getMenu();
    auto cc = Plugin::getInstance().getCheat();
    auto target_selector = Plugin::getInstance().getTarget();

    j = nlohmann::json{
        {
            "interface", {
                {"menu_key", p.menu_key_str},
                {"font_file", p.font_str},
                {"font_size", p.font_size},
                {"color_theme", p.color_theme},
                {"background_zoom", menu->backgroundImage.zoom},
                {"background_x", menu->backgroundImage.center_x},
                {"background_y", menu->backgroundImage.center_y},
                {"background_alpha", menu->backgroundImage.transparency},
                {"window_alpha", menu->menuTransparency},
            }
        },
        {
            "cheats", {

            }
        },
        {
            "goc", {
                {"gocRange", p.gocRange},
                {"gocFix", p.gocFix},
                {"mode", p.gocMode}
            }
        },
        {
            "target_selector", {
                {"include_npc", target_selector->NPCIncluded},
                {"mode", target_selector->mode},
                {"delay", target_selector->delay},
            }
        },
    };

    auto modules = cc->getAll();
    nlohmann::json cheat_status;
    for (auto &it: modules) {
        // Add the module status to cheat_status
        nlohmann::json j;
        it->saveJson(j);

        cheat_status[it->getNameEn()] = j;
        // append to cheat_status
    }
    j["cheats"] = cheat_status; // Add the cheat status to the main json
}

void CConfig::from_json(const nlohmann::json &j, CConfig &p) {
    auto menu = Plugin::getInstance().getMenu();
    auto cc = Plugin::getInstance().getCheat();
    auto target_selector = Plugin::getInstance().getTarget();


    auto inter = j.at("interface");
    auto tg = j.at("target_selector");
    inter.at("menu_key").get_to(p.menu_key_str);
    inter.at("font_file").get_to(p.font_str);
    inter.at("font_size").get_to(p.font_size);
    inter.at("color_theme").get_to(p.color_theme);
    inter.at("background_zoom").get_to(menu->backgroundImage.zoom);
    inter.at("background_x").get_to(menu->backgroundImage.center_x);
    inter.at("background_y").get_to(menu->backgroundImage.center_y);
    inter.at("background_alpha").get_to(menu->backgroundImage.transparency);
    inter.at("window_alpha").get_to(menu->menuTransparency);
    p.menu_key = CUtils::mapStringToVK(p.menu_key_str);

    auto goc = j.at("goc");
    goc.at("gocRange").get_to(p.gocRange);
    goc.at("gocFix").get_to(p.gocFix);
    goc.at("mode").get_to(p.gocMode);

    tg.at("include_npc").get_to(target_selector->NPCIncluded);
    tg.at("mode").get_to(target_selector->mode);
    tg.at("delay").get_to(target_selector->delay);

    try {
        auto modules = cc->getAll();
        auto cheats = j.at("cheats");
        for (auto &it: modules) {
            try {
                // Add the module status to cheat_status
                it->fromJson(cheats.at(it->getNameEn()));
            } catch (const std::exception &ex) {
                Plugin::getInstance().getLog()->log(ex.what());
            }
        }
    } catch (const std::exception &ex) {
        Plugin::getInstance().getLog()->log(ex.what());
        throw std::runtime_error(ex.what());
    }
}

void CConfig::applyGocVariables() {
    auto r = CCheatManager::getAll();
    for (auto &it: r) {
        if (auto goc = dynamic_cast<CheatModule::Vehicle::GocModule *>(it)) {
            goc->delay = gocDelay;
            goc->speed = gocSpeed;
        }
    }
}

void to_json(nlohmann::json &j, const CConfig &p) { CConfig::to_json(j, p); }

void from_json(const nlohmann::json &j, CConfig &p) {
    CConfig::from_json(j, p);
}

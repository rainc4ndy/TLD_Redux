#ifndef C26081BC_551B_4F98_ABA4_4894BB8D3540
#define C26081BC_551B_4F98_ABA4_4894BB8D3540

#include <string>
#include <nlohmann/json.hpp>
#include <RakHook/rakhook.hpp>

#include "samp.h"

#include "types.h"
class CCheatModule
{
protected:
    std::string name, nameEn;
    bool enabled;
    bool pending_for_enable;
    bool used_by_bot;
    CheatModule::Bot::CBot* bot;
public:
    CCheatModule(std::string name, std::string nameEn);
    CCheatModule(CheatModule::Bot::CBot *bot);

    virtual ~CCheatModule() = default;

    bool isPending();
    bool isEnabled();
    void setEnabled(bool enabled);
    virtual void onEnable() {};
    virtual void onDisable() {};
    virtual void onProcess() {};
    virtual void onProcessVisual() {};
    virtual bool onProcess(stOnFootData *data) { return true; };
    virtual bool onProcess(stBulletData *data) { return true; };
    virtual void onProcess(int playerid, stOnFootData *data) {};
    virtual void onProcess(int playerid, stBulletData *data) {};
    virtual void onProcess(int playerid, stAimData *data) {};

    virtual bool onProcess(stInCarData *data) { return true; };
    virtual bool onProcess(stAimData *data) { return true; };
    virtual bool onProcess(bool fromServer, int rpc, RakNet::BitStream* bs) { return true; }

    void saveJson(nlohmann::json &j);
    void fromJson(const nlohmann::json &j);

    virtual void onSave(nlohmann::json &j);
    virtual void onLoad(const nlohmann::json &j);

    template<typename T>
    void saveData(nlohmann::json &j, const std::string& key, T value);
    template<typename T>
    void loadData(const nlohmann::json &j, const std::string& key, T& value);

    std::string getName();
    std::string getNameEn();
};

#endif /* C26081BC_551B_4F98_ABA4_4894BB8D3540 */

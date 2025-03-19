#ifndef B6620D64_26E9_42EC_BF51_6BC49CE3AADA
#define B6620D64_26E9_42EC_BF51_6BC49CE3AADA

#include <string>
#include <unordered_set>
#include <unordered_map>

#include "CCheatModule.h"


#include "Cheats/Player/InvisibleSpec.h"
#include "Cheats/Player/Twister.h"
#include "Cheats/Player/GodMode.h"
#include "Cheats/Player/Unmoving.h"
#include "Cheats/Player/FastWalk.h"
#include "Cheats/Player/FastSpawn.h"

#include "Cheats/Player/RandomArmor.h"
#include "Cheats/Player/RandomHealth.h"
#include "Cheats/Player/PedPatch.h"

#include "Cheats/Vehicle/DoorStorm.h"
#include "Cheats/Vehicle/SteathDriver.h"
#include "Cheats/Vehicle/CarShooter.h"
#include "Cheats/Vehicle/CarLagger.h"
#include "Cheats/Vehicle/CarStorm.h"
#include "Cheats/Vehicle/CarWall.h"
#include "Cheats/Vehicle/SupremeDashing.h"

#include "Cheats/Weapon/FastShot.h"
#include "Cheats/Weapon/NoReload.h"
#include "Cheats/Weapon/NoSpread.h"
#include "Cheats/Weapon/NPCKiller.h"
#include "Cheats/Weapon/CTriggerBot.h"
#include "Cheats/Weapon/WepPatch.h"

#include "Cheats/Target/Smasher.h"
#include "Cheats/Target/Elevator.h"
#include "Cheats/Target/DeathTornado.h"
#include "Cheats/Target/DeathCross.h"
#include "Cheats/Target/Mimic.h"
#include "Cheats/Target/DarkStab.h"
#include "Cheats/Target/BulletKiller.h"
#include "Cheats/Target/TargetRenderer.h"

#include "Cheats/Visual/CheatList.h"
#include "Cheats/Visual/HUD.h"
#include "Cheats/Visual/PlayerESP.h"
#include "Cheats/Visual/FovChanger.h"

#include "Cheats/Hack/DialogSpam.h"
#include "Cheats/Hack/NoDialog.h"
#include "Cheats/Hack/ZombieAutomata.h"
#include "Cheats/Hack/CaptchaSolver.h"
#include "Cheats/Hack/AutoLogin.h"
#include "Cheats/Hack/FakeMobile.h"
#include "Cheats/Hack/RPCAnalysis.h"

#include "Cheats/Vehicle/AllVanish.h"
#include "Cheats/Vehicle/MasterDriver.h"
#include "Cheats/Visual/Console.h"
#include "Cheats/Visual/TimeWeatherHack.h"


#include "Cheats/Bot/CBotFollow.h"



class CCheatManager
{
private:
    static std::unordered_set<CCheatModule*> allInstnace;
    static std::unordered_map<std::string, CCheatModule*> nameMap;

    IDirect3DDevice9*device = nullptr;
public:
    static void add(CCheatModule *module);
    static const std::unordered_set<CCheatModule*> getAll();
    static CCheatModule* get(std::string englishName);

    CheatModule::Player::InvisibleSpec invis;
    CheatModule::Player::Twister twist;
    CheatModule::Player::GodMode god;
    CheatModule::Player::Unmoving unmoving;
    CheatModule::Player::FastWalk fastwalk;
    CheatModule::Player::FastSpawn fastSpawn;
    CheatModule::Player::RandomArmor randomArmor;
    CheatModule::Player::RandomHealth randomHealth;
    CheatModule::Player::NoCameraCol noCameraCol;
    CheatModule::Player::InfiniteRun infiniteRun;
    CheatModule::Player::AntiFall antiFall;
    CheatModule::Player::LongHP longHP;

    CheatModule::Vehicle::DoorStorm doorStorm;
    CheatModule::Vehicle::SteathDriver steathDriver;
    CheatModule::Vehicle::CarShooter carShooter;
    CheatModule::Vehicle::CarLagger carLagger;
    CheatModule::Vehicle::CarStorm carStorm;
    CheatModule::Vehicle::CarWall carWall;
    CheatModule::Vehicle::AllVanish allVanish;
    CheatModule::Vehicle::SupremeDashing supremeDashing;
    CheatModule::Vehicle::MasterDriver masterDriver;

    CheatModule::Weapon::FastShot fastShot;
    CheatModule::Weapon::NoReload noReload;
    CheatModule::Weapon::NoSpread noSpread;
    CheatModule::Weapon::NPCKiller npcKiller;
    CheatModule::Weapon::CTriggerBot triggerBot;

    CheatModule::Target::Smasher smasher;
    CheatModule::Target::Elevator elevator;
    CheatModule::Target::DeathTornado deathTornado;
    CheatModule::Target::DeathCross deathCross;
    CheatModule::Target::Mimic mimic;
    CheatModule::Target::DarkStab darkStab;
    CheatModule::Target::TargetRenderer targetRenderer;
    CheatModule::Target::BulletKiller bulletKiller;

    CheatModule::Visual::CheatList cheat_list;
    CheatModule::Visual::HUD hud;
    CheatModule::Visual::PlayerESP playerESP;
    CheatModule::Visual::Console console;
    CheatModule::Visual::TimeWeatherHack timeWeatherHack;
    CheatModule::Visual::FovChanger fovChanger;

    CheatModule::Hack::DialogSpam dialogSpam;
    CheatModule::Hack::NoDialog NoDialog;
    CheatModule::Hack::ZombieAutomata zombieAutomata;
    CheatModule::Hack::CaptchaSolver captchaSolver;
    CheatModule::Hack::AutoLogin autoLogin;
    CheatModule::Hack::FakeMobile fakeMobile;
    CheatModule::Hack::RPCAnalysis rpcAnalysis;

    CheatModule::Bot::CBotFollow botFollow;
};

#endif /* B6620D64_26E9_42EC_BF51_6BC49CE3AADA */

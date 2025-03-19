#ifndef DD33AD86_6D04_4140_BDA5_EE860567D2DC
#define DD33AD86_6D04_4140_BDA5_EE860567D2DC

#include <RakHook/rakhook.hpp>

#include "../../CCheatModule.h"
namespace CheatModule
{
namespace Player 
{

class GodMode : public CCheatModule
{
private:
    float previous_hp;
    int tick;
public:
    GodMode(/* args */);
    void onProcess() override;
    bool onProcess(bool fromServer, int rpc, RakNet::BitStream* bs) override;
    void onEnable() override;
    void onDisable() override;
};

}
}


#endif /* DD33AD86_6D04_4140_BDA5_EE860567D2DC */

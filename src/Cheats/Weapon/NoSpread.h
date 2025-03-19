#ifndef E6D15331_85F7_4A43_A2D2_A0A07F31DF34
#define E6D15331_85F7_4A43_A2D2_A0A07F31DF34


#include "../../CCheatModule.h"
namespace CheatModule
{
namespace Weapon
{

class NoSpread : public CCheatModule
{
private:
    int tick;
    int speed;
public:
    NoSpread();
    void onEnable() override;
    void onDisable() override;
};

}
}

#endif /* E6D15331_85F7_4A43_A2D2_A0A07F31DF34 */

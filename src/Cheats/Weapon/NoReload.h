#ifndef F3E97F1C_70F8_4EC0_BBE9_374F3EA5E07B
#define F3E97F1C_70F8_4EC0_BBE9_374F3EA5E07B



#include "../../CCheatModule.h"
namespace CheatModule
{
namespace Weapon
{

class NoReload : public CCheatModule
{
private:
    int tick;
    int speed;
public:
    NoReload();
    void onProcess() override;
};

}
}


#endif /* F3E97F1C_70F8_4EC0_BBE9_374F3EA5E07B */

#ifndef C6955208_E227_4A02_94E8_5616591E3D09
#define C6955208_E227_4A02_94E8_5616591E3D09


#include "../../CCheatModule.h"
namespace CheatModule
{
namespace Weapon
{

class FastShot : public CCheatModule
{
private:
    int tick;
public:
    FastShot();
    void onProcess() override;
    float speed;

    void onSave(nlohmann::json &j) override;
    void onLoad(const nlohmann::json &j) override;
};

}
}



#endif /* C6955208_E227_4A02_94E8_5616591E3D09 */

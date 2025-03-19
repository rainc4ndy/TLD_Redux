#ifndef B7C5190D_9E9D_4165_B5D2_3D8D0A72DA99
#define B7C5190D_9E9D_4165_B5D2_3D8D0A72DA99


#include <RakHook/rakhook.hpp>

#include "../../CCheatModule.h"
namespace CheatModule
{
namespace Hack 
{

class DialogSpam : public CCheatModule
{
private:
    int tick;
public:
    bool no_show, no_response;
    bool spoofing;
    bool r_button;
    char content[128];
    unsigned int delay;
    int item;

    unsigned short last_dialog;

    DialogSpam(/* args */);
    void onProcess() override;
};

}
}


#endif /* B7C5190D_9E9D_4165_B5D2_3D8D0A72DA99 */

#ifndef E52A659C_4CBC_4F44_A24D_48982430EDF8
#define E52A659C_4CBC_4F44_A24D_48982430EDF8

#include "../../CCheatModule.h"

namespace CheatModule
{
namespace Weapon
{

class NPCKiller : public CCheatModule
{
public:
    NPCKiller();
    bool onProcess(bool fromServer, int rpc, RakNet::BitStream* bs) override;

    void killAll();
};

}
}


#endif /* E52A659C_4CBC_4F44_A24D_48982430EDF8 */

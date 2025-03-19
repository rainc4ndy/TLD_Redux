#ifndef DDEB16BF_63DE_4F8D_97E1_CB3A71C3C53D
#define DDEB16BF_63DE_4F8D_97E1_CB3A71C3C53D

#include <RakHook/rakhook.hpp>

#include "../../CCheatModule.h"
namespace CheatModule
{
namespace Player 
{

class Unmoving: public CCheatModule
{
private:
    /* data */
public:
    Unmoving(/* args */);
    bool onProcess(bool fromServer, int rpc, RakNet::BitStream* bs) override;
};

}
}


#endif /* DDEB16BF_63DE_4F8D_97E1_CB3A71C3C53D */

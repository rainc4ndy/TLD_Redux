#ifndef A644CA24_62CF_4CFB_89AD_EC53D648E065
#define A644CA24_62CF_4CFB_89AD_EC53D648E065


#include <RakHook/rakhook.hpp>

#include "../../CCheatModule.h"
namespace CheatModule
{
namespace Hack 
{

class NoDialog : public CCheatModule
{
public:
    NoDialog();
    bool onProcess(bool fromServer, int rpc, RakNet::BitStream* bs) override;
};

}
}

#endif /* A644CA24_62CF_4CFB_89AD_EC53D648E065 */

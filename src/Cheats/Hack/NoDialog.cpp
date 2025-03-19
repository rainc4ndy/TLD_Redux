#include "NoDialog.h"

#include <sampapi/CEntity.h>
#include <sampapi/CLocalPlayer.h>
#include <sampapi/CNetGame.h>
#include <sampapi/CPlayerPool.h>

#include <RakHook/rakhook.hpp>

#include "../../samp.h"

namespace CheatModule {
namespace Hack {

NoDialog::NoDialog() : CCheatModule("不显示对话框", "No Dialog") {
}

bool NoDialog::onProcess(bool fromServer, int rpc, RakNet::BitStream* bs) {
    if (fromServer) {
        if (rpc == RPC_ShowDialog) {
            return false;
        }
    } else {
        if (rpc == RPC_DialogResponse) {
            return false;
        }
    }
    return true;
}

}  // namespace Hack
}  // namespace CheatModule
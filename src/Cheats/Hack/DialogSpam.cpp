#include "DialogSpam.h"

#include <BitStream.h>
#include <sampapi/CEntity.h>
#include <sampapi/CLocalPlayer.h>
#include <sampapi/CNetGame.h>
#include <sampapi/CPlayerPool.h>

#include <RakHook/rakhook.hpp>

#include "../../samp.h"

namespace CheatModule {
namespace Hack {
DialogSpam::DialogSpam() : CCheatModule("刷对话框", "Dialog Spam") {
    delay = 200;
    r_button = false;
}

void DialogSpam::onProcess() {
    if (GetTickCount() - tick > delay) {
        RakNet::BitStream bsSend;
        bsSend.Write(last_dialog);
        bsSend.Write((BYTE)!r_button);
        bsSend.Write((INT16)item);

        BYTE len = (BYTE)strlen(content);
        bsSend.Write(len);
        bsSend.Write(content, len);

        rakhook::send_rpc(RPC_DialogResponse, &bsSend, HIGH_PRIORITY,
                          RELIABLE_ORDERED, 0, false);
        tick = GetTickCount();
    }
}

}  // namespace Hack
}  // namespace CheatModule
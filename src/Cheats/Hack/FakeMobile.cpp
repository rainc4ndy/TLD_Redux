//
// Created by Administrator on 2025/2/15.
//

#include "FakeMobile.h"
#include "../../Utils.h"

namespace CheatModule::Hack {
    FakeMobile::FakeMobile() : CCheatModule("手机端", "Fake Mobile") {}
    bool FakeMobile::onProcess(bool fromServer, int rpc, RakNet::BitStream *bs) {
        if (!fromServer && rpc == RPC_ClientJoin) {
            bs->ResetReadPointer();
            UINT32 iVersion; UINT8 byteMod; UINT8 byteNicknameLen;
            UINT32 uiClientChallengeResponse;
            UINT8 byteAuthKeyLen;
            UINT8 iClientVerLen;

            bs->Read(iVersion);
            bs->Read(byteMod);

            bs->Read(byteNicknameLen);
            std::string NickName(byteNicknameLen, 0);
            bs->Read(NickName.data(), byteNicknameLen);

            bs->Read(uiClientChallengeResponse);

            bs->Read(byteAuthKeyLen);
            std::string auth_key(byteAuthKeyLen, 0);
            bs->Read(auth_key.data(), byteAuthKeyLen);

            bs->Read(iClientVerLen);
            std::string clientVersion(iClientVerLen, 0);
            bs->Read(clientVersion.data(), iClientVerLen);

            auth_key = FakeMobileKey;

            RakNet::BitStream bsSend;
            bsSend.Reset();
            bsSend.Write(iVersion);
            bsSend.Write(byteMod);
            bsSend.Write((BYTE)NickName.size());
            bsSend.Write(NickName.data(), NickName.size());
            bsSend.Write(uiClientChallengeResponse);
            bsSend.Write((BYTE)auth_key.size());
            bsSend.Write(auth_key.data(), auth_key.size());
            bsSend.Write((BYTE)clientVersion.size());
            bsSend.Write(clientVersion.data(), clientVersion.size());

            rakhook::send_rpc(RPC_ClientJoin, &bsSend, HIGH_PRIORITY, RELIABLE_ORDERED, 0, false);
            return false;
        }
        return true;
    }
} // CheatModule

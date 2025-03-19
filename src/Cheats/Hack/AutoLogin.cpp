//
// Created by Administrator on 2024/10/13.
//

#include "AutoLogin.h"

#include "encoding.h"
#include "Utils.h"
#include "Cheats/Bot/CBot.h"

namespace CheatModule {
namespace Hack {
    AutoLogin::AutoLogin() : CCheatModule("自动登录", "Auto Login") {}

    bool AutoLogin::onProcess(bool fromServer, int rpc, RakNet::BitStream *bs) {
        if (rpc == RPC_ShowDialog) {
            bs->ResetReadPointer();
            INT16 wDialogID;
            UINT8 bDialogStyle, bTitleLength;
            bs->Read(wDialogID);
            bs->Read(bDialogStyle);
            bs->Read(bTitleLength);

            if (bDialogStyle == DIALOG_STYLE_INPUT || bDialogStyle == DIALOG_STYLE_PASSWORD) {
                auto title_converted = EncodingUtil::readAndConvertUTF8(bs, bTitleLength);
                if (std::strstr(title_converted.c_str(), "登录")  ||
                    std::strstr(title_converted.c_str(), "注册") ||
                    std::strstr(title_converted.c_str(), "新手资金")) {

                    UINT8 bButton1Len, bButton2Lens;
                    bs->Read(bButton1Len);
                    bs->IgnoreBits(bButton1Len * 8);
                    bs->Read(bButton2Lens);
                    bs->IgnoreBits(bButton2Lens * 8);

                    RakNet::BitStream response;
                    response.Reset();
                    response.Write(wDialogID);
                    response.Write(static_cast<UINT8>(1));
                    response.Write(static_cast<INT16>(-1));
                    response.Write(static_cast<UINT8>(password.size()));
                    response.Write(password.data(), password.size());
                    rakhook::send_rpc(RPC_DialogResponse, &response, HIGH_PRIORITY,
                                                  RELIABLE_ORDERED, 0, false);
                    CUtils::logToConsole("[Login]: Auto login done");
                    return false;
                }
            }
        }
        return true;
    }
    void AutoLogin::onSave(nlohmann::json &j) {
        saveData(j, "password", password);
    }
    void AutoLogin::onLoad(const nlohmann::json &j) {
        loadData(j, "password", password);
    }
} // Hack
} // CheatModule

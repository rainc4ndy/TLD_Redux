//
// Created by Administrator on 2024/10/8.
//

#ifndef CAPTCHASOLVER_H
#define CAPTCHASOLVER_H

#include "CCheatModule.h"
#include "types.h"

namespace CheatModule::Hack {
    class CaptchaSolver : public CCheatModule {
    private:
        enum eAuthType {
            AUTH_BOT,
            AUTH_IP
        };

        bool required_to_auth;
        int auth_type;
        INT16 dialog_id;

        int tick;

        std::string auth_data;

        static constexpr int getTextdrawStringOffset();
    public:
        CaptchaSolver();
        explicit CaptchaSolver(Bot::CBot* bot);
        void solve(const std::string &captcha);
        bool onReceiveTD(RakNet::BitStream *bs);

        void onProcess();

        bool onProcess(bool fromServer, int rpc, RakNet::BitStream *bs) override;
    };

    constexpr int CaptchaSolver::getTextdrawStringOffset() {
        return
                sizeof(UINT16) + //TD_ID
                sizeof(UINT8) + // Flags
                sizeof(float) + // fLetterWidth
                sizeof(float) + // fLetterHeight
                sizeof(UINT32) + // dLetterColor
                sizeof(float) + // fLineWidth
                sizeof(float) + // fLineHeight
                sizeof(UINT32) + // dBoxColor
                sizeof(UINT8) + // Shadow
                sizeof(UINT8) + // Outline
                sizeof(UINT32) + // dBackgroundColor
                sizeof(UINT8) + // Style
                sizeof(UINT8) + // Selectable
                sizeof(float) + // fX
                sizeof(float) + // fY
                sizeof(UINT16) + // wModelID
                sizeof(float) + // fRotX
                sizeof(float) + // fRotY
                sizeof(float) + // fRotZ
                sizeof(float) + // fZoom
                sizeof(INT16) + // wColor1
                sizeof(INT16);
    }
} // Hack
// CheatModule

#endif //CAPTCHASOLVER_H

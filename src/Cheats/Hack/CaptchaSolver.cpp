#include "CaptchaSolver.h"
#include <StringCompressor.h>

#include "encoding.h"
#include "Utils.h"
#include "Cheats/Bot/CBot.h"

namespace CheatModule::Hack {
    CaptchaSolver::CaptchaSolver() : CCheatModule("过人机/双开检测", "Captcha Solver") {
        required_to_auth = false;
        auth_type = AUTH_BOT;
    }

    CaptchaSolver::CaptchaSolver(Bot::CBot *bot) : CCheatModule(bot) {
        required_to_auth = false;
        auth_type = AUTH_BOT;
    }

    void CaptchaSolver::solve(const std::string &captcha) {
        RakNet::BitStream response;
        response.Write(dialog_id); // 使用已定义的对话ID
        response.Write(static_cast<UINT8>(1)); // 响应类型
        response.Write(static_cast<INT16>(-1)); // 列表项
        response.Write(static_cast<UINT8>(captcha.size()));
        response.Write(captcha.data(), captcha.size());
        if (used_by_bot) {
            bot->sendRPC(RPC_DialogResponse, &response);
        } else {
            if (!rakhook::send_rpc(RPC_DialogResponse, &response, HIGH_PRIORITY, RELIABLE_ORDERED, 0, false)) {
                CUtils::chatprintf("Failed to send captcha response");
            }
        }
        required_to_auth = false;
    }

    bool CaptchaSolver::onReceiveTD(RakNet::BitStream *bs) {
        UINT16 text_len;
        bs->Read(text_len);
        if (text_len >= 4) {
            auth_data = EncodingUtil::readAndConvertUTF8(bs, text_len);
            if (auth_data.size() > 0) {
                auth_data.erase(begin(auth_data), begin(auth_data) + 3);
                std::erase_if(auth_data, isspace);

                if (required_to_auth) {
                    solve(auth_data);
                    if (!used_by_bot)
                        CUtils::logToConsole("[Captcha] IP验证码 \"%s\" 已解决", auth_data.c_str());
                    else {
                        CUtils::logToConsole("[Captcha] Bot %s solved \"%s\" (mode: IP)", bot->getName().c_str(),
                                             auth_data.c_str());
                    }
                }
            }
        }
        return true;
    }

    void CaptchaSolver::onProcess() {
        // TODO: 处理逻辑，如果需要可以添加实现
    }

    bool CaptchaSolver::onProcess(bool fromServer, int rpc, RakNet::BitStream *bs) {
        if (!fromServer) {
            return true;
        }
        if (rpc == RPC_ShowTextDraw) {
            bs->ResetReadPointer();
            bs->IgnoreBits(getTextdrawStringOffset() * 8);
            onReceiveTD(bs);
        } else if (rpc == RPC_ShowDialog) {
            bs->ResetReadPointer();
            INT16 wDialogID;
            UINT8 bDialogStyle, bTitleLength;
            bs->Read(wDialogID);
            bs->Read(bDialogStyle);
            bs->Read(bTitleLength);

            if (bDialogStyle == DIALOG_STYLE_INPUT) {
                auto title_converted = EncodingUtil::readAndConvertUTF8(bs, bTitleLength);
                if (std::strstr(title_converted.c_str(), "人类通行码")) {
                    dialog_id = wDialogID;
                    UINT8 bButton1Len, bButton2Lens;
                    bs->Read(bButton1Len);
                    bs->IgnoreBits(bButton1Len * 8);
                    bs->Read(bButton2Lens);
                    bs->IgnoreBits(bButton2Lens * 8);

                    char szInfo[257]{};
                    StringCompressor::Instance()->DecodeString(szInfo, 256, bs);
                    auto content_converted = std::unique_ptr<char[]>(EncodingUtil::g2u(szInfo));
                    if (std::strstr(content_converted.get(), "否则踢")) {
                        if (!used_by_bot) {
                            CUtils::logToConsole("[Captcha] 检测到IP验证，正在等待Textdraw");
                        } else {
                            CUtils::logToConsole("[Captcha] Bot %s discovered IP, wait for", bot->getName().c_str());
                        }
                        required_to_auth = true;
                    } else {
                        solve(auth_data);
                        if (!used_by_bot) {
                            CUtils::logToConsole("[Captcha] 检测到验证码 \"%s\", 处理完成", auth_data.c_str());
                        } else {
                            CUtils::logToConsole("[Captcha] Bot %s solved \"%s\" (mode: Bot)", bot->getName().c_str(),
                                                 auth_data.c_str());
                        }
                    }
                    return false;
                }
                if (std::strstr(title_converted.c_str(), "验证码")) {
                    dialog_id = wDialogID;
                    UINT8 bButton1Len, bButton2Lens;
                    bs->Read(bButton1Len);
                    bs->IgnoreBits(bButton1Len * 8);
                    bs->Read(bButton2Lens);
                    bs->IgnoreBits(bButton2Lens * 8);

                    char szInfo[257]{};
                    StringCompressor::Instance()->DecodeString(szInfo, 256, bs);
                    auto content_converted = std::unique_ptr<char[]>(EncodingUtil::g2u(szInfo));

                    static char auth_igta[] = "{FF0000}";
                    char *ptr;
                    CUtils::logToConsole(content_converted.get());
                    if ((ptr = std::strstr(content_converted.get(), auth_igta))) {
                        auto cap = std::string(ptr + sizeof(auth_igta) - 1);
                        solve(cap);
                        if (!used_by_bot) {
                            CUtils::logToConsole("[Captcha] 检测到 IGTA 验证码 \"%s\", 处理完成", cap);
                        } else {
                            CUtils::logToConsole("[Captcha] Bot %s solved \"%s\" (mode: IGTA)", bot->getName().c_str(),
                                                 cap.c_str());
                        }
                        return false;
                    }
                }
            }
        }
        return true;
    }
} // Hack

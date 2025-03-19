//
// Created by Administrator on 2024/10/13.
//

#ifndef AUTOLOGIN_H
#define AUTOLOGIN_H
#include "CCheatModule.h"

namespace CheatModule {
namespace Hack {


class AutoLogin : public CCheatModule {
private:
    std::string password;
public:
    AutoLogin();

    bool onProcess(bool fromServer, int rpc, RakNet::BitStream *bs) override;
    virtual void onLoginDialogRequest(int dialogID) {}
    void onLoad(const nlohmann::json &j) override;
    void onSave(nlohmann::json &j) override;
};

} // Hack
} // CheatModule

#endif //AUTOLOGIN_H

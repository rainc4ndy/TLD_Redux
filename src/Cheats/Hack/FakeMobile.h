//
// Created by Administrator on 2025/2/15.
//

#ifndef TLD_REDUX_FAKEMOBILE_H
#define TLD_REDUX_FAKEMOBILE_H

#include "../../CCheatModule.h"

constexpr char FakeMobileKey[] = "39FB2DEEDB49ACFB8D4EECE6953D2507988CCCF4410";

namespace CheatModule::Hack {
    class FakeMobile : public CCheatModule {
    public:
        FakeMobile();
        bool onProcess(bool fromServer, int rpc, RakNet::BitStream *bs) override;
    };
}


#endif //TLD_REDUX_FAKEMOBILE_H

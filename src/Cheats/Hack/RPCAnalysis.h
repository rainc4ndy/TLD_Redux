//
// Created by Administrator on 2025/2/15.
//

#ifndef TLD_REDUX_RPCANALYSIS_H
#define TLD_REDUX_RPCANALYSIS_H

#include "CCheatModule.h"

namespace CheatModule::Hack {
    class RPCAnalysis : public CCheatModule {
    private:
        static std::unordered_map<int, std::string> rpcIdToString;
    public:
        RPCAnalysis();
        bool onProcess(bool fromServer, int rpc, RakNet::BitStream *bs) override;
    };

} // CheatModule

#endif //TLD_REDUX_RPCANALYSIS_H

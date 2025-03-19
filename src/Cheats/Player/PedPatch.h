//
// Created by Administrator on 2025/2/15.
//

#ifndef TLD_REDUX_PEDPATCH_H
#define TLD_REDUX_PEDPATCH_H

#include "../../CPatchModule.h"

namespace CheatModule::Player {
    class NoCameraCol : public CPatchModule {
    private:
        void work() override;
    public:
        NoCameraCol();
    };

    class InfiniteRun : public CPatchModule {
    private:
        void work() override;
    public:
        InfiniteRun();
    };

    class AntiFall : public CPatchModule {
    private:
        void work() override;
    public:
        AntiFall();
    };

    class LongHP : public CPatchModule {
    private:
        void work() override;
    public:
        LongHP();
    };


} // CCheatModule

#endif //TLD_REDUX_PEDPATCH_H

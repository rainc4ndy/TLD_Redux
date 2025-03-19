//
// Created by Administrator on 2024/10/7.
//

#ifndef MASTERDRIVER_H
#define MASTERDRIVER_H

#include "../../CCheatModule.h"



namespace CheatModule::Vehicle {
    class MasterDriver : public CCheatModule {
        unsigned int tick;
    public:
        MasterDriver();
        void onProcess() override;
    };
}

#endif //MASTERDRIVER_H

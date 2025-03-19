//
// Created by Administrator on 2024/10/6.
//

#ifndef ALLVANISH_H
#define ALLVANISH_H


#include "../../CCheatModule.h"

namespace CheatModule::Vehicle {
    class AllVanish : public CCheatModule {
    private:
        int tick;
    public:
        AllVanish();
        void onProcess() override;
    };
}



#endif //ALLVANISH_H

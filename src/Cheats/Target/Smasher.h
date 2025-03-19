//
// Created by Administrator on 2024/10/7.
//

#ifndef SMASHER_H
#define SMASHER_H


#include "../Vehicle/GocModule.h"

namespace CheatModule::Target {
    class Smasher : public Vehicle::GocModule {
    public:
        Smasher();
        void onProcess() override;
    };
}
// CheatModule
#endif //SMASHER_H

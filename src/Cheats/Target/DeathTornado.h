//
// Created by Administrator on 2024/10/7.
//

#ifndef DEATHTORNADO_H
#define DEATHTORNADO_H


#include "../Vehicle/GocModule.h"

namespace CheatModule::Target {
    class DeathTornado : public Vehicle::GocModule {
    public:
        DeathTornado();
        void onProcess() override;
    };
}
// CheatModule


#endif //DEATHTORNADO_H

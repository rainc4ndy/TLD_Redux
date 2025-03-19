//
// Created by Administrator on 2024/10/7.
//

#ifndef DEATHCROSS_H
#define DEATHCROSS_H


#include "../Vehicle/GocModule.h"

namespace CheatModule::Target {
    class DeathCross : public Vehicle::GocModule {
    private:
        int m_iState_DeadCross;
    public:
        DeathCross();
        void onProcess() override;
    };
}
// CheatModule



#endif //DEATHCROSS_H

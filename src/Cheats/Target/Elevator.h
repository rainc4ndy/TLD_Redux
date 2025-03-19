//
// Created by Administrator on 2024/10/7.
//

#ifndef ELEVATOR_H
#define ELEVATOR_H


#include "../Vehicle/GocModule.h"

namespace CheatModule::Target {
    class Elevator : public Vehicle::GocModule {
    public:
        Elevator();
        void onProcess() override;
    };
}
// CheatModule


#endif //ELEVATOR_H

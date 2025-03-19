//
// Created by Administrator on 2024/10/6.
//

#ifndef CARLAGGER_H
#define CARLAGGER_H

#include <CVector.h>

#include "GocModule.h"

namespace CheatModule::Vehicle {

class CarLagger : public GocModule {
private:
    CVector old_pos;
public:
    CarLagger();
    void onProcess() override;
};

} // Vehicle
// CheatModule

#endif //CARLAGGER_H

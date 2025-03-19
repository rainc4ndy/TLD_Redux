//
// Created by Administrator on 2024/10/6.
//

#ifndef CARSTORM_H
#define CARSTORM_H

#include <CVector.h>

#include "GocModule.h"
namespace CheatModule {
    namespace Vehicle {
        class CarStorm : public GocModule {
        private:
            float fStormAngle;
        public:
            CarStorm();
            void onProcess() override;
        };

    } // Vehicle
} // CheatModule

#endif //CARSTORM_H

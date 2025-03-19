//
// Created by Administrator on 2024/10/6.
//

#ifndef CARWALL_H
#define CARWALL_H

#include <CVector.h>

#include "GocModule.h"
namespace CheatModule {
    namespace Vehicle {
        class CarWall : public GocModule {
        private:
            int current_point;
        public:
            CarWall();
            void onProcess() override;
        };

    } // Vehicle
} // CheatModule


#endif //CARWALL_H

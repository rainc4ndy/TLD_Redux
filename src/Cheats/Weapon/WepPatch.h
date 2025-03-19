//
// Created by Administrator on 2025/2/15.
//

#ifndef TLD_REDUX_WEPPATCH_H
#define TLD_REDUX_WEPPATCH_H

#include "../../CPatchModule.h"

namespace CheatModule::Weapon {
    class AllAxisAim : public CPatchModule{
    private:
        void work() override;
    public:
        AllAxisAim();
    };

} // CheatModule

#endif //TLD_REDUX_WEPPATCH_H

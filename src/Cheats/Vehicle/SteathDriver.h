#ifndef A5B3A9D0_379D_4866_A8E1_2D321759837E
#define A5B3A9D0_379D_4866_A8E1_2D321759837E

#include "../../CCheatModule.h"


namespace CheatModule::Vehicle {
    class SteathDriver : public CCheatModule {
    public:
        SteathDriver();
        bool onProcess(stInCarData *data) override;
        void onDisable() override;
    };
}


#endif /* A5B3A9D0_379D_4866_A8E1_2D321759837E */

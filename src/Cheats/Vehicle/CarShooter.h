#ifndef C924A0F0_2083_4CD8_BB70_810D3A89B9ED
#define C924A0F0_2083_4CD8_BB70_810D3A89B9ED

#include "GocModule.h"
#include "../../CCheatModule.h"

namespace CheatModule::Vehicle {
    class CarShooter : public GocModule {
    private:
    public:
        CarShooter();
        void onProcess() override;
    };
}


#endif /* C924A0F0_2083_4CD8_BB70_810D3A89B9ED */

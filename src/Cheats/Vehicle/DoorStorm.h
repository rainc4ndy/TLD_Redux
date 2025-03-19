#ifndef A74D3300_012C_4565_AF78_9A31F9766DE0
#define A74D3300_012C_4565_AF78_9A31F9766DE0

#include "../../CCheatModule.h"

namespace CheatModule::Vehicle {
    class DoorStorm : public CCheatModule {
    private:
        int tick;
        bool bDoorStormState;
    public:
        DoorStorm();
        void onProcess() override;
    };
}


#endif /* A74D3300_012C_4565_AF78_9A31F9766DE0 */

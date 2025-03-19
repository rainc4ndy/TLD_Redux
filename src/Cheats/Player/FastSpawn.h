#ifndef B9CF7151_3CD4_4FAA_98A9_1CA8A1308312
#define B9CF7151_3CD4_4FAA_98A9_1CA8A1308312


#include <RakHook/rakhook.hpp>

#include "../../CCheatModule.h"
namespace CheatModule
{
namespace Player 
{

class FastSpawn : public CCheatModule
{
public:
    FastSpawn(/* args */);
    void onProcess() override;
};

}
}

#endif /* B9CF7151_3CD4_4FAA_98A9_1CA8A1308312 */

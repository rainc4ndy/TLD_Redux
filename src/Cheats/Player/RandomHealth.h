#ifndef B8530846_6D4E_4CD2_BA80_14DC3923ADBD
#define B8530846_6D4E_4CD2_BA80_14DC3923ADBD


#include "../../CCheatModule.h"

namespace CheatModule
{
namespace Player 
{
class RandomHealth : public CCheatModule {
private:
    bool isReversed;
public:
    RandomHealth();
    void onProcess() override;
};
}
} // namespace CheatModule


#endif /* B8530846_6D4E_4CD2_BA80_14DC3923ADBD */

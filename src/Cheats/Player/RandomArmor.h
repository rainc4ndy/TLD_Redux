#ifndef E03E48E4_8ECD_4467_8478_914229314861
#define E03E48E4_8ECD_4467_8478_914229314861

#include "../../CCheatModule.h"


namespace CheatModule
{
namespace Player 
{
class RandomArmor : public CCheatModule {
private:
    bool isReversed;
public:
    RandomArmor();
    void onProcess() override;
};
}
} // namespace CheatModule



#endif /* E03E48E4_8ECD_4467_8478_914229314861 */

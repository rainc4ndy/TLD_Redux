#ifndef E73B049B_8A91_4FD2_BB43_FEA76BAB39E9
#define E73B049B_8A91_4FD2_BB43_FEA76BAB39E9

#include "../../CCheatModule.h"

namespace CheatModule
{
namespace Player 
{
class Twister : public CCheatModule {
public:
    enum TwisterMode {
        TW_Twisted,
        TW_MJ,
        TW_UPDOWN,
        TW_Spin
    };
    int mode;
    Twister();
    bool onProcess(stOnFootData *data) override;

    void onSave(nlohmann::json &j) override;
    void onLoad(const nlohmann::json &j) override;
};
}
} // namespace CheatModule


#endif /* E73B049B_8A91_4FD2_BB43_FEA76BAB39E9 */

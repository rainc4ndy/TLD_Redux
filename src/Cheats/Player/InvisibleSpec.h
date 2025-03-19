#ifndef D9FADC43_1358_4355_B0AB_64E3B1D3459A
#define D9FADC43_1358_4355_B0AB_64E3B1D3459A

// Ensure CCheatModule is included first
#include "../../CCheatModule.h"

namespace CheatModule
{
namespace Player 
{
class InvisibleSpec : public CCheatModule {
private:
    bool m_bInvisDetonState;
public:
    enum InvisMode {
        Invis_Pos,
        Invis_Spec,
        Invis_Deton,
        Invis_SurfVehicle,
        Invis_SurfObject
    };
    int mode;
    InvisibleSpec();
    bool onProcess(stOnFootData *data) override;
    bool onProcess(stAimData *data) override;

    void onSave(nlohmann::json &j) override;
    void onLoad(const nlohmann::json &j) override;
};
}
} // namespace CheatModule


    

#endif /* D9FADC43_1358_4355_B0AB_64E3B1D3459A */

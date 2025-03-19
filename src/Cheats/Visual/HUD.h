#ifndef B1358D92_2CCB_4BC2_B74C_311DF8B72E57
#define B1358D92_2CCB_4BC2_B74C_311DF8B72E57

#include "../../CCheatModule.h"
#include <imgui.h>

namespace CheatModule {
namespace Visual {
    
class HUD : public CCheatModule
{
private:
    unsigned long color;

    void col_hud(DWORD color);
    void col_textdraws(ImVec4 *color);
    void col_menu(ImVec4 *color);
    void col_weapon(ImVec4 *color);
    void col_gameInput(ImVec4 *color, DWORD color_x);
    void col_carLight(ImVec4 *color);
    void col_crossHair(ImVec4 *color);
    void col_radar(ImVec4 *color);

public:
    HUD(/* args */);

    bool bVisRainbow;
    bool bVisHUD;
	bool bVisTextdraw;
	bool bVisWeaponSlot;
	bool bVisMenu;
	bool bVisInput;
	bool bVisCarLight;
	bool bVisCrossHair;
	bool bVisRadar;

    ImVec4 vVisColor;
    unsigned long dVisColorHex;

    void onProcessVisual() override;

	void onSave(nlohmann::json &j) override;
	void onLoad(const nlohmann::json &j) override;
};

}
}


#endif /* B1358D92_2CCB_4BC2_B74C_311DF8B72E57 */

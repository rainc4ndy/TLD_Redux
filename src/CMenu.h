#ifndef C367F380_4450_4C55_9EE7_46A13F6FE2ED
#define C367F380_4450_4C55_9EE7_46A13F6FE2ED

#include <initializer_list>
#include <string>
#include <map>
#include <d3dx9.h>

#include "CBackground.h"
class CCheatModule;

class CMenu
{
private:
    bool enabled;
    std::map<std::string, bool> options_state;

    enum Category {
        Category_Player,
        Category_Vehicle,
        Category_Weapon,
        Category_Target,
        Category_Visual,
        Category_Hack,
        Category_Bot,
        Category_Setting
    };

    int currentCategory;
    void renderGoc();
public:

    void renderModuleCheckbox(CCheatModule* module);

    D3DPRESENT_PARAMETERS pPresentParam;
    float menuTransparency;
    CBackground backgroundImage;

    CMenu(/* args */);
    ~CMenu();

    void setEnable(bool enable);
    bool isEnabled();
    
    void RenderBackground();
    void RenderSidebarButtons(std::initializer_list<std::string> v);
    void render();

    void enableCursor(bool enable);
private:
    void render_menu_bar();
};



#endif /* C367F380_4450_4C55_9EE7_46A13F6FE2ED */

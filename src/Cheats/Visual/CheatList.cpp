#include <vector>
#include <string>

#include "CheatList.h"
#include "../../CCheatManager.h"
#include "../../CCheatModule.h"
#include "../../CRenderer.h"
#include "../../d3drender.h"
#include "../../CMenu.h"
#include "../../Plugin.h"
#include "../../HueCycler.h"

namespace CheatModule {
namespace Visual {
CheatList::CheatList() : CCheatModule("模块列表", "Module View") { enabled = true; }
void CheatList::onProcessVisual() {
    auto r = Plugin::getInstance().getCheat()->getAll();
    std::vector<std::string> v(r.size());

    int index = 0;
    for (auto& it : r) {
        if (it->isEnabled())
            v[index++] = it->getNameEn();
    }
    
    static auto cmp = [](std::string& a, std::string& b) {
        return a.size() > b.size();
    };
    std::sort(begin(v), end(v), cmp);

    auto d = Plugin::getInstance().getRenderer();
    auto hue = Plugin::getInstance().getHueCycler();
    auto color = hue->currentRgb();
    auto para = Plugin::getInstance().getMenu()->pPresentParam;

    int screen_x = para.BackBufferWidth;
    int screen_y = para.BackBufferHeight;

    // 设置起始位置和间距
    float pos_x = static_cast<float>(screen_x) - 20;  // 靠近右侧，留出边距
    float pos_y = 20;              // 初始 Y 位置
    float line_spacing = 20;       // 每行之间的间距

    for (auto& it : v) {
        // 计算每行文本的宽度以进行右对齐
        float textWidth = d->screenFont->DrawLength(it.c_str());
        d->screenFont->PrintShadow(pos_x - textWidth, pos_y, 
            D3DCOLOR_XRGB((int)(color.r * 255), (int)(color.g * 255), (int)(color.b * 255)), 
            it.c_str());
        pos_y += line_spacing;  // 更新 Y 位置
    }
}

}  // namespace Visual
}
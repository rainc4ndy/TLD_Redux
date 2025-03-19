#include "test_render.h"

#include <d3d9.h>
#include <tchar.h>

#include <initializer_list>
#include <iostream>
#include <string>
#include <iomanip>  // for std::hex and std::setw
#include <vector>
#include <unordered_map>
#include <filesystem>
#include <set>
#include <istream>

namespace fs = std::filesystem;

#include "HueCycler.h"
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include "test_render.h"
#include "CBinaryPacker.h"
#include "CTexture.h"

static HueCycler cycler(600);

std::unordered_map<std::string, CTexture> mp;

void printHex(const unsigned char* array, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        // Print each byte as a two-digit hexadecimal number
        std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(array[i]) << " ";
    }
    std::cout << std::dec << std::endl;  // Switch back to decimal format after printing
}

void init() {
    CBinaryPacker pck;
    pck.open("data.bin", true);
    
    stBinaryHeader header;
    while (pck.nextHeader(header))
    {
        std::cout << "dumping " << header.fileNameStr << std::endl;
        auto data = new unsigned char[header.size];
        pck.dump(data, header.size);

        printHex(data, 32);
        mp[header.fileNameStr] = CTexture();
        auto r = &mp[header.fileNameStr];
        r->fromRaw(data, header.size);

        delete data;
    }
}

void DrawRainbowBar(float height) {
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImGuiStyle* style = &ImGui::GetStyle();

    auto sz = ImGui::GetWindowSize();
    auto pos = ImGui::GetCursorScreenPos();
    auto c = cycler.currentRgb();
    // Draw each band
    drawList->AddRectFilled(
        ImVec2(pos.x - style->WindowPadding.x, pos.y - style->WindowPadding.y),
        ImVec2(pos.x + sz.x, pos.y + height),
        ImColor(ImVec4(c.r, c.g, c.b, 1.0f)));

    cycler.setCurrent();   // Set current color based on index
    ImGui::SetCursorScreenPos(ImVec2(pos.x, pos.y + height + style->WindowPadding.y));
    cycler += 1; 
}

void RenderSidebarButtons(std::vector<std::string>& v) {
    auto sz = ImGui::GetWindowSize();
    sz.y = (sz.y / v.size());
    for (auto& it : v) {
        ImGui::Button(it.c_str(), sz);
    }
}
void RenderSidebarButtons(std::initializer_list<std::string> v) {

    ImGuiStyle *style = &ImGui::GetStyle();

    float length = ImGui::GetWindowSize().x - (style->WindowPadding.x * 2);
    ImVec2 sz(length, length);

    int i = 0;
    for (auto &it: v) {
        auto tex = mp[it];
        if (ImGui::ImageButton(it.c_str(), ImTextureID((void *) tex.getTexture()), sz)) {
        }
        i++;
    }

}

void render_my_test() {
    static bool inited = false;
    if (!inited) {
        init();
        inited = true;
    }
    auto rainbow = cycler.currentRgb();
    auto drawlist = ImGui::GetBackgroundDrawList();

    static bool menu_open = true;
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(rainbow.r, rainbow.g, rainbow.b, 1.0f));
    ImGui::Begin(
        "The Last Dancer - Redux", &menu_open,
        ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollWithMouse);
    ImGui::PopStyleColor();
    
    const int min_size_h1 = 100;

    auto sz = ImGui::GetWindowSize();
    auto sz_1 = sz, sz_2 = sz;
    sz_1.x = max(sz.x * 0.2f, min_size_h1);
    sz_2.x = sz.x * 0.8f;

    // Set the position and size for the rainbow bar

    // Draw the rainbow bar
    DrawRainbowBar(2);
    ImGui::BeginChild("horizon_1", sz_1, 0, ImGuiWindowFlags_NoScrollbar);
    RenderSidebarButtons({
        "player.png", "vehicle.png", "weapon.png", "target.png",
        "visual.png", "hack.png", "bot.png", "settings.png"
    });
    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild("horizon_2", sz_2, 0, ImGuiWindowFlags_NoScrollbar);

    ImGui::EndChild();


    ImGui::End();
    cycler.setCurrent();
    cycler += 1; 
}

#include "CBackground.h"

#include <imgui.h>

#include "Plugin.h"
#include "CTextureManager.h"

void CBackground::draw() {
    auto tex = Plugin::getInstance().getTexs();

    auto ori = ImGui::GetCursorPos();
    ImGui::SetCursorPos(ImVec2(0, 0));

    ImVec2 windowSize = ImGui::GetWindowSize();
    CTexture* bg = tex->getTexture("background");

    // Calculate the aspect ratios
    float aspect_image = static_cast<float>(bg->getWidth()) / bg->getHeight();
    float aspect_window = static_cast<float>(windowSize.x) / windowSize.y;

    float new_width, new_height;

    // Determine the new size based on aspect ratio and zoom
    if (aspect_image > aspect_window) {
        // Scale based on width
        new_width = windowSize.x * zoom;
        new_height = new_width / aspect_image;
    } else {
        // Scale based on height
        new_height = windowSize.y * zoom;
        new_width = new_height * aspect_image;
    }

    // Calculate position to center the image with offsets
    float x = (windowSize.x - new_width) / 2 + center_x;
    float y = (windowSize.y - new_height) / 2 + center_y;

    // Draw the image
    ImGui::SetCursorPos(ImVec2(x, y));
    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, transparency);
    ImTextureID textureID = reinterpret_cast<ImTextureID>(bg->getTexture());
    ImGui::Image(textureID , ImVec2(new_width, new_height));
    ImGui::PopStyleVar();
    ImGui::SetCursorPos(ori); // Restore cursor position
}
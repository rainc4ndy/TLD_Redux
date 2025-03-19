//
// Created by Administrator on 2024/10/9.
//

#include "colors.h"

ImColor D3DColorToImColor(D3DCOLOR d3dColor) {
    // Extract the color components from the D3DCOLOR value
    int r = (d3dColor >> 16) & 0xFF; // Red component
    int g = (d3dColor >> 8) & 0xFF; // Green component
    int b = d3dColor & 0xFF; // Blue component
    int a = (d3dColor >> 24) & 0xFF; // Alpha component

    // Create and return the ImColor
    return ImColor(r, g, b, a);
}

D3DCOLOR ImColorToD3DColor(const ImColor& imColor) {
    // Extract the color components from ImColor
    int r = static_cast<int>(imColor.Value.x * 255); // Red component
    int g = static_cast<int>(imColor.Value.y * 255); // Green component
    int b = static_cast<int>(imColor.Value.z * 255); // Blue component
    int a = static_cast<int>(imColor.Value.w * 255); // Alpha component

    // Pack the components into a D3DCOLOR
    return D3DCOLOR_ARGB(a, r, g, b);
}

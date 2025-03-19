//
// Created by Administrator on 2024/10/9.
//

#ifndef COLORS_H
#define COLORS_H

#include <d3dx9.h>
#include <imgui.h>
ImColor D3DColorToImColor(D3DCOLOR d3dColor);
D3DCOLOR ImColorToD3DColor(const ImColor& imColor);

#endif //COLORS_H

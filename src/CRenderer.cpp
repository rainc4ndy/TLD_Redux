#include "CRenderer.h"

#include "d3drender.h"

CRenderer::CRenderer() {
    screenFont = new CD3DFont("Tahoma", 10, FCR_BORDER);
    infoFont = new CD3DFont( "Small Fonts", 8, FCR_BORDER );
    renderer = new CD3DRender(128);
    isInit = false;
}

CRenderer::~CRenderer() {
    uninit();
    delete screenFont;
    delete infoFont;
    delete renderer;
}

void CRenderer::init(IDirect3DDevice9* dev) {
    screenFont->Initialize(dev);
    infoFont->Initialize(dev);
    renderer->Initialize(dev);
    isInit = true;
}

void CRenderer::uninit() {
    screenFont->Invalidate();
    infoFont->Invalidate();
    renderer->Invalidate();
    isInit = false;
}

bool CRenderer::IsInited() {
    return isInit;
}

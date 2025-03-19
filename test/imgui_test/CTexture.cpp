// Include Windows and DirectX headers
#include <windows.h>               // Windows API header
#include <d3d9.h>                 // Direct3D 9 API header
#include <d3dx9.h>                // Direct3D extensions for DirectX 9

// Standard libraries (optional, depending on your usage)
#include <iostream>               // For console output (if needed)
#include <stdexcept>              // For exception handling (if needed)

#include "CTexture.h"

LPDIRECT3DDEVICE9 CTexture::device = 0;

void CTexture::setDevice(LPDIRECT3DDEVICE9 device) { CTexture::device = device; }

bool CTexture::fromRaw(unsigned char* raw, size_t size) {
    // Load texture from disk
    using namespace std;

    PDIRECT3DTEXTURE9 tex;
    D3DXIMAGE_INFO info;
    HRESULT hr = D3DXCreateTextureFromFileInMemoryEx(
        device,                   // Direct3D device
        raw,                      // Pointer to the raw image data
        static_cast<UINT>(size), // Size of the raw image data
        D3DX_DEFAULT,            // Width (D3DX_DEFAULT to use original width)
        D3DX_DEFAULT,            // Height (D3DX_DEFAULT to use original height)
        0,                       // MipLevels (0 for all levels)
        D3DUSAGE_DYNAMIC,        // Usage (change based on your needs)
        D3DFMT_UNKNOWN,          // Format (auto-detect)
        D3DPOOL_DEFAULT,         // Memory pool
        D3DX_DEFAULT,            // Filter (default filtering)
        D3DX_DEFAULT,            // MipFilter (default mip filtering)
        0,                       // Color key (no color key)
        &info,             // Pointer to receive image info
        nullptr,                 // No palette
        &tex                     // Pointer to the created texture
    );

    if (hr != S_OK)
        return false;

    // Retrieve description of the texture surface so we can access its size
    //D3DSURFACE_DESC my_image_desc;
    texture = tex;
    //texture->GetLevelDesc(0, &my_image_desc);
    width = (int)info.Width;
    height = (int)info.Height;
    return true;
} 

PDIRECT3DTEXTURE9 CTexture::getTexture() const { return texture; }
int CTexture::getWidth() const { return width; }
int CTexture::getHeight() const { return height; }

void CTexture::release() {
    texture->Release();
}

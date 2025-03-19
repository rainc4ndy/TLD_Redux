#ifndef B4FD9984_1057_49FD_8295_7FBE8813E86C
#define B4FD9984_1057_49FD_8295_7FBE8813E86C

#include <d3dx9.h>

class CTexture
{
private:
    /* data */
    static LPDIRECT3DDEVICE9 device;
    
    PDIRECT3DTEXTURE9 texture;
    int width, height;
public:
    CTexture(/* args */) = default;
    ~CTexture() = default;

    static void setDevice(LPDIRECT3DDEVICE9 device);
    bool fromRaw(unsigned char* raw, size_t size);

    PDIRECT3DTEXTURE9 getTexture() const;
    int getWidth() const;
    int getHeight() const;

    void release();
};


#endif /* B4FD9984_1057_49FD_8295_7FBE8813E86C */

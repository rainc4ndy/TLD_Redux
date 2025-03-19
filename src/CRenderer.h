#ifndef C7FAB77F_819D_4347_8861_C05D65DFBE67
#define C7FAB77F_819D_4347_8861_C05D65DFBE67

class CD3DFont;
class CD3DRender;
struct IDirect3DDevice9;

class CRenderer
{
private:
    bool isInit;
public:
    CRenderer(/* args */);
    ~CRenderer();
    
    void init(IDirect3DDevice9 *dev);
    void uninit();
    bool IsInited();

    CD3DRender *renderer;
    CD3DFont *screenFont;
    CD3DFont *infoFont;
};



#endif /* C7FAB77F_819D_4347_8861_C05D65DFBE67 */

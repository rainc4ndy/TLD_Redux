#include <Windows.h>
#include <d3d9.h>
#include <imgui_impl_dx9.h>
#include <imgui_impl_win32.h>

#include <kthook/kthook.hpp>

#include "CCheatManager.h"
#include "CConfig.h"
#include "CLog.h"
#include "CMenu.h"
#include "CRenderer.h"
#include "CTextureManager.h"
#include "HueCycler.h"
#include "Plugin.h"
#include "default_style.h"
#include "keyhook.h"

#include "d3dhook.h"

bool initStatus = false;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd,
                                                             UINT msg,
                                                             WPARAM wParam,
                                                             LPARAM lParam);

using PresentSignature = HRESULT(__stdcall*)(IDirect3DDevice9*, const RECT*,
                                             const RECT*, HWND, const RGNDATA*);
using ResetSignature = HRESULT(__stdcall*)(IDirect3DDevice9*,
                                           D3DPRESENT_PARAMETERS*);
using InitGameInstance = HWND(__cdecl*)(HINSTANCE);

std::uintptr_t find_device(std::uint32_t Len) {
    static std::uintptr_t base = [](std::size_t Len) {
        std::string path_to(MAX_PATH, '\0');
        if (auto size = GetSystemDirectoryA(path_to.data(), MAX_PATH)) {
            path_to.resize(size);
            path_to += "\\d3d9.dll";
            std::uintptr_t dwObjBase =
                reinterpret_cast<std::uintptr_t>(LoadLibraryA(path_to.c_str()));
            while (dwObjBase++ < dwObjBase + Len) {
                if (*reinterpret_cast<std::uint16_t*>(dwObjBase + 0x00) ==
                        0x06C7 &&
                    *reinterpret_cast<std::uint16_t*>(dwObjBase + 0x06) ==
                        0x8689 &&
                    *reinterpret_cast<std::uint16_t*>(dwObjBase + 0x0C) ==
                        0x8689) {
                    dwObjBase += 2;
                    break;
                }
            }
            return dwObjBase;
        }
        return std::uintptr_t(0);
    }(Len);
    return base;
}

void* get_function_address(int VTableIndex) {
    return (*reinterpret_cast<void***>(find_device(0x128000)))[VTableIndex];
}

kthook::kthook_signal<InitGameInstance> game_instance_init_hook{0x745560};

HWND game_hwnd = []() {
    HWND* hwnd_ptr = *reinterpret_cast<HWND**>(0xC17054);
    if (hwnd_ptr != nullptr) {
        return *hwnd_ptr;
    } else {
        game_instance_init_hook.after +=
            [](const auto& hook, HWND& return_value, HINSTANCE inst) {
                game_hwnd = return_value;
            };
        return HWND(0);
    }
}();

kthook::kthook_signal<PresentSignature> present_hook{get_function_address(17)};
kthook::kthook_signal<ResetSignature> reset_hook{get_function_address(16)};
kthook::kthook_simple<WNDPROC> wndproc_hook{};

HRESULT __stdcall on_wndproc(const decltype(wndproc_hook)& hook, HWND hwnd,
                             UINT uMsg, WPARAM wParam, LPARAM lParam) {

    CMenu* m = Plugin::getInstance().getMenu();
    // gta_menu_active
    if (( *reinterpret_cast<uint8_t *>(0x00BA67A4) )) {
        goto wnd_out;
    }

    keyhook_process ( hwnd,  uMsg,  wParam,  lParam );
    switch (uMsg) {
        case WM_KEYDOWN: {
            // samp::RefChat()->AddMessage(-1,
            // std::to_string(Plugin::getInstance().getConfig()->menu_key).c_str());

            if (wParam == Plugin::getInstance().getConfig()->menu_key &&
                (HIWORD(lParam) & KF_REPEAT) != KF_REPEAT) {
                if (m->isEnabled()) {
                    m->enableCursor(false);
                    m->setEnable(false);
                } else {
                    m->enableCursor(true);
                    m->setEnable(true);
                }
            }
            break;
        }
    }
    ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam);
    if (m->isEnabled()) {
        ImGuiIO& io = ImGui::GetIO();
        if (io.WantTextInput) {
            return false;
        }
    }
    wnd_out:
    return hook.get_trampoline()(hwnd, uMsg, wParam, lParam);
}

std::optional<HRESULT> on_present(const decltype(present_hook)& hook,
                                  IDirect3DDevice9* device_ptr, const RECT*,
                                  const RECT*, HWND, const RGNDATA*) {

    static bool ImGui_inited = false;
    if (!ImGui_inited) {
        ImGui::CreateContext();
        ImGui_ImplWin32_Init(game_hwnd);
        ImGui_ImplDX9_Init(device_ptr);

        auto setting = Plugin::getInstance().getConfig();
        SetupImGuiStyle(setting->color_theme);
        ImGuiIO& io = ImGui::GetIO();

        io.Fonts->AddFontFromFileTTF(
            (Plugin::getInstance().getDirectory() + '/' + setting->font_str)
                .c_str(),
            setting->font_size, NULL, io.Fonts->GetGlyphRangesChineseFull());
        io.Fonts->Build();

        auto latest_wndproc_ptr = GetWindowLongPtrA(game_hwnd, GWLP_WNDPROC);
        wndproc_hook.set_dest(latest_wndproc_ptr);
        wndproc_hook.set_cb(&on_wndproc);
        wndproc_hook.install();
        ImGui_inited = true;

        Plugin::getInstance().getLog()->log("ImGui_inited finished");
    }

    auto r = Plugin::getInstance().getCheat()->getAll();
    for (auto& it : r) {
        if (it->isEnabled()) it->onProcessVisual();
    }
    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    if (Plugin::getInstance().getMenu()->isEnabled()) {
        Plugin::getInstance().getMenu()->render();
    }
    Plugin::getInstance().getCheat()->console.draw();

    ImGui::EndFrame();
    ImGui::Render();
    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

    *Plugin::getInstance().getHueCycler() += 1;

    return std::nullopt;
}

std::optional<HRESULT> on_lost(const decltype(reset_hook)& hook,
                               IDirect3DDevice9* device_ptr,
                               D3DPRESENT_PARAMETERS* parameters) {
    ImGui_ImplDX9_InvalidateDeviceObjects();
    auto r = Plugin::getInstance().getRenderer();
    auto t = Plugin::getInstance().getTexs();
    if (initStatus) {
        r->uninit();
        t->clear();
        initStatus = false;
    }

    return std::nullopt;
}

bool isBadPtr_handlerAny(void* pointer, ULONG size, DWORD dwFlags) {
    DWORD dwSize;
    MEMORY_BASIC_INFORMATION meminfo;

    if (NULL == pointer) return true;

    memset(&meminfo, 0x00, sizeof(meminfo));
    dwSize = VirtualQuery(pointer, &meminfo, sizeof(meminfo));

    if (0 == dwSize) return true;

    if (MEM_COMMIT != meminfo.State) return true;

    if (0 == (meminfo.Protect & dwFlags)) return true;

    if (size > meminfo.RegionSize) return true;

    if ((unsigned)((char*)pointer - (char*)meminfo.BaseAddress) >
        (unsigned)(meminfo.RegionSize - size))
        return true;

    return false;
}

bool isBadPtr_readAny(void* pointer, ULONG size) {
    return isBadPtr_handlerAny(pointer, size,
                               PAGE_READONLY | PAGE_READWRITE | PAGE_WRITECOPY |
                                   PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE |
                                   PAGE_EXECUTE_WRITECOPY);
}

bool isBadPtr_writeAny(void* pointer, ULONG size) {
    return isBadPtr_handlerAny(pointer, size,
                               PAGE_READWRITE | PAGE_WRITECOPY |
                                   PAGE_EXECUTE_READWRITE |
                                   PAGE_EXECUTE_WRITECOPY);
}

void on_reset(const decltype(reset_hook)& hook, HRESULT& return_value,
              IDirect3DDevice9* device_ptr, D3DPRESENT_PARAMETERS* parameters) {

    if (return_value == D3D_OK) {
        // variable for checking the pointers
        short badPointerCount_g_pGTAPresent = 0;
        bool badPointerBreak_g_pGTAPresent = false;
        short badPointerCount_pPresentationParameters = 0;
        bool badPointerBreak_pPresentationParameters = false;
        D3DPRESENT_PARAMETERS	*g_pGTAPresent = (D3DPRESENT_PARAMETERS *)0xC9C040;
        

        while (isBadPtr_writeAny(parameters, sizeof(D3DPRESENT_PARAMETERS)) &&
               !badPointerBreak_pPresentationParameters) {
            badPointerCount_pPresentationParameters++;
            if (badPointerCount_pPresentationParameters < 50) {
                Sleep(100);
            } else {
                Plugin::getInstance().getLog()->log(
                    "During D3D9 Reset(), pPresentationParameters was bad for "
                    "over 5 seconds. Continuing anyways.");
                badPointerBreak_pPresentationParameters = true;
            }
        }
        auto r = Plugin::getInstance().getRenderer();

        while ( !initStatus )
        {
            r->init(device_ptr);
            CTexture::setDevice(device_ptr);
            Plugin::getInstance().getTexs()->loadFromBin(
                Plugin::getInstance().getDirectory() + "/data.bin");
            Plugin::getInstance().getTexs()->loadFromFile("background",
                Plugin::getInstance().getDirectory() + "/bg.png");

            initStatus = true;
            if ( !initStatus)
                Sleep( 100 );
        }
        Plugin::getInstance().getMenu()->pPresentParam = *parameters;
    }
}

void SetD3D9Hooks() {
    present_hook.before += on_present;
    reset_hook.before += on_lost;
    reset_hook.after += on_reset;

    Plugin::getInstance().getLog()->log("D3D9 hook setup finished");
}

void DestoryD3D9Hooks() {}

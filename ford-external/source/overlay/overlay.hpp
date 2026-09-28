#ifndef overlay_hpp
#define overlay_hpp

#include <Windows.h>
#include <d3d11.h>
#include <dxgi1_2.h>
#include <dcomp.h>
#include <dwmapi.h>

struct overlay_t
{
    HWND hwnd;
    int width;
    int height;

    bool vsync = true;

    ID3D11Device* device;
    ID3D11DeviceContext* context;
    IDXGISwapChain1* swap_chain;
    ID3D11RenderTargetView* render_target;

    IDCompositionDevice* comp_device;
    IDCompositionTarget* comp_target;
    IDCompositionVisual* comp_visual;

    overlay_t( ) = default;

    bool init( );
    bool begin_frame( );
    void end_frame( );
    void shutdown( );

private:
    bool create_window( );
    bool create_device( );
    bool create_swap_chain( );
    bool create_dcomp( );
    void release_render_target( );
    bool create_render_target( );
};

inline overlay_t g_overlay;

#endif  // overlay_hpp

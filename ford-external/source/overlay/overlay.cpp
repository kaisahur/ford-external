#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

#include <features/visuals.hpp>
#include <framework/globals.hpp>
#include <overlay/overlay.hpp>

#pragma comment( lib, "d3d11.lib" )
#pragma comment( lib, "dxgi.lib" )
#pragma comment( lib, "dcomp.lib" )
#pragma comment( lib, "dwmapi.lib" )

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam );

static bool g_menu_open = false;
static bool g_insert_was_down = false;

static LRESULT CALLBACK wnd_proc( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam )
{
    if ( ImGui_ImplWin32_WndProcHandler( hwnd, msg, wparam, lparam ) )
        return true;

    if ( msg == WM_DESTROY )
    {
        PostQuitMessage( 0 );
        return 0;
    }
    return DefWindowProcW( hwnd, msg, wparam, lparam );
}

bool overlay_t::create_window( )
{
    WNDCLASSEX wc{};
    wc.cbSize = sizeof( wc );
    wc.lpfnWndProc = wnd_proc;
    wc.hInstance = GetModuleHandleW( nullptr );
    wc.lpszClassName = "overlay_cls";

    if ( !RegisterClassExA( &wc ) )
        return false;

    width = GetSystemMetrics( SM_CXSCREEN );
    height = GetSystemMetrics( SM_CYSCREEN );

    hwnd = CreateWindowExA(
        WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOPMOST | WS_EX_NOACTIVATE,
        wc.lpszClassName,
        "overlay",
        WS_POPUP,
        0,
        0,
        width,
        height,
        nullptr,
        nullptr,
        wc.hInstance,
        nullptr );

    if ( !hwnd )
        return false;

    SetLayeredWindowAttributes( hwnd, 0, 255, LWA_ALPHA );

    MARGINS margins{ -1, -1, -1, -1 };
    DwmExtendFrameIntoClientArea( hwnd, &margins );

    ShowWindow( hwnd, SW_SHOW );
    UpdateWindow( hwnd );

    return true;
}

bool overlay_t::create_device( )
{
    D3D_FEATURE_LEVEL feature_levels[] = { D3D_FEATURE_LEVEL_11_0 };
    D3D_FEATURE_LEVEL obtained_level;

    HRESULT hr = D3D11CreateDevice(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        D3D11_CREATE_DEVICE_BGRA_SUPPORT,
        feature_levels,
        1,
        D3D11_SDK_VERSION,
        &device,
        &obtained_level,
        &context );

    return SUCCEEDED( hr );
}

bool overlay_t::create_swap_chain( )
{
    IDXGIDevice* dxgi_device = nullptr;
    if ( FAILED( device->QueryInterface( __uuidof( IDXGIDevice ), reinterpret_cast< void** >( &dxgi_device ) ) ) )
        return false;

    IDXGIAdapter* adapter = nullptr;
    if ( FAILED( dxgi_device->GetAdapter( &adapter ) ) )
    {
        dxgi_device->Release( );
        return false;
    }

    IDXGIFactory2* factory = nullptr;
    if ( FAILED( adapter->GetParent( __uuidof( IDXGIFactory2 ), reinterpret_cast< void** >( &factory ) ) ) )
    {
        adapter->Release( );
        dxgi_device->Release( );
        return false;
    }

    DXGI_SWAP_CHAIN_DESC1 desc{};
    desc.Width = width;
    desc.Height = height;
    desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    desc.BufferCount = 2;
    desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
    desc.AlphaMode = DXGI_ALPHA_MODE_PREMULTIPLIED;

    HRESULT hr = factory->CreateSwapChainForComposition( device, &desc, nullptr, &swap_chain );

    factory->Release( );
    adapter->Release( );
    dxgi_device->Release( );

    return SUCCEEDED( hr );
}

bool overlay_t::create_dcomp( )
{
    IDXGIDevice* dxgi_device = nullptr;
    if ( FAILED( device->QueryInterface( __uuidof( IDXGIDevice ), reinterpret_cast< void** >( &dxgi_device ) ) ) )
        return false;

    HRESULT hr = DCompositionCreateDevice( dxgi_device, __uuidof( IDCompositionDevice ), reinterpret_cast< void** >( &comp_device ) );
    dxgi_device->Release( );

    if ( FAILED( hr ) )
        return false;

    if ( FAILED( comp_device->CreateTargetForHwnd( hwnd, TRUE, &comp_target ) ) )
        return false;

    if ( FAILED( comp_device->CreateVisual( &comp_visual ) ) )
        return false;

    if ( FAILED( comp_visual->SetContent( swap_chain ) ) )
        return false;

    if ( FAILED( comp_target->SetRoot( comp_visual ) ) )
        return false;

    if ( FAILED( comp_device->Commit( ) ) )
        return false;

    return true;
}

bool overlay_t::create_render_target( )
{
    ID3D11Texture2D* back_buffer = nullptr;
    if ( FAILED( swap_chain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), reinterpret_cast< void** >( &back_buffer ) ) ) )
        return false;

    HRESULT hr = device->CreateRenderTargetView( back_buffer, nullptr, &render_target );
    back_buffer->Release( );

    return SUCCEEDED( hr );
}

void overlay_t::release_render_target( )
{
    if ( render_target )
    {
        render_target->Release( );
        render_target = nullptr;
    }
}

bool overlay_t::init( )
{
    if ( !create_window( ) )
        return false;

    if ( !create_device( ) )
        return false;

    if ( !create_swap_chain( ) )
        return false;

    if ( !create_dcomp( ) )
        return false;

    if ( !create_render_target( ) )
        return false;

    IMGUI_CHECKVERSION( );
    ImGui::CreateContext( );

    ImGuiIO& io = ImGui::GetIO( );
    io.IniFilename = nullptr;

    ImGui::StyleColorsDark( );

    ImGui_ImplWin32_Init( hwnd );
    ImGui_ImplDX11_Init( device, context );

    return true;
}

bool overlay_t::begin_frame( )
{
    MSG msg{};
    while ( PeekMessageA( &msg, nullptr, 0, 0, PM_REMOVE ) )
    {
        TranslateMessage( &msg );
        DispatchMessageA( &msg );

        if ( msg.message == WM_QUIT )
            return false;
    }

    const bool insert_down = ( GetAsyncKeyState( VK_INSERT ) & 0x8000 ) != 0;
    if ( insert_down && !g_insert_was_down )
    {
        g_menu_open = !g_menu_open;

        LONG style = GetWindowLongA( hwnd, GWL_EXSTYLE );
        if ( g_menu_open )
        {
            style &= ~( WS_EX_TRANSPARENT | WS_EX_NOACTIVATE );
            SetWindowLongA( hwnd, GWL_EXSTYLE, style );
            SetForegroundWindow( hwnd );
            SetFocus( hwnd );
        }
        else
        {
            style |= WS_EX_TRANSPARENT | WS_EX_NOACTIVATE;
            SetWindowLongA( hwnd, GWL_EXSTYLE, style );
        }
    }
    g_insert_was_down = insert_down;

    ImGui_ImplDX11_NewFrame( );
    ImGui_ImplWin32_NewFrame( );
    ImGui::NewFrame( );

    visuals->render( );

    if ( g_menu_open )
    {
        ImGui::SetNextWindowSize( ImVec2( 300.f, 200.f ), ImGuiCond_FirstUseEver );
        ImGui::Begin( "menu", nullptr, ImGuiWindowFlags_NoCollapse );
        ImGui::Checkbox( "VSync", &vsync );
        ImGui::Checkbox( "boxes", &visuals->boxes );
        ImGui::Checkbox( "health bar", &visuals->health_bar );
        ImGui::End( );
    }

    ImGui::Render( );

    const float clear[ 4 ] = { 0.f, 0.f, 0.f, 0.f };
    context->ClearRenderTargetView( render_target, clear );
    context->OMSetRenderTargets( 1, &render_target, nullptr );

    D3D11_VIEWPORT vp{};
    vp.Width = static_cast< float >( width );
    vp.Height = static_cast< float >( height );
    vp.MaxDepth = 1.f;
    context->RSSetViewports( 1, &vp );

    ImGui_ImplDX11_RenderDrawData( ImGui::GetDrawData( ) );

    return true;
}

void overlay_t::end_frame( )
{
    swap_chain->Present( vsync ? 1 : 0, 0 );
}

void overlay_t::shutdown( )
{
    ImGui_ImplDX11_Shutdown( );
    ImGui_ImplWin32_Shutdown( );
    ImGui::DestroyContext( );

    release_render_target( );

    if ( comp_visual )
    {
        comp_visual->Release( );
        comp_visual = nullptr;
    }
    if ( comp_target )
    {
        comp_target->Release( );
        comp_target = nullptr;
    }
    if ( comp_device )
    {
        comp_device->Release( );
        comp_device = nullptr;
    }
    if ( swap_chain )
    {
        swap_chain->Release( );
        swap_chain = nullptr;
    }
    if ( context )
    {
        context->Release( );
        context = nullptr;
    }
    if ( device )
    {
        device->Release( );
        device = nullptr;
    }

    if ( hwnd )
    {
        DestroyWindow( hwnd );
        hwnd = nullptr;
    }

    UnregisterClassA( "overlay_cls", GetModuleHandleA( nullptr ) );
}

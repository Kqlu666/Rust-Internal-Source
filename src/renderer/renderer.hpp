#pragma once

#include "../vcruntime/include.hpp"
#include "../vcruntime/core/singleton.hpp"

#define IMGUI_DEFINE_MATH_OPERATORS

#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"

#include "../imgui/directx11/imgui_impl_win32.h"
#include "../imgui/directx11/imgui_impl_dx11.h"

#include "color.hpp"


namespace image {
    inline ID3D11ShaderResourceView* background_preview = nullptr;
    inline ID3D11ShaderResourceView* preview_model = nullptr;
    inline ID3D11ShaderResourceView* logo = nullptr;
}

namespace font {
    inline ImFont* icomoon = nullptr;
    inline ImFont* icomoon_tabs = nullptr;
    inline ImFont* icomoon_widget = nullptr;

    inline ImFont* inter_child = nullptr;
    inline ImFont* inter_element = nullptr;
}

namespace esp_preview {
    inline  bool money = true;
    inline bool nickname = true;
    inline bool weapon = true;
    inline bool zoom = true;

    inline bool c4 = true;
    inline bool HP_line = true;
    inline bool hit = true;
    inline bool box = true;
    inline bool bomb = true;

    static float box_color[ 4 ] = { 37 / 255.f, 37 / 255.f, 47 / 255.f, 1.f };
    static float nick_color[ 4 ] = { 255 / 255.f, 255 / 255.f, 255 / 255.f, 1.f };
    static float money_color[ 4 ] = { 255 / 255.f, 255 / 255.f, 255 / 255.f, 1.f };
    static float zoom_color[ 4 ] = { 255 / 255.f, 255 / 255.f, 255 / 255.f, 1.f };
    static float c4_color[ 4 ] = { 255 / 255.f, 255 / 255.f, 255 / 255.f, 1.f };
    static float bomb_color[ 4 ] = { 255 / 255.f, 255 / 255.f, 255 / 255.f, 1.f };
    static float hp_color[ 4 ] = { 255 / 255.f, 255 / 255.f, 255 / 255.f, 1.f };
    static float hp_line_color[ 4 ] = { 112 / 255.f, 109 / 255.f, 214 / 255.f, 1.f };
    static float weapon_color[ 4 ] = { 255 / 255.f, 255 / 255.f, 255 / 255.f, 1.f };
    static float hit_color[ 4 ] = { 255 / 255.f, 255 / 255.f, 255 / 255.f, 1.f };

    inline int hp = 85;
}

enum e_text_layout {
    text_left,
    text_right,
    text_center_v,
    text_center_h = 4,
    text_center = ( text_center_v | text_center_h ),
};

class c_renderer : public singleton< c_renderer > {
public:
    ~c_renderer( );

public:
    bool initialize( IDXGISwapChain* swap_chain );
    void destroy( );

    bool begin( );
    void end( );

    void lost( );
    bool reset( );

    bool intersect( const ImVec2& position );

    bool is_cn( );
    bool is_cn_chars( const char* text );

    IDXGISwapChain* get_swap_chain( ) const;
    ID3D11Device* get_device( ) const;
    ID3D11DeviceContext* get_device_context( ) const;

    const D3D11_VIEWPORT& get_viewport( ) const;
public:
    HWND get_window( ) { return HWND( _window ); }

    float get_framerate( ) { return _framerate; }

    ImVec2 get_size( ) { return ImVec2{ _viewport.Width, _viewport.Height }; }

    inline ImDrawList* get_draw( ) { return _draw_list; }

    inline void resize( ) {
        uint32_t viewport_count = 1;
        D3D11_VIEWPORT viewport = { };
        vcall( void, _device_context, RSGetViewports, &viewport_count, &viewport );

        if ( _viewport.Width == viewport.Width && _viewport.Height == viewport.Height )
            return;

        destory_render_target( );

        _viewport = viewport;
        _viewport_count = viewport_count;

        vcall( HRESULT, _swap_chain, ResizeBuffers, 0, _viewport.Width, _viewport.Height, DXGI_FORMAT_UNKNOWN, 0 );
        create_render_target( );
    }

public:
    inline ImDrawList* set_draw( ImDrawList* render ) {
        const auto draw = _draw_list;

        _draw_list = render;
        return draw;
    }

public:
    void draw_circle( const ImVec2& center_screen, float radius, const c_color& color, int segments = 256 );
    void draw_line( const ImVec2& Begin, const ImVec2& End, const c_color& color, float thickness = 1.f );
    void draw_filled_rect( const ImVec2& position, const ImVec2& size, const c_color& color, float rounding = 0.f );
    void draw_rect( const ImVec2& position, const ImVec2& size, const c_color& color, float rounding = 0.f );

    void draw_box( const ImVec2& position, const ImVec2& size, float thickness, const c_color& color, float rounding = 0.f );
    void draw_box_border( const ImVec2& position, const ImVec2& size, float thickness, const c_color& color,
                          const c_color& border = c_color::black( ), float rounding = 0.f );

    void draw_corner_box( const ImVec2& position, const ImVec2& size, const c_color& color );
    void draw_corner_box_border( const ImVec2& position, const ImVec2& size, const c_color& color,
                                 const c_color& border = c_color::black( ) );

    void draw_text( const ImVec2& position, uint32_t layout, const c_color& color, const char* const format, ... );
    void draw_text( const ImVec2& position, const c_color& color, const char* const format, ... );
    void draw_text(
        const ImVec2& position, float font_size, const c_color& color, const char* const format, ... );
    void draw_text(
        const ImVec2& position, float size, uint32_t layout, const c_color& color, const char* const format, ... );

    void draw_text_shadow(
        const ImVec2& position, uint32_t layout, const c_color& color, const char* const format, ... );
    void draw_text_shadow(
        const ImVec2& position, float font_size, const c_color& color, const char* const format, ... );
    void draw_text_shadow(
        const ImVec2& position, float size, uint32_t layout, const c_color& color, const char* const format, ... );

    void draw_text_outline(
        const ImVec2& position, std::uint32_t layout, const c_color& color, const char* const format, ... );
    void draw_text_outline(
        const ImVec2& position, float font_size, const c_color& color, const char* const format, ... );
    void draw_text_outline(
        const ImVec2& position, float size, std::uint32_t layout, const c_color& color, const char* const format, ... );

    void draw_text_border( const ImVec2& position, uint32_t layout, const c_color& color, const c_color& border,
                           const char* const format, ... );
    void draw_text_border( const ImVec2& position, float font_size, const c_color& color, const c_color& border,
                           const char* const format, ... );

    void draw_texture( ImTextureID tex_id, ImVec2 center, ImVec2 size, float angle, const c_color& color = c_color::white( ) );
    void draw_texture(
        ImTextureID texture_id, const ImVec2& position, const ImVec2& size, const c_color& color = c_color::white( ) );
    ID3D11ShaderResourceView* create_texture(
        uint8_t* image_rgba, int image_width, int image_height );
    ID3D11ShaderResourceView* create_srv( ID3D11Resource* resource );

    char* to_ansi( const wchar_t* unicode ) {
        static char output[ 512 ] = { };
        ImTextStrToUtf8(
            output, sizeof( output ), ( const ImWchar* )unicode, ( const ImWchar* )&unicode[ wcslen( unicode ) ] );
        return output;
    }

    wchar_t* to_unicode( const char* ansi ) {
        static wchar_t output[ 512 ] = { };
        ImTextStrFromUtf8( ( ImWchar* )output, sizeof( output ), ansi, &ansi[ strlen( ansi ) ] );
        return output;
    }

    uint16_t utf8_to_utf16( char* ansi ) {
        const auto unicode = to_unicode( ansi );
        return *( uint16_t* )( &unicode[ 0 ] );
    }

    bool create_render_target( );
    void destory_render_target( );

protected:
    void draw_format_text( ImVec2 position, uint32_t layout, const c_color& color, const c_color& border,
                           const char* const format, va_list arg_pack );
    void draw_format_text( ImVec2 position, const c_color& color, const c_color& border,
                           const char* const format, va_list arg_pack );
    void draw_format_text( ImVec2 position, float size, uint32_t layout, const c_color& color,
                           const c_color& border, const char* const format, va_list arg_pack );
    void draw_format_text( ImVec2 position, float size, const c_color& color, const c_color& border,
                           const char* const format, va_list arg_pack );

protected:
    IDXGISwapChain* _swap_chain = nullptr;
    ID3D11Device* _device = nullptr;
    ID3D11DeviceContext* _device_context = nullptr;

    void* _window = nullptr;

    ImGuiContext* _context = nullptr;
    ImDrawList* _draw_list = nullptr;

    ID3D11RenderTargetView* _render_target_view = nullptr;

    uint32_t _viewport_count = 1;
    D3D11_VIEWPORT _viewport = { };

    float _framerate = 0.f;
};

static auto& renderer = c_renderer::instance( );
#include "renderer.hpp"

#include "../vcruntime/memory/operation.hpp"
#include "../vcruntime/safe/safe.hpp"

#include "../game/sdk/unity.hpp"
#include "../util/logger.hpp"

#include "fonts.hpp"

template < typename T >
void safe_release( T*& object ) {
    if ( memory::is_valid( object ) ) {
        auto pointer = ( object );
        vcall( ULONG, pointer, Release );

        object = nullptr;
    }
}

c_renderer::~c_renderer( ) { destroy( ); }

bool c_renderer::initialize( IDXGISwapChain* swap_chain ) {
    _swap_chain = swap_chain;
    if ( !memory::is_valid( _swap_chain ) ) {
        TRACE( "swap_chain is not valid!" );
        return false;
    }

    auto result = vcall( HRESULT, _swap_chain, GetDevice,
                         IID_PPV_ARGS( &_device ) );
    if ( FAILED( result ) )
        return false;

    vcall( void, _device, GetImmediateContext, &_device_context );

    if ( !memory::is_valid( _device_context ) )
        return false;

    DXGI_SWAP_CHAIN_DESC swap_chain_desc = { };

    result = vcall(
        HRESULT, _swap_chain, GetDesc, &swap_chain_desc );
    if ( FAILED( result ) )
        return false;

    _window = swap_chain_desc.OutputWindow;
    if ( !_window )
        return false;

    _context = ImGui::CreateContext( );
    if ( !memory::is_valid( _context ) )
        return false;

    if ( !ImGui_ImplWin32_Init( _window ) )
        return false;

    if ( !ImGui_ImplDX11_Init( _device, _device_context ) )
        return false;

    auto& io = ImGui::GetIO( );

    ImGui::StyleColorsDark( );

    ImFontConfig font_cfg = { };
    font_cfg.FontDataOwnedByAtlas = false;

    font::inter_element = io.Fonts->AddFontFromMemoryTTF( inter_semibold, sizeof( inter_semibold ), 12.f, &font_cfg, io.Fonts->GetGlyphRangesCyrillic( ) );
    font::inter_child = io.Fonts->AddFontFromMemoryTTF( inter_semibold, sizeof( inter_semibold ), 14.f, &font_cfg, io.Fonts->GetGlyphRangesCyrillic( ) );

    font::icomoon = io.Fonts->AddFontFromMemoryTTF( icomoon, sizeof( icomoon ), 19.f, &font_cfg, io.Fonts->GetGlyphRangesCyrillic( ) );
    font::icomoon_tabs = io.Fonts->AddFontFromMemoryTTF( icomoon, sizeof( icomoon ), 22.f, &font_cfg, io.Fonts->GetGlyphRangesCyrillic( ) );
    font::icomoon_widget = io.Fonts->AddFontFromMemoryTTF( icomoon, sizeof( icomoon ), 16.f, &font_cfg, io.Fonts->GetGlyphRangesCyrillic( ) );

    //D3DX11_IMAGE_LOAD_INFO info; ID3DX11ThreadPump* pump{ nullptr };
    //if ( image::background_preview == nullptr ) D3DX11CreateShaderResourceViewFromMemory( g_pd3dDevice, background, sizeof( background ), &info, pump, &image::background_preview, 0 );
    //if ( image::preview_model == nullptr ) D3DX11CreateShaderResourceViewFromMemory( g_pd3dDevice, preview_model, sizeof( preview_model ), &info, pump, &image::preview_model, 0 );
    //if ( image::logo == nullptr ) D3DX11CreateShaderResourceViewFromMemory( g_pd3dDevice, logo, sizeof( logo ), &info, pump, &image::logo, 0 );


    //fonts[ 0 ] = io.Fonts->AddFontFromMemoryCompressedTTF( font_binary, sizeof( font_binary_size ), 14.f, &font_cfg, io.Fonts->GetGlyphRangesCyrillic( ) );
    //fonts[ 1 ] = io.Fonts->AddFontFromMemoryCompressedTTF( font_binary, sizeof( font_binary_size ), 18.f, &font_cfg, io.Fonts->GetGlyphRangesCyrillic( ) );
    //fonts[ 2 ] = io.Fonts->AddFontFromMemoryCompressedTTF( font_binary, sizeof( font_binary_size ), 20.f, &font_cfg, io.Fonts->GetGlyphRangesCyrillic( ) );
    //fonts[ 3 ] = io.Fonts->AddFontFromMemoryCompressedTTF( font_binary, sizeof( font_binary_size ), 50.f, &font_cfg, io.Fonts->GetGlyphRangesCyrillic( ) );

    //const auto& china_font_path = util::game_path( ) + _( "\\secret\\fonts\\china.ttf" );
    //fonts[ 4 ] = io.Fonts->AddFontFromFileTTF( china_font_path.c_str( ), 14.f, &font_cfg, io.Fonts->GetGlyphRangesChineseSimplifiedOfficial( ) );
    //fonts[ 5 ] = io.Fonts->AddFontFromFileTTF( china_font_path.c_str( ), 18.f, &font_cfg, io.Fonts->GetGlyphRangesChineseSimplifiedOfficial( ) );
    //fonts[ 6 ] = io.Fonts->AddFontFromFileTTF( china_font_path.c_str( ), 20.f, &font_cfg, io.Fonts->GetGlyphRangesChineseSimplifiedOfficial( ) );
    //fonts[ 7 ] = io.Fonts->AddFontFromFileTTF( china_font_path.c_str( ), 50.f, &font_cfg, io.Fonts->GetGlyphRangesChineseSimplifiedOfficial( ) );

    //icons[ 0 ] = io.Fonts->AddFontFromMemoryCompressedTTF( icons_binary, sizeof( icons_binary_size ), 12.f );
    //icons[ 1 ] = io.Fonts->AddFontFromMemoryCompressedTTF( icons_binary, sizeof( icons_binary_size ), 16.f );
    //icons[ 2 ] = io.Fonts->AddFontFromMemoryCompressedTTF( icons_binary, sizeof( icons_binary_size ), 20.f );
    //icons[ 3 ] = io.Fonts->AddFontFromMemoryCompressedTTF( icons_binary, sizeof( icons_binary_size ), 50.f );

    //io.Fonts->AddFontFromMemoryCompressedTTF( font_binary, sizeof( font_binary_size ), 14.f, &font_cfg, io.Fonts->GetGlyphRangesCyrillic( ) );
    //io.Fonts->AddFontFromMemoryCompressedTTF( icons_binary, sizeof( icons_binary_size ), 20.f );
    //io.Fonts->AddFontFromMemoryCompressedTTF( font_binary, sizeof( font_binary_size ), 18.f, &font_cfg, io.Fonts->GetGlyphRangesCyrillic( ) );
    //io.Fonts->AddFontFromMemoryCompressedTTF( font_binary, sizeof( font_binary_size ), 20.f, &font_cfg, io.Fonts->GetGlyphRangesCyrillic( ) );
    //io.Fonts->AddFontFromMemoryCompressedTTF( icons_binary, sizeof( icons_binary_size ), 50.f );
    //io.Fonts->AddFontFromMemoryCompressedTTF( icons_binary, sizeof( icons_binary_size ), 16.f );
    //io.Fonts->AddFontFromMemoryCompressedTTF( icons_binary, sizeof( icons_binary_size ), 12.f );
    //io.Fonts->AddFontFromMemoryCompressedTTF( font_binary, sizeof( font_binary_size ), 50.f, &font_cfg, io.Fonts->GetGlyphRangesCyrillic( ) );

    return create_render_target( );
}

bool c_renderer::is_cn( ) {
    ULONG num = 0;
    ULONG buff_size = 300;

    wchar_t buff[ 300 ];
    proc_call( GetUserPreferredUILanguages )( MUI_LANGUAGE_NAME, &num, buff, &buff_size );

    std::wstring str = buff;
    std::transform( str.begin( ), str.end( ), str.begin( ), [ ]( auto c ) { return constant::to_lower( c ); } );
    if ( str.find( _( L"cn" ) ) != std::wstring::npos || str.find( _( L"zh" ) ) != std::wstring::npos )
        return true;

    return false;
}

bool c_renderer::is_cn_chars( const char* text ) {
    while ( *text ) {

        if ( ( *text & 0xE0 ) == 0xE0 && ( *( text + 1 ) & 0x80 ) && ( *( text + 2 ) & 0x80 ) ) {
            unsigned int codepoint = ( ( *text & 0x0F ) << 12 ) | ( ( *( text + 1 ) & 0x3F ) << 6 ) | ( *( text + 2 ) & 0x3F );
            if ( codepoint >= 0x4E00 && codepoint <= 0x9FFF )
                return true;

            text += 3;
        } else {
            ++text;
        }
    }

    return false;
}

void c_renderer::destroy( ) {
    lost( );

    ImGui_ImplDX11_Shutdown( );
    ImGui_ImplWin32_Shutdown( );

    if ( memory::is_valid( _context ) )
        ImGui::DestroyContext( _context );

    _swap_chain = nullptr;
    _device = nullptr;
    _device_context = nullptr;

    _window = nullptr;
    _context = nullptr;
    _draw_list = nullptr;

    _render_target_view = nullptr;
}

bool c_renderer::begin( ) {
    if ( !memory::is_valid( _render_target_view ) )
        return false;

    if ( _viewport.Width == 0.f && _viewport.Height == 0.f )
        vcall( void, _device_context, RSGetViewports, &_viewport_count, &_viewport );

    renderer.resize( );

    ImGui_ImplDX11_NewFrame( );
    ImGui_ImplWin32_NewFrame( );

    ImGui::NewFrame( );

    _draw_list = ImGui::GetBackgroundDrawList( );

    auto& io = ImGui::GetIO( );
    io.DisplaySize = { _viewport.Width, _viewport.Height };

    _framerate = io.Framerate;

    return memory::is_valid( _draw_list );
}

void c_renderer::end( ) {
    ImGui::Render( );

    vcall( void, _device_context, OMSetRenderTargets, 1, &_render_target_view, nullptr );

    ImGui_ImplDX11_RenderDrawData( ImGui::GetDrawData( ) );
}

void c_renderer::lost( ) {
    ImGui_ImplDX11_InvalidateDeviceObjects( );
    destory_render_target( );
}

bool c_renderer::reset( ) {
    if ( !create_render_target( ) )
        return false;

    if ( !ImGui_ImplDX11_CreateDeviceObjects( ) )
        return false;

    return true;
}

bool c_renderer::intersect( const ImVec2& position ) {
    return ( position[ 0 ] >= 0.f && position[ 1 ] >= 0.f && position[ 0 ] <= _viewport.Width
             && position[ 1 ] <= _viewport.Height );
}

IDXGISwapChain* c_renderer::get_swap_chain( ) const { return _swap_chain; }

ID3D11Device* c_renderer::get_device( ) const { return _device; }

ID3D11DeviceContext* c_renderer::get_device_context( ) const { return _device_context; }

const D3D11_VIEWPORT& c_renderer::get_viewport( ) const { return _viewport; }

void c_renderer::draw_circle(
    const ImVec2& center_screen, float radius, const c_color& color, int segments /*= 512*/ ) {
    _draw_list->AddCircle( { center_screen[ 0 ], center_screen[ 1 ] }, radius,
                           ImColor( color.r, color.g, color.b, color.a ), segments );
}

void c_renderer::draw_line(
    const ImVec2& Begin, const ImVec2& End, const c_color& color, float thickness /*= 1.f*/ ) {
    _draw_list->AddLine( { Begin[ 0 ], Begin[ 1 ] }, { End[ 0 ], End[ 1 ] },
                         ImColor( color.r, color.g, color.b, color.a ), thickness );
}

void c_renderer::draw_filled_rect(
    const ImVec2& position, const ImVec2& size, const c_color& color, float rounding /*= 0.f*/ ) {
    _draw_list->AddRectFilled( { position[ 0 ], position[ 1 ] }, { position[ 0 ] + size[ 0 ], position[ 1 ] + size[ 1 ] },
                               ImColor( color.r, color.g, color.b, color.a ), rounding, 0 );
}

void c_renderer::draw_rect(
    const ImVec2& position, const ImVec2& size, const c_color& color, float rounding /*= 0.f*/ ) {
    _draw_list->AddRect( { position[ 0 ], position[ 1 ] }, { position[ 0 ] + size[ 0 ], position[ 1 ] + size[ 1 ] },
                         ImColor( color.r, color.g, color.b, color.a ), rounding, 0 );
}

void c_renderer::draw_box( const ImVec2& position, const ImVec2& size, float thickness, const c_color& color, float rounding ) {
    draw_rect( position, { size[ 0 ], thickness }, color, rounding );
    draw_rect( position, { thickness, size[ 1 ] }, color, rounding );

    draw_rect( { position[ 0 ] + size[ 0 ], position[ 1 ] }, { thickness, size[ 1 ] }, color, rounding );
    draw_rect( { position[ 0 ], position[ 1 ] + size[ 1 ] }, { size[ 0 ] + thickness, thickness }, color, rounding );
}

void c_renderer::draw_box_border( const ImVec2& position, const ImVec2& size, float thickness, const c_color& color,
                                  const c_color& border, float rounding ) {
    draw_box( position, size, thickness, color, rounding );
    draw_box(
        position - ImVec2( thickness, thickness ), size + ImVec2( thickness * 2.f, thickness * 2.f ), 1.f, border, rounding );
    draw_box(
        position + ImVec2( thickness, thickness ), size - ImVec2( thickness * 2.f, thickness * 2.f ), 1.f, border, rounding );
}

void c_renderer::draw_corner_box(
    const ImVec2& position, const ImVec2& size, const c_color& color ) {

    const auto& line_width = ( size.x / 5.f );
    const auto& line_height = ( size.y / 6.f );

    draw_line( { position.x, position.y }, { position.x, position.y + line_height }, color );
    draw_line( { position.x, position.y }, { position.x + line_width, position.y }, color );
    draw_line( { position.x + size.x - line_width, position.y }, { position.x + size.x, position.y }, color );
    draw_line( { position.x + size.x, position.y }, { position.x + size.x, position.y + line_height }, color );
    draw_line( { position.x, position.y + size.y - line_height }, { position.x, position.y + size.y }, color );
    draw_line( { position.x, position.y + size.y }, { position.x + line_width, position.y + size.y }, color );
    draw_line( { position.x + size.x - line_width, position.y + size.y }, { position.x + size.x, position.y + size.y }, color );
    draw_line( { position.x + size.x, position.y + size.y - line_height }, { position.x + size.x, position.y + size.y }, color );
}

void c_renderer::draw_corner_box_border( const ImVec2& position, const ImVec2& size,
                                         const c_color& color, const c_color& border /*= c_color::black()*/ ) {
    const auto& line_width = ( size.x / 5.f );
    const auto& line_height = ( size.y / 6.f );

    draw_line( { position.x - 1.f, position.y - 1.f }, { position.x + line_width, position.y - 1.f }, border );
    draw_line( { position.x - 1.f, position.y - 1.f }, { position.x - 1.f, position.y + line_height }, border );
    draw_line( { position.x - 1.f, position.y + size.y - line_height }, { position.x - 1.f, position.y + size.y + 1.f }, border );
    draw_line( { position.x - 1.f, position.y + size.y + 1.f }, { position.x + line_width, position.y + size.y + 1.f }, border );
    draw_line( { position.x + size.x - line_width, position.y - 1.f }, { position.x + size.x + 1.f, position.y - 1.f }, border );
    draw_line( { position.x + size.x + 1.f, position.y - 1.f }, { position.x + size.x + 1.f, position.y + line_height }, border );
    draw_line( { position.x + size.x + 1.f, position.y + size.y - line_height }, { position.x + size.x + 1.f, position.y + size.y + 1.f }, border );
    draw_line( { position.x + size.x - line_width, position.y + size.y + 1.f }, { position.x + size.x + 1.f, position.y + size.y + 1.f }, border );

    draw_corner_box( position, size, color );
}

void c_renderer::draw_text(
    const ImVec2& position, uint32_t layout, const c_color& color, const char* const format, ... ) {
    va_list arg_pack = { };
    va_start( arg_pack, format );
    draw_format_text( position, layout, color, c_color::transparent( ), format, arg_pack );
    va_end( arg_pack );
}

void c_renderer::draw_text(
    const ImVec2& position, float font_size, const c_color& color, const char* const format, ... ) {
    va_list arg_pack = { };
    va_start( arg_pack, format );
    draw_format_text( position, font_size, color, c_color::transparent( ), format, arg_pack );
    va_end( arg_pack );
}

void c_renderer::draw_text(
    const ImVec2& position, const c_color& color, const char* const format, ... ) {
    va_list arg_pack = { };
    va_start( arg_pack, format );
    draw_format_text( position, color, c_color::transparent( ), format, arg_pack );
    va_end( arg_pack );
}

void c_renderer::draw_text(
    const ImVec2& position, float size, uint32_t layout, const c_color& color, const char* const format, ... ) {
    va_list arg_pack = { };
    va_start( arg_pack, format );
    draw_format_text( position, size, layout, color, c_color::transparent( ), format, arg_pack );
    va_end( arg_pack );
}

void c_renderer::draw_text_shadow(
    const ImVec2& position, uint32_t layout, const c_color& color, const char* const format, ... ) {
    const auto shadow = c_color::black( color.a );

    va_list arg_pack = { };
    va_start( arg_pack, format );
    draw_format_text( position + ImVec2( 1.f, 1.f ), layout, shadow, c_color::transparent( ), format, arg_pack );
    draw_format_text( position, layout, color, c_color::transparent( ), format, arg_pack );
    va_end( arg_pack );
}

void c_renderer::draw_text_shadow(
    const ImVec2& position, float font_size, const c_color& color, const char* const format, ... ) {
    const auto shadow = c_color::black( color.a );

    va_list arg_pack = { };
    va_start( arg_pack, format );
    draw_format_text( position + ImVec2( 1.f, 1.f ), font_size, shadow, c_color::transparent( ), format, arg_pack );
    draw_format_text( position, font_size, color, c_color::transparent( ), format, arg_pack );
    va_end( arg_pack );
}

void c_renderer::draw_text_shadow(
    const ImVec2& position, float size, uint32_t layout, const c_color& color, const char* const format, ... ) {
    const auto shadow = c_color::black( color.a );

    va_list arg_pack = { };
    va_start( arg_pack, format );
    draw_format_text( position + ImVec2( 1.f, 1.f ), size, layout, shadow, c_color::transparent( ), format, arg_pack );
    draw_format_text( position, size, layout, color, c_color::transparent( ), format, arg_pack );
    va_end( arg_pack );
}

void c_renderer::draw_text_outline(
    const ImVec2& position, uint32_t layout, const c_color& color, const char* const format, ... ) {

    const auto shadow = c_color::black( color.a );
    va_list arg_pack = { };
    va_start( arg_pack, format );

    draw_format_text(
        position + ImVec2( 0.f, 1.f ), layout, shadow, c_color::transparent( ), format, arg_pack ); // shadow
    draw_format_text(
        position - ImVec2( 0.f, 1.f ), layout, shadow, c_color::transparent( ), format, arg_pack ); // shadow
    draw_format_text(
        position + ImVec2( 1.f, 0.f ), layout, shadow, c_color::transparent( ), format, arg_pack ); // shadow
    draw_format_text(
        position - ImVec2( 1.f, 0.f ), layout, shadow, c_color::transparent( ), format, arg_pack ); // shadow

    draw_format_text( position, layout, color, c_color::transparent( ), format, arg_pack );
    va_end( arg_pack );
}

void c_renderer::draw_text_outline(
    const ImVec2& position, float font_size, const c_color& color, const char* const format, ... ) {

    const auto shadow = c_color::black( color.a );
    va_list arg_pack = { };
    va_start( arg_pack, format );

    draw_format_text(
        position + ImVec2( 0.f, 1.f ), font_size, shadow, c_color::transparent( ), format, arg_pack ); // shadow
    draw_format_text(
        position - ImVec2( 0.f, 1.f ), font_size, shadow, c_color::transparent( ), format, arg_pack ); // shadow
    draw_format_text(
        position + ImVec2( 1.f, 0.f ), font_size, shadow, c_color::transparent( ), format, arg_pack ); // shadow
    draw_format_text(
        position - ImVec2( 1.f, 0.f ), font_size, shadow, c_color::transparent( ), format, arg_pack ); // shadow

    draw_format_text( position, font_size, color, c_color::transparent( ), format, arg_pack );
    va_end( arg_pack );
}

void c_renderer::draw_text_outline(
    const ImVec2& position, float size, uint32_t layout, const c_color& color, const char* const format, ... ) {
    const auto shadow = c_color::black( color.a );

    va_list arg_pack = { };
    va_start( arg_pack, format );
    draw_format_text( position + ImVec2( 1.f, 1.f ), size, layout, shadow, c_color::transparent( ), format, arg_pack );
    draw_format_text( position, size, layout, color, c_color::transparent( ), format, arg_pack );

    draw_format_text(
        position + ImVec2( 0.f, 1.f ), size, layout, shadow, c_color::transparent( ), format, arg_pack ); // shadow
    draw_format_text(
        position - ImVec2( 0.f, 1.f ), size, layout, shadow, c_color::transparent( ), format, arg_pack ); // shadow
    draw_format_text(
        position + ImVec2( 1.f, 0.f ), size, layout, shadow, c_color::transparent( ), format, arg_pack ); // shadow
    draw_format_text(
        position - ImVec2( 1.f, 0.f ), size, layout, shadow, c_color::transparent( ), format, arg_pack ); // shadow

    draw_format_text( position, size, layout, color, c_color::transparent( ), format, arg_pack );
    va_end( arg_pack );
}

void c_renderer::draw_text_border( const ImVec2& position, uint32_t layout, const c_color& color,
                                   const c_color& border, const char* const format, ... ) {
    va_list arg_pack = { };
    va_start( arg_pack, format );
    draw_format_text( position, layout, color, border, format, arg_pack );
    va_end( arg_pack );
}

void c_renderer::draw_text_border( const ImVec2& position, float font_size, const c_color& color,
                                   const c_color& border, const char* const format, ... ) {
    va_list arg_pack = { };
    va_start( arg_pack, format );
    draw_format_text( position, font_size, color, border, format, arg_pack );
    va_end( arg_pack );
}

void c_renderer::draw_texture( ImTextureID tex_id, ImVec2 center, ImVec2 size, float angle, const c_color& color ) {
    float cos_a = cosf( angle );
    float sin_a = sinf( angle );
    ImVec2 pos[ 4 ] =
    {
        center + ImRotate( ImVec2( -size.x * 0.5f, -size.y * 0.5f ), cos_a, sin_a ),
        center + ImRotate( ImVec2( +size.x * 0.5f, -size.y * 0.5f ), cos_a, sin_a ),
        center + ImRotate( ImVec2( +size.x * 0.5f, +size.y * 0.5f ), cos_a, sin_a ),
        center + ImRotate( ImVec2( -size.x * 0.5f, +size.y * 0.5f ), cos_a, sin_a )
    };

    ImVec2 uvs[ 4 ] =
    {
        ImVec2( 0.0f, 0.0f ),
        ImVec2( 1.0f, 0.0f ),
        ImVec2( 1.0f, 1.0f ),
        ImVec2( 0.0f, 1.0f )
    };

    _draw_list->AddImageQuad( tex_id, pos[ 0 ], pos[ 1 ], pos[ 2 ], pos[ 3 ], uvs[ 0 ], uvs[ 1 ], uvs[ 2 ], uvs[ 3 ], ImColor( color.r, color.g, color.b, color.a ) );
}

void c_renderer::draw_texture( ImTextureID texture_id, const ImVec2& position, const ImVec2& size,
                               const c_color& color /*= c_color::white()*/ ) {
    _draw_list->AddImage( texture_id, { position[ 0 ], position[ 1 ] }, { position[ 0 ] + size[ 0 ], position[ 1 ] + size[ 1 ] },
                          { 0.f, 1.f }, { 1.f, 0.f }, ImColor( color.r, color.g, color.b, color.a ) );
}

ID3D11ShaderResourceView* c_renderer::create_texture(
    uint8_t* image_rgba, int image_width, int image_height ) {

    D3D11_TEXTURE2D_DESC desc = { };

    desc.Width = image_width;
    desc.Height = image_height;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags = 0;

    ID3D11Texture2D* texture2d = nullptr;
    D3D11_SUBRESOURCE_DATA subResource = { };

    subResource.pSysMem = image_rgba;
    subResource.SysMemPitch = desc.Width * 4;
    subResource.SysMemSlicePitch = 0;
    vcall( HRESULT, _device, CreateTexture2D, &desc, &subResource, &texture2d );

    D3D11_SHADER_RESOURCE_VIEW_DESC shader_desc = { };

    shader_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    shader_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    shader_desc.Texture2D.MipLevels = desc.MipLevels;
    shader_desc.Texture2D.MostDetailedMip = 0;

    ID3D11ShaderResourceView* output_shader = nullptr;
    vcall( HRESULT, _device, CreateShaderResourceView, texture2d, &shader_desc, &output_shader );
    vcall( ULONG, texture2d, Release );

    return output_shader;
}

ID3D11ShaderResourceView* c_renderer::create_srv( ID3D11Resource* resource ) {

    D3D11_SHADER_RESOURCE_VIEW_DESC shader_desc = { };

    shader_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    shader_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    shader_desc.Texture2D.MipLevels = -1;
    shader_desc.Texture2D.MostDetailedMip = 0;

    ID3D11ShaderResourceView* output_shader = nullptr;
    vcall( HRESULT, _device, CreateShaderResourceView, resource, &shader_desc, &output_shader );

    return output_shader;
}

void c_renderer::draw_format_text( ImVec2 position, uint32_t layout, const c_color& color, const c_color& border,
                                   const char* const format, va_list arg_pack ) {
    const auto output_length = static_cast< size_t >( vsprintf_s( nullptr, 0, format, arg_pack ) ) + 1;

    if ( !output_length )
        return;

    char output[ 512 ] = { };
    vsprintf_s( output, output_length, format, arg_pack );

    const auto& size = ImGui::CalcTextSize( output );

    if ( layout & text_right ) {
        position[ 0 ] -= size[ 0 ];
    } else if ( layout & text_center_h ) {
        position[ 0 ] -= size[ 0 ] * 0.5f;
    }

    if ( layout & text_center_v )
        position[ 1 ] -= size[ 1 ] * 0.5f;

    if ( border.a > 0.f )
        draw_rect( position, { size[ 0 ], size[ 1 ] }, border );

    _draw_list->AddText(
        { position[ 0 ], position[ 1 ] }, ImColor( color.r, color.g, color.b, color.a ), output );
}

void c_renderer::draw_format_text( ImVec2 position, const c_color& color, const c_color& border,
                                   const char* const format, va_list arg_pack ) {
    const auto output_length = static_cast< size_t >( vsprintf_s( nullptr, 0, format, arg_pack ) ) + 1;

    if ( !output_length )
        return;

    char output[ 512 ] = { };
    vsprintf_s( output, output_length, format, arg_pack );

    const auto& size = ImGui::CalcTextSize( output );

    if ( border.a > 0.f )
        draw_rect( position, { size[ 0 ], size[ 1 ] }, border );

    _draw_list->AddText(
        { position[ 0 ], position[ 1 ] }, ImColor( color.r, color.g, color.b, color.a ), output );
}

void c_renderer::draw_format_text( ImVec2 position, float size, const c_color& color, const c_color& border,
                                   const char* const format, va_list arg_pack ) {
    const auto output_length = static_cast< size_t >( vsprintf_s( nullptr, 0, format, arg_pack ) ) + 1;

    if ( !output_length )
        return;

    char output[ 512 ] = { };
    vsprintf_s( output, output_length, format, arg_pack );

    const auto& text_size = ImGui::CalcTextSize( output );

    if ( border.a > 0.f )
        draw_rect( position, { text_size[ 0 ], text_size[ 1 ] }, border );

    _draw_list->AddText( nullptr, size, { position[ 0 ], position[ 1 ] },
                         ImColor( color.r, color.g, color.b, color.a ), output );
}

void c_renderer::draw_format_text( ImVec2 position, float size, uint32_t layout, const c_color& color,
                                   const c_color& border, const char* const format, va_list arg_pack ) {
    const auto output_length = static_cast< size_t >( vsprintf_s( nullptr, 0, format, arg_pack ) ) + 1;

    if ( !output_length )
        return;

    char output[ 512 ] = { };
    vsprintf_s( output, output_length, format, arg_pack );

    const auto& text_size = ImGui::CalcTextSize( output );

    if ( layout & text_right ) {
        position[ 0 ] -= text_size[ 0 ];
    } else if ( layout & text_center_h ) {
        position[ 0 ] -= text_size[ 0 ] * 0.5f;
    }

    if ( layout & text_center_v )
        position[ 1 ] -= text_size[ 1 ] * 0.5f;

    if ( border.a > 0.f )
        draw_rect( position, { text_size[ 0 ], text_size[ 1 ] }, border );

    _draw_list->AddText( nullptr, size, { position[ 0 ], position[ 1 ] },
                         ImColor( color.r, color.g, color.b, color.a ), output );
}

bool c_renderer::create_render_target( ) {
    ID3D11Texture2D* texture2d = nullptr;

    auto result = vcall( HRESULT, _swap_chain, GetBuffer, 0,
                         IID_PPV_ARGS( &texture2d ) );

    if ( FAILED( result ) )
        return false;

    result = vcall( HRESULT, _device, CreateRenderTargetView, texture2d, nullptr,
                    &_render_target_view );

    safe_release( texture2d );

    if ( FAILED( result ) )
        return false;

    return true;
}

void c_renderer::destory_render_target( ) { safe_release( _render_target_view ); }
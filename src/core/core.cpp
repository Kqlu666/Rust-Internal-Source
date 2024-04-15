#include "core.hpp"
#include <evntprov.h>
#include <d3d11.h>
#include <dxgi.h>

#include "../vcruntime/pe/image.hpp"
#include "../vcruntime/safe/safe.hpp"
#include "../util/logger.hpp"
#include "../util/util.hpp"

#include "../game/sdk/unity.hpp"
#include "../renderer/renderer.hpp"

#include "../game/sdk/sdk.hpp"

#include "../options/config_mgr.hpp"
#include "../renderer/input_handler.hpp"

#include "../game/features/features.hpp"
#include "../game/hooks.hpp"

#include "../ui/ui.hpp"

namespace core {
    uintptr_t original_address = 0;
    uintptr_t import_address = 0;
    EVENT_DESCRIPTOR* event_present_start = 0;
    uintptr_t dxgi_enable_bits = 0;
    IDXGISwapChain* swap_chain = nullptr;

    bool destroy( ) {
        auto& renderer = c_renderer::instance( );

        memory::write< uint8_t >( dxgi_enable_bits, 0x00 );

        DWORD protect = 0;

        if ( VirtualProtect( memory::to_ptr( import_address ), 8, PAGE_READWRITE, &protect ) ) {
            memory::write< uint64_t >( import_address, original_address );

            VirtualProtect( memory::to_ptr( import_address ), 8, protect, &protect );
        }

        g_hooks.destroy_all( );

        if ( memory::is_valid( swap_chain ) )
            renderer.destroy( );

        return true;
    }

    IDXGISwapChain* get_swap_chain( ) {
        const auto& unity_player = pe::get_image( HASH( "unityplayer" ) );
        const auto& get_swapchain_d3d11 = reinterpret_cast< IDXGISwapChain * ( * )( ) >( unity_player + 0x76e1e0 );
        return get_swapchain_d3d11( );
    }

    bool initialize_interfaces( ) {
        renderer.initialize( swap_chain );
        il2cpp::initialize( );
        hooks::initialize( );
        sdk::entity_types_t::initialize( );

        return true;
    }

    HRESULT hk_present( IDXGISwapChain* swap_chain, UINT sync_interval, UINT flags ) {
        static bool initialized = false;
        if ( !initialized ) {
            initialize_interfaces( );
            initialized = true;
        }

        renderer.begin( );

        ui::draw( );
        features::loop( );

        renderer.end( );

        return 0;
    }

    uint32_t hk_EtwEventWriteTransfer(
        REGHANDLE, EVENT_DESCRIPTOR* event, GUID*, GUID*, uint32_t, EVENT_DATA_DESCRIPTOR* ) {

        if ( event != event_present_start )
            return 0;

        if ( memory::is_valid( swap_chain ) )
            hk_present( swap_chain, 0, 0 );
        else
            swap_chain = get_swap_chain( );

        return 0;
    }

    bool initialize_present_hk( ) {
        auto get_trampoline = [ ]( void* procedure ) -> uintptr_t {
            const auto ntdll = pe::get_image( HASH( "ntdll" ) );
            auto jump = pe::get_procedure< memory::address_t< uintptr_t > >( ntdll, HASH( "NtdllDialogWndProc_W" ) )
                + 0x30; // NtdllListBoxWndProc_A

            if ( !memory::is_valid( jump ) )
                return 0;

            import_address = jump.rel( 2 ).as< uintptr_t >( );
            if ( !memory::is_valid( import_address ) )
                return 0;

            DWORD protect = 0;

            VirtualProtect( memory::to_ptr( import_address ), 8, PAGE_READWRITE, &protect );
            memory::write( import_address, procedure );
            VirtualProtect( memory::to_ptr( import_address ), 8, protect, &protect );

            return jump.as< uintptr_t >( ) - 0x8;
        };

        const auto image = pe::get_image( HASH( "dxgi" ) );

        if ( !memory::is_valid( image ) ) {
            TRACE( "memory::GetImage( ... ) !", );
            return false;
        }

        TRACE( "dxgi.dll @ 0x%llX", image );
        auto etw_event_write_transfer = pe::get_import( HASH( "dxgi" ), HASH( "EtwEventWriteTransfer" ) );
        if ( !etw_event_write_transfer.procedure ) {
            TRACE( "memory::GetImageImport( ... ) !", );
            return false;
        }

        TRACE( "dxgi.dll->EtwEventWriteTransfer @ 0x%llX", etw_event_write_transfer.procedure );

        auto dxgi_enable_bits_ptr = memory::scan_image( HASH( "dxgi" ), _( "F6 05 ? ? ? ? 02 74 22 41" ) );
        const auto& dxgi_enable_bits = ( dxgi_enable_bits_ptr.rel( 2 ) + 1 ).as( );

        if ( !memory::is_valid( dxgi_enable_bits ) )
            return false;

        TRACE( "m_dxgi_enable_bits @ 0x%llX", dxgi_enable_bits );
        if ( !memory::is_valid( dxgi_enable_bits ) )
            return false;

        memory::write( dxgi_enable_bits, 0x02 );
        TRACE( "dxgi_enable_bits patched to 0x02!" );

        event_present_start
            = memory::scan_image( HASH( "dxgi" ), _( "48 8D 15 ? ? ? ? 89 7C" ) ).rel( 3 ).as< EVENT_DESCRIPTOR* >( );
        TRACE( "m_event_present_start @ 0x%llX", event_present_start );

        if ( !memory::is_valid( event_present_start ) )
            return false;

        DWORD protect = 0;
        VirtualProtect( memory::to_ptr( etw_event_write_transfer.procedure ), 8, PAGE_READWRITE, &protect );
        original_address = memory::read( etw_event_write_transfer.procedure );

        const auto address = get_trampoline( &hk_EtwEventWriteTransfer );
        memory::write( etw_event_write_transfer.procedure, address );

        VirtualProtect( memory::to_ptr( etw_event_write_transfer.procedure ), 8, protect, &protect );
        return true;
    }

    bool initialize( ) {
        config::manager::init( );

        if ( !initialize_present_hk( ) )
            return false;

        return true;
    }
} // namespace core

#include "vcruntime/include.hpp"
#include "core/core.hpp"
#include "util/logger.hpp"
#include "vcruntime/safe/safe.hpp"
#include "vcruntime/safe/secure.hpp"
#include "vcruntime/memory/operation.hpp"

extern "C" {
#pragma const_seg( ".CRT$XCA" )
    const _PVFV g_xc_a[] = { 0 };

#pragma const_seg( ".CRT$XCZ" )
    const _PVFV g_xc_z[] = { 0 };
};

void initialize_global_vars( const _PVFV* begin, const _PVFV* end ) {
    while ( begin < end )
        if ( auto fn = *++begin )
            fn( );
}

bool on_proc_attach( uintptr_t image ) {

    if ( !memory::set_image_info( image ) || !safe.initialize( ) )
        return false;

#ifdef DEBUG
    logger::initialize( );
#endif

    initialize_global_vars( g_xc_a, g_xc_z );

    if ( !core::initialize( ) )
        return false;

    return true;
}

bool entry( uintptr_t base, uint32_t reason, uintptr_t ) {
    if ( reason == DLL_PROCESS_ATTACH )
        return on_proc_attach( base );
    //if ( reason == DLL_PROCESS_DETACH )
    //    return core::destroy( );

    return true;
}
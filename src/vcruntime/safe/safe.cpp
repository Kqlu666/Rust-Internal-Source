#include "safe.hpp"
#include "../../util/logger.hpp"

const uint32_t c_safe::get_system_index( uintptr_t image, hash_t procedure ) {

    const auto proc = pe::get_procedure( image, procedure );

    if ( !proc )
        return m_bad_index;

    auto system_number = *reinterpret_cast< std::uint32_t* >( proc + 4 );

    if ( system_number != m_bad_index )
        return system_number;

    TRACE( "System number for '%#llx' not found!", procedure );
    PAUSE( );

    return m_bad_index;
}

const uint32_t c_safe::get_system_index( hash_t procedure ) {

    auto proc_addr = pe::get_procedure< uintptr_t >( HASH( "ntdll" ), procedure );
    if ( !proc_addr )
        proc_addr = pe::get_procedure< uintptr_t >( HASH( "win32u" ), procedure );

    if ( !proc_addr )
        return 0;

    bool is_hooked = *reinterpret_cast< uint32_t* >( proc_addr ) != 0xb8d18b4c
        && *reinterpret_cast< uint32_t* >( proc_addr + 0x10 ) != 0x050f0375;
    if ( is_hooked )
        return m_bad_index;

    auto idx = *reinterpret_cast< uint32_t* >( proc_addr + 0x4 );
    if ( !idx )
        return m_bad_index;

    return idx;
}

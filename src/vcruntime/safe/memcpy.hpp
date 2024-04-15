#pragma once
#include "../include.hpp"

namespace mem {

    uintptr_t get_jmp_addr( );
    bool initialize( );
    void copy( void* buffer, void* address, size_t size );

    template < typename T = uintptr_t, typename A >
    T read( A address ) {
        constexpr auto size = sizeof( T );

        uint8_t buffer[ size ] = { };
        copy( buffer, ( void* )( address ), size );

        return *( T* )( &buffer[ 0 ] );
    }

    template < class T = uintptr_t, typename A >
    void write( A address, T value ) {
        return copy( ( void* )( address ), ( void* )( &value ), sizeof( T ) );
    }

    template<class result_t, class T>
    inline result_t read_chain( T base, const std::vector< uint32_t > offsets ) {
        uintptr_t addr = reinterpret_cast< uintptr_t >( base );

        for ( const auto& offset : offsets ) {
            addr = read<uintptr_t>( addr + offset );
            if ( !addr )
                return { };
        }

        return result_t( addr );
    };
} // namespace mem

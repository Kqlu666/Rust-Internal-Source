#pragma once
#include "../safe/memcpy.hpp"

namespace memory {

    constexpr auto min_user_addr = 0x0000000000010000;
    constexpr auto max_user_addr = 0x00007FFFFFFEFFFF;

    inline uintptr_t image_base = 0;
    inline size_t image_size = 0;

    template < class T, typename _ty2 >
    __forceinline T get( _ty2 addr ) {
        return T( addr );
    }

    __forceinline uintptr_t to_addr( const void* pointer ) { return reinterpret_cast< uintptr_t >( pointer ); }

    __forceinline void* to_ptr( uintptr_t address ) { return reinterpret_cast< void* >( address ); }

    __forceinline bool is_valid( uintptr_t address ) {
        if ( address > image_base && address < ( image_base + image_size ) )
            return true;

        return ( address >= min_user_addr && address < max_user_addr );
    }

    __forceinline bool is_valid( const void* pointer ) { return is_valid( to_addr( pointer ) ); }

    __forceinline bool set_image_info( uintptr_t base ) {
        image_size = 0x14ACCC;
        image_base = base;
        return true;
    }

    template < class T = uintptr_t, typename addr_t >
    T read( addr_t address ) {
        return mem::read< T >( address );
    }

    template < class T, typename addr_t >
    void write( addr_t address, T value ) {
        return mem::write< T >( address, value );
    }

    __forceinline void copy( void* buffer, void* address, std::size_t size ) {
        return mem::copy( buffer, address, size );
    }

    template < class T = std::uintptr_t, typename A >
    T ReadProcessMemory( A address ) {
        constexpr auto size = sizeof( T );

        std::uint8_t buffer[ size ] = { };
        ::ReadProcessMemory( GetCurrentProcess( ), ( const void* )( address ), buffer, size, nullptr );
        return *( T* )( &buffer[ 0 ] );
    }

} // namespace memory

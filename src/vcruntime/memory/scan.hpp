#pragma once
#include "../pe/image.hpp"

namespace memory {
    namespace impl {
        uintptr_t _scan_region(
            const uint8_t* region_begin, const uint8_t* region_end, const char* signature, bool to_end );
        uintptr_t _scan_section( uintptr_t image, hash_t section_name, const char* signature, bool to_end );
        uintptr_t _scan_image( uintptr_t image, const char* signature, bool to_end );
    } // namespace impl

    template < class T >
    struct address_t {
        T* m_ptr;

        __forceinline address_t< T >( ) { m_ptr = ( T* )0; }

        __forceinline address_t< T >( T* ptr ) { m_ptr = ptr; }

        __forceinline address_t< T >( uintptr_t va ) { m_ptr = ( T* )( va ); }

        template < typename _cast_type = uintptr_t >
        __forceinline _cast_type as( ) {
            return ( _cast_type )m_ptr;
        }

        template < typename _cast_type >
        __forceinline address_t< _cast_type > to( ) {
            return address_t< _cast_type >( reinterpret_cast< _cast_type* >( m_ptr ) );
        }

        __forceinline address_t< T > add( uint64_t offset ) { return address_t< T >( ( T* )( ( uintptr_t )m_ptr + offset ) ); }

        __forceinline address_t< T > rel( uint64_t offset = 0 ) {
            if ( !m_ptr )
                return address_t< T >( nullptr );

            return address_t< T >( ( *( uint8_t* )( ( uint64_t )m_ptr + offset + 3 ) == 0xFF )
                                   ? ( T* )( ( uint64_t )m_ptr + offset + 4 + *( uint32_t* )( ( uint64_t )m_ptr + offset ) + 0xFFFFFFFF00000000 )
                                   : ( T* )( ( uint64_t )m_ptr + offset + 4 + *( uint32_t* )( ( uint64_t )m_ptr + offset ) ) );
        }

        __forceinline operator T* ( ) const { return m_ptr; }

        __forceinline T* operator->( ) { return m_ptr; }

        __forceinline T& operator[]( uint64_t index ) { return m_ptr[ index ]; }

        __forceinline bool operator==( T* val ) { return m_ptr == val; }

        __forceinline bool operator==( uintptr_t val ) { return reinterpret_cast< uintptr_t >( m_ptr ) == val; }

        __forceinline bool operator!( ) { return !m_ptr; }

        __forceinline auto operator+( uint32_t val ) { return add( val ); }
    };

    template < class T = uintptr_t >
    auto scan_region(
        const uint8_t* region_begin, const uint8_t* region_end, const char* signature, bool to_end = false ) {
        return address_t< T >( impl::_scan_region( region_begin, region_end, signature, to_end ) );
    }

    template < class T = uintptr_t >
    auto scan_image_section(
        const hash_t image_hash, const hash_t section_hash, const char* signature, bool to_end = false ) {
        return address_t< T >(
            impl::_scan_section( pe::get_image< uintptr_t >( image_hash ), section_hash, signature, to_end ) );
    }

    template < class T = uintptr_t >
    auto scan_image_section(
        const uint8_t* image_base, const hash_t section_hash, const char* signature, bool to_end = false ) {
        return address_t< T >(
            impl::_scan_section( reinterpret_cast< uintptr_t >( image_base ), section_hash, signature, to_end ) );
    }

    template < class T = uintptr_t >
    auto scan_image( const hash_t image_hash, const char* signature, bool to_end = false ) {
        return address_t< T >( impl::_scan_image( pe::get_image< uintptr_t >( image_hash ), signature, to_end ) );
    }

} // namespace memory

#pragma once
#include "../constant/hash.hpp"
#include "../nt/nt.hpp"

struct import_t {
    char image_name[ 512 ] = { };
    char procedure_name[ 512 ] = { };
    uintptr_t procedure = 0;
};

namespace pe {
    extern const char* curr_module;
    namespace impl {
        uintptr_t _get_procedure( const hash_t proc_hash, uint8_t* excluded_module = nullptr, bool rva = false );
        uintptr_t _get_procedure( uintptr_t base, const hash_t proc_hash, bool rva = false );
        uintptr_t _get_image( const hash_t module_hash, bool unlink = false );
    } // namespace impl

    const PIMAGE_DOS_HEADER get_dos_header( uintptr_t image );
    const PIMAGE_NT_HEADERS get_nt_headers( uintptr_t image );
    const uint32_t get_image_size( uintptr_t image );

    template < class T = uint8_t* >
    T get_image( const hash_t module_hash ) {
        return ( T )impl::_get_image( module_hash );
    }

    template < class T = uint8_t* >
    T get_procedure( uint8_t* base, const hash_t proc_hash, bool rva = false ) {
        return ( T )impl::_get_procedure( ( uintptr_t )base, proc_hash, rva );
    }

    template < class T = uint8_t* >
    T get_procedure( const hash_t image_hash, const hash_t proc_hash, bool rva = false ) {
        return ( T )impl::_get_procedure( get_image< uintptr_t >( image_hash ), proc_hash, rva );
    }

    template < class T = uint8_t* >
    T get_procedure( const hash_t proc_hash, bool rva = false ) {
        return ( T )impl::_get_procedure( proc_hash );
    }

} // namespace pe

class c_image {
    uintptr_t m_image = 0;
    PIMAGE_DOS_HEADER m_dos_header = nullptr;
    PIMAGE_NT_HEADERS m_nt_headers = nullptr;
    PIMAGE_SECTION_HEADER m_section_header = nullptr;

    bool m_state = false;

public:
    c_image( hash_t image_hash ) :
        m_image( pe::get_image< uintptr_t >( image_hash ) ), m_dos_header( get_dos_header_impl( ) ),
        m_nt_headers( get_nt_headers_impl( ) ), m_section_header( get_section_header_impl( ) ) { }

    template < class T = uintptr_t >
    T get( ) {
        return T( m_image );
    }

    auto dos_header( ) { return m_dos_header; }
    auto nt_headers( ) { return m_nt_headers; }
    auto section_header( ) { return m_section_header; }

    auto section( hash_t name_hash ) { return get_section_impl( name_hash ); }
    auto section( const char* name ) { return get_section_impl( RHASH( name ) ); }

    auto size( ) { return static_cast< size_t >( m_nt_headers->OptionalHeader.SizeOfImage ); }

    auto data_dir( uint16_t directory ) { return get_data_dir_impl( directory ); }

    template < class T = uintptr_t >
    T get_procedure( hash_t name_hash ) {
        return T( get_export_impl( name_hash ) );
    }
    template < class T = uintptr_t >
    T get_procedure( const char* name ) {
        return T( get_export_impl( RHASH( name ) ) );
    }

    auto get_import( hash_t name_hash ) { return get_import_impl( name_hash ); }
    auto get_import( const char* name ) { return get_import_impl( RHASH( name ) ); }

private:
    const PIMAGE_DOS_HEADER get_dos_header_impl( );
    const PIMAGE_NT_HEADERS get_nt_headers_impl( );
    const PIMAGE_SECTION_HEADER get_section_header_impl( );
    const PIMAGE_SECTION_HEADER get_section_impl( hash_t name_hash );
    const uintptr_t get_export_impl( hash_t name_hash );
    const import_t get_import_impl( hash_t import_name_hash );
    const PIMAGE_DATA_DIRECTORY get_data_dir_impl( uint16_t directory );
};

namespace pe {
    __forceinline auto get_import( const hash_t image, const hash_t name_hash ) {
        c_image pe( image );
        return pe.get_import( name_hash );
    }
} // namespace pe

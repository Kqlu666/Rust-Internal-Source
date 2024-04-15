#include "scan.hpp"
#include "operation.hpp"

#define IN_RANGE( x, a, b ) ( x >= a && x <= b )
#define GET_BITS( x )       ( IN_RANGE( x, '0', '9' ) ? ( x - '0' ) : ( ( x & ( ~0x20 ) ) - 'A' + 0x0A ) )
#define GET_BYTE( x )       ( GET_BITS( x[0] ) << 4 | GET_BITS( x[1] ) )

namespace memory::impl {
    uintptr_t _scan_region( const uint8_t* region_begin, const uint8_t* region_end, const char* signature, bool to_end ) {
        if ( !region_begin || !region_end || !signature )
            return 0;

        auto scan_result = static_cast< uintptr_t >( 0 );
        auto scan_compare = reinterpret_cast< const uint8_t* >( signature );

        auto sign_size = [ & ]( ) {
            if ( to_end )
                return -1;

            auto size = 0;
            for ( auto i = 0; i < strlen( signature ); i++ ) {
                if ( signature[ i ] != ' ' && signature[ i ] != '?' )
                    size++;
                else if ( signature[ i ] == '?' )
                    size += 2;
            }
            return size / 2 - 1;
        }( );
        // auto sign_size = 0;

        const auto scan_begin = region_begin;
        const auto scan_end = region_end;

        for ( auto scan_current = scan_begin; scan_current < scan_end; scan_current++ ) {
            if ( scan_compare[ 0 ] == '\0' ) {
                // TRACE( "found %s pattern at %#llx", signature, scan_result - sign_size );
                return scan_result - sign_size;
            }

            if ( !const_cast< uint8_t* >( scan_current ) )
                continue;

            if ( scan_compare[ 0 ] == '?' || scan_current[ 0 ] == GET_BYTE( scan_compare ) ) {
                scan_result = reinterpret_cast< uintptr_t >( scan_current );

                if ( scan_compare[ 2 ] == '\0' ) {
                    // TRACE( "found %s pattern at %#llx (%#lx)", signature, scan_result - sign_size,
                    //  scan_result - sign_size - (uintptr_t)region_begin );
                    return scan_result - sign_size;
                }

                const bool question[ 2 ] = {
                  scan_compare[ 0 ] == '?',
                  scan_compare[ 1 ] == '?',
                };

                if ( question[ 0 ] && question[ 1 ] || !question[ 0 ] )
                    scan_compare = ( scan_compare + 3 );
                else
                    scan_compare = ( scan_compare + 2 );

            } else {
                scan_compare = reinterpret_cast< const uint8_t* >( signature );
                scan_result = 0;
            }
        }
        // TRACE( "%s pattern not found!", signature );
        return 0;
    }
    uintptr_t _scan_section( uintptr_t image, const hash_t section_name, const char* signature, bool to_end ) {

        const auto image_nt_headers = pe::get_nt_headers( image );
        if ( !image_nt_headers )
            return 0;

        const auto image_section_headers = IMAGE_FIRST_SECTION( image_nt_headers );

        for ( uint16_t index = 0; index < image_nt_headers->FileHeader.NumberOfSections; index++ ) {
            const auto image_section_header = &image_section_headers[ index ];

            const auto image_section_address = image_section_header->VirtualAddress;
            const auto image_section_size
                = std::min( image_section_header->Misc.VirtualSize, image_section_header->SizeOfRawData );

            if ( image_section_address && image_section_size ) {

                char image_section_name[ 16 ] = { };
                memcpy( image_section_name, image_section_header->Name, sizeof( image_section_header->Name ) );

                if ( !HASH_EQUAL( section_name, image_section_name ) )
                    continue;

                const auto section_begin = reinterpret_cast< const uint8_t* >( image + image_section_address );
                const auto section_end
                    = reinterpret_cast< const uint8_t* >( image + image_section_address + image_section_size );

                return _scan_region( section_begin, section_end, signature, to_end );
            }
        }

        return 0;
    }
    uintptr_t _scan_image( uintptr_t image, const char* signature, bool to_end ) {

        const auto image_nt_headers = pe::get_nt_headers( image );
        if ( !image_nt_headers )
            return 0;

        const auto region_begin = reinterpret_cast< const uint8_t* >( image );
        const auto region_end = reinterpret_cast< const uint8_t* >( image + image_nt_headers->OptionalHeader.SizeOfImage );

        return _scan_region( region_begin, region_end, signature, to_end );
    }
} // namespace memory::impl

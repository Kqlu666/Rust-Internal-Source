#include "image.hpp"
#include "../memory/operation.hpp"
#include "../constant/xor.hpp"
#include "../constant/character.hpp"

const PIMAGE_DOS_HEADER c_image::get_dos_header_impl( ) {

    if ( !m_image )
        return nullptr;

    const auto image_dos_header = reinterpret_cast< const PIMAGE_DOS_HEADER >( m_image );

    if ( image_dos_header->e_magic != IMAGE_DOS_SIGNATURE )
        return nullptr;

    return image_dos_header;
}

const PIMAGE_NT_HEADERS c_image::get_nt_headers_impl( ) {

    if ( !m_dos_header )
        return nullptr;

    const auto image_nt_headers = reinterpret_cast< const PIMAGE_NT_HEADERS >( m_image + m_dos_header->e_lfanew );

    if ( image_nt_headers->Signature != IMAGE_NT_SIGNATURE )
        return nullptr;

    return image_nt_headers;
}

const PIMAGE_SECTION_HEADER c_image::get_section_header_impl( ) {

    if ( !m_nt_headers || !m_nt_headers->FileHeader.NumberOfSections )
        return nullptr;

    return reinterpret_cast< const PIMAGE_SECTION_HEADER >( memory::to_addr( m_nt_headers )
                                                            + FIELD_OFFSET( IMAGE_NT_HEADERS, OptionalHeader ) + m_nt_headers->FileHeader.SizeOfOptionalHeader );
}

const PIMAGE_SECTION_HEADER c_image::get_section_impl( hash_t name_hash ) {

    if ( !m_nt_headers || !m_nt_headers->FileHeader.NumberOfSections ) {
        return nullptr;

        for ( uint16_t index = 0; index < m_nt_headers->FileHeader.NumberOfSections; index++ ) {
            const auto image_section = &m_section_header[ index ];

            char image_section_name[ 16 ] = { };
            std::memcpy( image_section_name, image_section->Name, sizeof( image_section->Name ) );

            if ( RHASH( image_section_name ) == name_hash )
                return image_section;
        }

        return nullptr;
    }
}

const PIMAGE_DATA_DIRECTORY c_image::get_data_dir_impl( uint16_t directory ) {

    if ( !m_nt_headers || directory >= IMAGE_NUMBEROF_DIRECTORY_ENTRIES )
        return nullptr;

    return &m_nt_headers->OptionalHeader.DataDirectory[ directory ];
}

const uintptr_t c_image::get_export_impl( hash_t name_hash ) {
    if ( !name_hash ) {
        return 0;
    }

    const auto image_data_directory = get_data_dir_impl( IMAGE_DIRECTORY_ENTRY_EXPORT );

    if ( !( image_data_directory ) ) {
        return 0;
    }

    const auto image_export_directory
        = reinterpret_cast< const IMAGE_EXPORT_DIRECTORY* >( m_image + image_data_directory->VirtualAddress );

    if ( !( image_export_directory ) ) {
        return 0;
    }

    if ( !image_export_directory->Base || !image_export_directory->AddressOfFunctions
         || !image_export_directory->AddressOfNames || !image_export_directory->AddressOfNameOrdinals
         || !image_export_directory->NumberOfFunctions || !image_export_directory->NumberOfNames ) {
        return 0;
    }

    const auto functions
        = reinterpret_cast< const std::uint32_t* >( m_image + image_export_directory->AddressOfFunctions );
    const auto names = reinterpret_cast< const std::uint32_t* >( m_image + image_export_directory->AddressOfNames );
    const auto name_ordinals
        = reinterpret_cast< const std::uint16_t* >( m_image + image_export_directory->AddressOfNameOrdinals );

    for ( auto index = 0ul; index < image_export_directory->NumberOfFunctions; index++ ) {
        const auto export_ordinal = name_ordinals[ index ];
        const auto export_procedure = functions[ export_ordinal ];

        if ( export_procedure ) {
            const auto export_name = reinterpret_cast< const char* >( m_image + names[ index ] );

            if ( ( export_name ) ) {
                if ( RHASH( export_name ) == name_hash )
                    return ( m_image + export_procedure );
            }
        }
    }

    return 0;
}

const import_t c_image::get_import_impl( hash_t import_name_hash ) {

    const auto image_data_directory = get_data_dir_impl( IMAGE_DIRECTORY_ENTRY_IMPORT );

    if ( !image_data_directory || !import_name_hash )
        return { };

    import_t import_ctx;

    auto image_import_descriptor
        = reinterpret_cast< IMAGE_IMPORT_DESCRIPTOR* >( m_image + image_data_directory->VirtualAddress );

    while ( ( image_import_descriptor ) && image_import_descriptor->Name ) {
        const auto import_image_name = reinterpret_cast< const char* >( m_image + image_import_descriptor->Name );

        if ( !( import_image_name ) )
            continue;

        auto image_thunk_data
            = reinterpret_cast< IMAGE_THUNK_DATA* >( m_image + image_import_descriptor->OriginalFirstThunk );
        auto image_thunk_data_count = static_cast< std::size_t >( 0 );

        while ( ( image_thunk_data ) && image_thunk_data->u1.AddressOfData ) {
            const auto image_import_by_name
                = reinterpret_cast< IMAGE_IMPORT_BY_NAME* >( m_image + image_thunk_data->u1.AddressOfData );

            if ( ( image_import_by_name ) ) {
                if ( RHASH( image_import_by_name->Name ) == import_name_hash ) {
                    strcpy( import_ctx.image_name, import_image_name );
                    strcpy( import_ctx.procedure_name, image_import_by_name->Name );
                    import_ctx.procedure = ( m_image + image_import_descriptor->FirstThunk
                                             + ( image_thunk_data_count * sizeof( std::uintptr_t ) ) );
                    return import_ctx;
                }
            }

            image_thunk_data++;
            image_thunk_data_count++;
        }

        image_import_descriptor++;
    }

    return { };
}

namespace pe {

    const PIMAGE_DOS_HEADER get_dos_header( uintptr_t image ) {

        if ( !image )
            return nullptr;

        const auto image_dos_header = reinterpret_cast< const PIMAGE_DOS_HEADER >( image );

        if ( image_dos_header->e_magic != IMAGE_DOS_SIGNATURE )
            return nullptr;

        return image_dos_header;
    }

    const PIMAGE_NT_HEADERS get_nt_headers( uintptr_t image ) {

        auto dos_header = get_dos_header( image );

        if ( !dos_header )
            return nullptr;

        const auto image_nt_headers = reinterpret_cast< const PIMAGE_NT_HEADERS >( image + dos_header->e_lfanew );

        if ( image_nt_headers->Signature != IMAGE_NT_SIGNATURE )
            return nullptr;

        return image_nt_headers;
    }

    const uint32_t get_image_size( uintptr_t image ) { return get_nt_headers( image )->OptionalHeader.SizeOfImage; }

    namespace impl {

    #define UNLINK( x )                 \
  ( x ).Flink->Blink = ( x ).Blink; \
  ( x ).Blink->Flink = ( x ).Flink

        struct ldr_data_table_entry_t {
            LIST_ENTRY InLoadOrderLinks;           /* 0x00 */
            LIST_ENTRY InMemoryOrderLinks;         /* 0x08 */
            LIST_ENTRY InInitializationOrderLinks; /* 0x10 */
            PVOID DllBase;                         /* 0x18 */
            PVOID EntryPoint;                      /* 0x1C */
            ULONG SizeOfImage;                     /* 0x20 */
            UNICODE_STRING FullDllName;            /* 0x24 */
            UNICODE_STRING BaseDllName;            /* 0x2C */
            ULONG Flags;                           /* 0x34 */
            union {
                UCHAR FlagGroup[ 4 ];
                ULONG Flags;
                struct {
                    /*  bit fields, see below  */
                };
            };
            WORD LoadCount; /* 0x38 */
            WORD TlsIndex;  /* 0x3A */
            union           /* 0x3C */
            {
                LIST_ENTRY HashLinks;
                struct {
                    PVOID SectionPointer;
                    ULONG CheckSum;
                };
            };
            union {
                ULONG TimeDateStamp;
                PVOID LoadedImports;
            };
            _ACTIVATION_CONTEXT* EntryPointActivationContext;
            PVOID PatchInformation;
            LIST_ENTRY ForwarderLinks;
            LIST_ENTRY ServiceTagLinks;
            LIST_ENTRY StaticLinks;
        };

        uintptr_t _get_image( const hash_t module_hash, bool unlink ) {
            auto ldr_data
                = ( reinterpret_cast< PTEB >( __readgsqword( FIELD_OFFSET( NT_TIB, Self ) ) ) )->ProcessEnvironmentBlock->Ldr;

            for ( auto entry_list = ldr_data->InMemoryOrderModuleList.Flink; entry_list != &ldr_data->InMemoryOrderModuleList;
                  entry_list = entry_list->Flink ) {
                auto ldr_entry = CONTAINING_RECORD( entry_list, ldr_data_table_entry_t, InMemoryOrderLinks );
                auto curr_module_name = &ldr_entry->BaseDllName;
                char base_module_name[ 100 ] = { 0 };

                for ( auto idx = 0; idx != ( curr_module_name->Length / 2 ) - 4; idx++ )
                    base_module_name[ idx ] = constant::to_lower( curr_module_name->Buffer[ idx ] );

                if ( !module_hash || HASH_EQUAL( module_hash, base_module_name ) ) {

                    if ( unlink ) {
                        UNLINK( ldr_entry->InLoadOrderLinks );
                        UNLINK( ldr_entry->InMemoryOrderLinks );
                        return reinterpret_cast< uintptr_t >( ldr_entry->DllBase );
                    }
                    // TRACE( "%s module found @ %#llx", base_module_name, ldr_entry->DllBase );
                    return reinterpret_cast< uintptr_t >( ldr_entry->DllBase );
                }
            }
            return 0;
        }

        uintptr_t _get_procedure( uintptr_t base, const hash_t proc_hash, bool rva ) {

            if ( !base )
                return 0;

            const auto& dos = reinterpret_cast< PIMAGE_DOS_HEADER >( base );
            const auto& nt_headers = reinterpret_cast< PIMAGE_NT_HEADERS >( base + dos->e_lfanew );

            if ( !nt_headers->OptionalHeader.SizeOfCode )
                return 0;

            const auto& _export_dir = nt_headers->OptionalHeader.DataDirectory[ IMAGE_DIRECTORY_ENTRY_EXPORT ];
            if ( !_export_dir.VirtualAddress || !_export_dir.Size )
                return 0;

            const auto& export_dir = reinterpret_cast< PIMAGE_EXPORT_DIRECTORY >( base + _export_dir.VirtualAddress );
            const auto& functions_table = reinterpret_cast< uint32_t* >( base + export_dir->AddressOfFunctions );
            const auto& names_table = reinterpret_cast< uint32_t* >( base + export_dir->AddressOfNames );
            const auto& ordinals_table = reinterpret_cast< uint16_t* >( base + export_dir->AddressOfNameOrdinals );

            const auto& is_forwarded = [ & ]( const auto& export_address ) {
                const auto& ui_ied = reinterpret_cast< uintptr_t >( export_dir );
                return export_address > ui_ied && export_address < ui_ied + _export_dir.Size;
            };

            for ( auto idx = 0; idx < export_dir->NumberOfNames; ++idx ) {
                const auto& procedure = base + functions_table[ ordinals_table[ idx ] ];
                const auto& export_name = reinterpret_cast< const char* >( base + names_table[ idx ] );

                if ( !HASH_EQUAL( proc_hash, export_name ) )
                    continue;

                if ( is_forwarded( procedure ) ) {
                    const std::string fwd_name = reinterpret_cast< const char* >( procedure );
                    if ( fwd_name.empty( ) )
                        continue;

                    const auto off = fwd_name.find( _( "." ) );
                    if ( off == std::string::npos )
                        continue;

                    const auto procedure_name_hash = RHASH( fwd_name.substr( off + 1 ).c_str( ) );
                    return _get_procedure( procedure_name_hash, ( uint8_t* )base );
                }

                return rva ? ( uintptr_t )functions_table[ ordinals_table[ idx ] ] : procedure;
            }
            return 0;
        }

        uintptr_t _get_procedure( const hash_t proc_hash, uint8_t* excluded_module, bool rva ) {

            auto ldr_data
                = ( reinterpret_cast< PTEB >( __readgsqword( FIELD_OFFSET( NT_TIB, Self ) ) ) )->ProcessEnvironmentBlock->Ldr;

            for ( auto entry_list = ldr_data->InMemoryOrderModuleList.Flink; entry_list != &ldr_data->InMemoryOrderModuleList;
                  entry_list = entry_list->Flink ) {
                auto ldr_entry = CONTAINING_RECORD( entry_list, ldr_data_table_entry_t, InMemoryOrderLinks );
                auto curr_module_name = reinterpret_cast< PUNICODE_STRING >( &ldr_entry->BaseDllName );
                char base_module_name[ 100 ] = { 0 };

                for ( auto idx = 0; idx != ( curr_module_name->Length / 2 ) - 4; idx++ )
                    base_module_name[ idx ] = constant::to_lower( curr_module_name->Buffer[ idx ] );

                if ( ( uintptr_t )excluded_module == ( uintptr_t )ldr_entry->DllBase )
                    continue;

                if ( auto result = _get_procedure( ( uintptr_t )ldr_entry->DllBase, proc_hash, rva ) )
                    return result;
            }
            return 0;
        }
    } // namespace impl

} // namespace pe

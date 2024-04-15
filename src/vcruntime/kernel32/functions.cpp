#include "functions.hpp"
#include "../constant/xor.hpp"
#include "../memory/operation.hpp"
#include "../memory/scan.hpp"
#include "../nt/functions.hpp"

namespace proc {

    HANDLE NtCurrentProcess( ) noexcept {
        constexpr intptr_t current_process = -1;
        return reinterpret_cast< const HANDLE >( current_process );
    }

    HANDLE NtCurrentThread( ) noexcept {
        constexpr intptr_t current_thread = -2;
        return reinterpret_cast< const HANDLE >( current_thread );
    }

    HANDLE NtCurrentSession( ) noexcept {
        constexpr intptr_t current_session = -3;
        return reinterpret_cast< const HANDLE >( current_session );
    }

    nt::TEB* NtCurrentTeb( ) noexcept {
        const auto displacement = static_cast< uint32_t >( FIELD_OFFSET( NT_TIB, Self ) );
        const auto teb = __readgsqword( displacement );

        return reinterpret_cast< nt::TEB* >( teb );
    }

    nt::PEB* NtCurrentPeb( ) noexcept {
        const auto teb = NtCurrentTeb( );

        if ( memory::is_valid( teb ) )
            return teb->ProcessEnvironmentBlock;

        return nullptr;
    }

    uint32_t NtCurrentProcessId( ) noexcept {
        const auto teb = NtCurrentTeb( );

        if ( memory::is_valid( teb ) ) {
            const auto process_id = memory::to_addr( teb->ClientId.UniqueProcess );
            return static_cast< uint32_t >( process_id );
        }

        return 0;
    }

    uint32_t NtCurrentThreadId( ) noexcept {
        const auto teb = NtCurrentTeb( );

        if ( memory::is_valid( teb ) ) {
            const auto thread_id = memory::to_addr( teb->ClientId.UniqueThread );
            return static_cast< uint32_t >( thread_id );
        }

        return 0;
    }

    int32_t GetLastStatus( ) noexcept {
        const auto teb = NtCurrentTeb( );

        if ( memory::is_valid( teb ) ) {
            return static_cast< int32_t >( teb->LastStatusValue );
        }

        return STATUS_INVALID_THREAD;
    }

    void SetLastStatus( int32_t last_status ) noexcept {
        const auto teb = NtCurrentTeb( );

        if ( memory::is_valid( teb ) )
            teb->LastStatusValue = static_cast< uint32_t >( last_status );
    }

    uint32_t GetLastError( ) noexcept {
        const auto teb = NtCurrentTeb( );

        if ( memory::is_valid( teb ) )
            return teb->LastErrorValue;

        return ERROR_INVALID_THREAD_ID;
    }

    void SetLastError( uint32_t last_error ) noexcept {
        const auto teb = NtCurrentTeb( );

        if ( memory::is_valid( teb ) ) {
            teb->LastErrorValue = last_error;
        }
    }

    void* GetProcessHeap( ) noexcept {
        const auto peb = NtCurrentPeb( );

        if ( memory::is_valid( peb ) ) {
            return peb->ProcessHeap;
        }

        return nullptr;
    }

} // namespace proc

LPVOID VirtualAllocEx( HANDLE process, LPVOID address, SIZE_T size, DWORD allocation_type, DWORD protect ) {
    const auto result = nt::NtAllocateVirtualMemory( process, &address, 0, &size, allocation_type, protect );

    if ( result != 0 ) {
        return nullptr;
    }

    if ( process == GetCurrentProcess( ) ) {
        static std::uint8_t data[ 4096 ] = { };
        std::memset( data, 0, 0x1000 );

        for ( auto i = 0; i <= static_cast< std::int32_t >( size ); i += PAGE_SIZE ) {
            const auto page_address = reinterpret_cast< std::uint8_t* >( address ) + i;

            if ( protect == PAGE_READWRITE || protect == PAGE_EXECUTE_READWRITE ) {
                mem::copy( page_address, data, 0x1000 );
            } else if ( protect == PAGE_READONLY || protect == PAGE_EXECUTE_READ ) {
                mem::copy( data, page_address, 0x1000 );
            }
        }
    }

    return address;
}

LPVOID VirtualAlloc( void* address, std::size_t size, DWORD allocation, DWORD protect ) {
    return VirtualAllocEx( GetCurrentProcess( ), address, size, allocation, protect );
}

BOOL VirtualFreeEx( HANDLE process, void* address, std::size_t size, DWORD type ) {
    const auto result = nt::NtFreeVirtualMemory( process, &address, &size, type );
    return ( result == 0 );
}

BOOL VirtualFree( void* address, std::size_t size, DWORD type ) {
    return VirtualFreeEx( GetCurrentProcess( ), address, size, type );
}

BOOL ReadProcessMemory( HANDLE process, const void* address, void* buffer, std::size_t size, std::size_t* read ) {
    if ( read )
        *read = 0;

    const auto result = nt::NtReadVirtualMemory( process, ( void* )address, buffer, size, ( ULONG* )read );
    return ( result >= 0 );
}

BOOL WriteProcessMemory( HANDLE process, void* address, const void* buffer, std::size_t size, std::size_t* write ) {
    if ( write )
        *write = 0;

    const auto result = nt::NtWriteVirtualMemory( process, address, ( void* )buffer, size, ( ULONG* )write );
    return ( result >= 0 );
}

void* HeapAlloc( HANDLE process_heap, DWORD flags, std::size_t size ) {
    auto v3 = size;
    auto v4 = flags;
    auto v5 = memory::to_addr( process_heap );

    if ( size > 0 ) {
        static std::uintptr_t RtlpHpAllocWithExceptionProtection = 0;

        if ( !RtlpHpAllocWithExceptionProtection ) {
            RtlpHpAllocWithExceptionProtection = memory::scan_image( HASH( "ntdll" ), _( "48 83 EC 58 F6" ) ).as( ); // TO DO
            if ( !memory::is_valid( RtlpHpAllocWithExceptionProtection ) )
                return nullptr;
        }

        return safe.call_fn< void* ( * )( std::uintptr_t, std::size_t, DWORD ) >( RtlpHpAllocWithExceptionProtection )(
            v5, v3, v4 );
    }
    return nullptr;
}

BOOL HeapFree( HANDLE process_heap, DWORD flags, void* memory ) {
    auto v3 = memory;
    auto v4 = flags;
    auto v5 = memory::to_addr( process_heap );

    if ( memory::is_valid( memory ) ) {
        static std::uintptr_t RtlpHpFreeWithExceptionProtection = 0;

        if ( !RtlpHpFreeWithExceptionProtection ) {
            RtlpHpFreeWithExceptionProtection
                = memory::scan_image( HASH( "ntdll" ), _( "E8 ? ? ? ? EB E2 48" ) ).rel( 1 ).as< uintptr_t >( );

            if ( !memory::is_valid( RtlpHpFreeWithExceptionProtection ) )
                return false;
        }

        return safe.call_fn< BOOL( * )( std::uintptr_t, void*, DWORD ) >( RtlpHpFreeWithExceptionProtection )(
            v5, v3, v4 );
    }

    return false;
}


HANDLE GetProcessHeap( ) { return proc::GetProcessHeap( ); }

HANDLE GetCurrentProcess( ) { return proc::NtCurrentProcess( ); }

DWORD GetCurrentProcessId( ) { return proc::NtCurrentProcessId( ); }

HANDLE GetCurrentThread( ) { return proc::NtCurrentThread( ); }

DWORD GetCurrentThreadId( ) { return proc::NtCurrentThreadId( ); }

FARPROC GetProcAddress( HMODULE image, const char* procedure_name ) {
    const auto image_base = reinterpret_cast< std::uintptr_t >( image );

    if ( !memory::is_valid( image_base ) ) {
        return nullptr;
    }

    const auto procedure = pe::get_procedure( ( uint8_t* )image_base, RHASH( procedure_name ) );
    if ( !memory::is_valid( procedure ) ) {
        return nullptr;
    }

    return reinterpret_cast< FARPROC >( procedure );
}

HMODULE GetModuleHandleW( const wchar_t* image_name ) {
    const auto image_hash = ( image_name ? RHASH( image_name ) : 0 );

    const auto image_base = pe::get_image( image_hash );
    if ( !memory::is_valid( image_base ) )
        return nullptr;

    return reinterpret_cast< HMODULE >( image_base );
}

HMODULE GetModuleHandleA( const char* image_name ) {
    const auto image_hash = ( image_name ? RHASH( image_name ) : 0 );

    const auto image_base = pe::get_image( image_hash );
    if ( !memory::is_valid( image_base ) )
        return nullptr;

    return reinterpret_cast< HMODULE >( image_base );
}

NTSTATUS NtQuerySystemInformation( SYSTEM_INFORMATION_CLASS system_information_class, void* system_information,
                                   ULONG system_information_size, PULONG size ) {
    return nt::NtQuerySystemInformation( system_information_class, system_information, system_information_size, size );
}

size_t VirtualQueryEx( HANDLE process, LPCVOID address, MEMORY_BASIC_INFORMATION* buffer, SIZE_T size ) {
    std::size_t result = 0;
    nt::NtQueryVirtualMemory( process, ( void* )( address ), 0, buffer, size, &result );
    return result;
}

SIZE_T VirtualQuery( const void* address, MEMORY_BASIC_INFORMATION* buffer, std::size_t size ) {
    return VirtualQueryEx( GetCurrentProcess( ), address, buffer, size );
}

BOOL VirtualProtectEx( HANDLE process, void* address, std::size_t size, DWORD protect, DWORD* restore ) {
    const auto result = nt::NtProtectVirtualMemory( process, &address, &size, protect, restore );
    return ( result == 0 );
}

BOOL VirtualProtect( void* address, SIZE_T dwSize, DWORD protect, DWORD* restore ) {
    return VirtualProtectEx( GetCurrentProcess( ), address, dwSize, protect, restore );
}

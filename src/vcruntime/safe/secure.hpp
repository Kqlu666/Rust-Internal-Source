#pragma once
#include "../pe/image.hpp"
#include "../../util/hde/hde64.hpp"
#include "../constant/xor.hpp"

#include "../memory/operation.hpp"
#include "../memory/scan.hpp"
#include "safe.hpp"

#define system_time *reinterpret_cast< uint64_t* >( 0x7ffe0014 )
#define cookie      *reinterpret_cast< uint64_t* >( 0x7ffe0330 )

#define TICKS_PER_SECOND   10000000
#define UNIX_TIME_START    0x019db1ded53e8000
#define GET_TIMESTAMP( t ) ( t - UNIX_TIME_START ) / TICKS_PER_SECOND
#define MAX_TIMESTAMP      1711247096

union bytes_t {
    uint64_t u64;
    uint32_t u32[ 2 ];
    uint8_t u8[ 8 ];
};

class c_secure {
public:
    UNICODE_STRING us;

    bool validate_cookie( uint64_t reg_cookie );
    uint64_t get_hashed_time( );

    std::pair< HANDLE, NTSTATUS > mutant_check( );
    bool registry_check( );

    void show_raise_message( std::wstring message, std::wstring caption, uint32_t flags );

    bool initialize( );
};

inline c_secure secure;

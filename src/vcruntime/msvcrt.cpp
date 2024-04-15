#include "include.hpp"
#include "kernel32/functions.hpp"
#include "constant/xor.hpp"

void* calloc( size_t count, size_t size ) {
    return HeapAlloc( GetProcessHeap( ), 8, size * count );
}

void* malloc( size_t size ) {
    return HeapAlloc( GetProcessHeap( ), 0, size );
}

void free( void* pointer ) {
    HeapFree( GetProcessHeap( ), 0, pointer );
}

void* operator new( size_t size ) {
    const auto process_heap = GetProcessHeap( );
    return HeapAlloc( process_heap, 0, size );
}

void* operator new[ ]( size_t size ) {
    const auto process_heap = GetProcessHeap( );
    return HeapAlloc( process_heap, 0, size );
}

void operator delete( void* data ) noexcept {
    const auto process_heap = GetProcessHeap( );
    HeapFree( process_heap, 0, data );
}

void operator delete( void* data, size_t size ) noexcept {
    const auto process_heap = GetProcessHeap( );
    HeapFree( process_heap, 0, data );
}

void operator delete[ ]( void* data ) noexcept {
    const auto process_heap = GetProcessHeap( );
    HeapFree( process_heap, 0, data );
}

void operator delete[ ]( void* data, size_t size ) noexcept {
    const auto process_heap = GetProcessHeap( );
    HeapFree( process_heap, 0, data );
}

#pragma warning( pop )

struct HANDLE_FILE {
    HANDLE hFile = nullptr;
};

HANDLE_FILE Handlers[ 3 ] = {
  nullptr,
  nullptr,
  nullptr,
};

FILE* __acrt_iob_func( unsigned i ) {
    const auto pointer = *( UINT64* )( __readgsqword( 0x60 ) + 32 );

    if ( !memory::is_valid( pointer ) ) {
        return nullptr;
    }

    const auto pointer_array = reinterpret_cast< HANDLE* >( pointer + 32 );

    if ( !memory::is_valid( pointer_array ) ) {
        return nullptr;
    }

    Handlers[ i ] = { pointer_array[ i ] };
    return ( FILE* )&Handlers[ i ];
}

void InitUnicodeString( UNICODE_STRING* pointer, wchar_t* string ) {
    const auto size = static_cast< USHORT >( std::wcslen( string ) );

    if ( !size )
        return;

    pointer->Length = size * sizeof( wchar_t );
    pointer->MaximumLength = ( size + 1 ) * sizeof( wchar_t );

    pointer->Buffer = string;
}

void initialize_nt_path( const wchar_t* path, wchar_t* nt ) {
    const auto size = static_cast< std::size_t >( std::wcslen( path ) );

    if ( path[ size - 1 ] == L'$' ) {
        std::memcpy( nt, _( L"\\??\\" ), 8 );
        std::memcpy( &nt[ 4 ], path, ( size + 1 ) * sizeof( WCHAR ) );
    }
    else if ( ( *path == L'\\' || path[ 1 ] != L':' ) ) {
        WCHAR directory[ 256 ] = { };
        GetCurrentDirectoryW( 256, directory );

        const auto size = static_cast< std::size_t >( std::wcslen( directory ) );

        if ( size > 1 ) {
            if ( directory[ size - 1 ] == L'\\' )
                directory[ size - 1 ] = 0x0000;

            swprintf( nt, _( L"\\??\\%s%s%s" ), directory, *path == L'\\' ? L"" : L"\\", path );
        }
    }
    else {
        std::memcpy( nt, _( L"\\??\\" ), 8 );
        std::memcpy( &nt[ 4 ], path, ( size + 1 ) * sizeof( WCHAR ) );
    }
}

DWORD GetCurrentDirectoryW( DWORD output_size, wchar_t* output ) {
    const auto pointer = *( std::uintptr_t* )( __readgsqword( 0x60 ) + 32 );

    if ( !( pointer ) ) {
        return 0;
    }

    const auto size = *( std::uint16_t* )( pointer + 56 ) >> 1;

    if ( size > output_size ) {
        return 0;
    }

    std::memcpy( output, *( const void** )( pointer + 64 ), size * sizeof( wchar_t ) );
    output[ size ] = L'\0';

    return size;
}

FILE* _wfopen( const wchar_t* name, const wchar_t* mode ) {
    WCHAR path[ 256 ] = { };
    initialize_nt_path( name, path );

    UNICODE_STRING string = { };
    InitUnicodeString( &string, path );

    OBJECT_ATTRIBUTES object_attributes = { };
    InitializeObjectAttributes( &object_attributes, &string, OBJ_CASE_INSENSITIVE, nullptr, nullptr );

    auto access = FILE_GENERIC_READ;

    if ( std::wcschr( mode, L'w' ) || std::wcschr( mode, L'a' ) || std::wcschr( mode, L'+' ) )
        access |= FILE_GENERIC_WRITE;

    const auto create = ( !std::wcschr( mode, L'r' ) && std::wcschr( mode, L'+' ) ) || std::wcschr( mode, L'w' )
        || std::wcschr( mode, L'a' );
    const auto replace = ( std::wcschr( mode, L'w' ) );

    auto disposition = ( replace ? FILE_OVERWRITE_IF : ( create ? FILE_OPEN_IF : FILE_OPEN ) );
    auto share = ( FILE_SHARE_READ | FILE_SHARE_WRITE );

    if ( !std::memcmp( name, _( L"CONIN" ), 10 ) ) {
        share = FILE_SHARE_READ;
    }

    if ( !std::memcmp( name, _( L"CONOUT" ), 12 ) || !std::memcmp( name, _( L"CONERR" ), 12 ) ) {
        share = FILE_SHARE_WRITE;
    }

    HANDLE_FILE result = { };
    IO_STATUS_BLOCK io_status_block = { };

    const auto status = nt::NtCreateFile( &result.hFile, access, &object_attributes, &io_status_block, nullptr,
                                          FILE_ATTRIBUTE_NORMAL, share, disposition, FILE_SYNCHRONOUS_IO_NONALERT, nullptr, 0 );
    return ( FILE* )( status < 0 ? nullptr : new HANDLE_FILE( result ) );
}

FILE* fopen( const char* name, const char* mode ) {
    WCHAR output_path[ 256 ] = { };
    swprintf( output_path, _( L"%S" ), name );

    WCHAR output_mode[ 20 ] = { };
    swprintf( output_mode, _( L"%S" ), mode );

    return _wfopen( output_path, output_mode );
}

long ftell( FILE* stream ) {
    const auto pointer = reinterpret_cast< HANDLE_FILE* >( stream );

    if ( !pointer )
        return 0;

    LARGE_INTEGER large_integer = { };
    IO_STATUS_BLOCK io_status_block = { };

    const auto status = nt::NtQueryInformationFile(
        pointer->hFile, &io_status_block, &large_integer, 8, static_cast< FILE_INFORMATION_CLASS >( 14 ) );
    return ( status < 0 ? 0 : large_integer.QuadPart );
}

int fseek( FILE* stream, long offset, int origin ) {
    const auto pointer = reinterpret_cast< HANDLE_FILE* >( stream );

    if ( !pointer )
        return -1;

    LARGE_INTEGER large_integer = { };
    large_integer.QuadPart = offset;

    IO_STATUS_BLOCK io_status_block = { };

    if ( origin == SEEK_END ) {
        char output[ 24 ] = { };

        if ( nt::NtQueryInformationFile(
            pointer->hFile, &io_status_block, output, 24, static_cast< FILE_INFORMATION_CLASS >( 5 ) )
            < 0 )
            return -1;

        large_integer = *( ( LARGE_INTEGER* )( &output[ 8 ] ) );
    }
    else if ( origin == SEEK_CUR ) {
        large_integer.QuadPart += ftell( stream );
    }

    return nt::NtSetInformationFile(
        pointer->hFile, &io_status_block, &large_integer, 8, static_cast< FILE_INFORMATION_CLASS >( 14 ) );
}

int fclose( FILE* stream ) {
    const auto pointer = reinterpret_cast< HANDLE_FILE* >( stream );

    if ( !pointer )
        return -1;

    return nt::NtClose( pointer->hFile );
}

FILE* freopen( const char* name, const char* mode, FILE* stream ) {
    HANDLE_FILE* result = reinterpret_cast< HANDLE_FILE* >( stream );

    if ( !result ) {
        return nullptr;
    }

    const auto pointer = *( UINT64* )( __readgsqword( 0x60 ) + 32 );

    if ( !memory::is_valid( pointer ) ) {
        return nullptr;
    }

    const auto pointer_array = reinterpret_cast< HANDLE* >( pointer + 32 );

    if ( !memory::is_valid( pointer_array ) ) {
        return nullptr;
    }

    if ( std::strstr( name, _( "CONIN" ) ) ) {
        result->hFile = pointer_array[ 0 ];
    }
    else if ( std::strstr( name, _( "CONOUT" ) ) ) {
        result->hFile = pointer_array[ 1 ];
    }
    else if ( std::strstr( name, _( "CONERR" ) ) ) {
        result->hFile = pointer_array[ 2 ];
    }

    return stream;
}

std::size_t fwrite( const void* pointer, std::size_t size, std::size_t count, FILE* stream ) {
    HANDLE_FILE* handle_file = reinterpret_cast< HANDLE_FILE* >( stream );

    if ( !( handle_file ) ) {
        return 0;
    }

    IO_STATUS_BLOCK io_status_block = { };

    const auto status
        = nt::NtWriteFile( handle_file->hFile, NULL, NULL, NULL, &io_status_block, ( void* )pointer, size * count, 0, 0 );
    return ( ( status < 0 ) ? static_cast< std::size_t >( 0 ) : io_status_block.Information );
}

std::size_t fread( void* pointer, std::size_t size, std::size_t count, FILE* stream ) {
    HANDLE_FILE* handle_file = reinterpret_cast< HANDLE_FILE* >( stream );

    if ( !( handle_file ) ) {
        return 0;
    }

    IO_STATUS_BLOCK io_status_block = { };

    const auto status
        = nt::NtReadFile( handle_file->hFile, NULL, NULL, NULL, &io_status_block, pointer, size * count, 0, 0 );
    return ( ( status < 0 ) ? static_cast< std::size_t >( 0 ) : io_status_block.Information );
}

int getchar( ) {
    wchar_t character = 0;
    fread( &character, 2, 1, stdin );
    return static_cast< int >( character );
}

int putchar( int character ) {
    return static_cast< int >( fwrite( &character, 2, 1, stdout ) );
}

#pragma warning( pop )

#include <xmmintrin.h>
#include <cassert>

//
// msvcrt.dll
//

constexpr float XM_PI = 3.141592654f;
constexpr float XM_2PI = 6.283185307f;
constexpr float XM_1DIVPI = 0.318309886f;
constexpr float XM_1DIV2PI = 0.159154943f;
constexpr float XM_PIDIV2 = 1.570796327f;
constexpr float XM_PIDIV4 = 0.785398163f;

#define GET_FLOAT_WORD( i, d )  \
  do {                          \
    ieee_float_shape_type gf_u; \
    gf_u.value = ( d );         \
    ( i ) = gf_u.word;          \
  } while ( 0 )

#define SET_FLOAT_WORD( d, i )  \
  do {                          \
    ieee_float_shape_type sf_u; \
    sf_u.word = ( i );          \
    ( d ) = sf_u.value;         \
  } while ( 0 )

#define IS_DIGIT( character ) ( character >= '0' && character <= '9' )

//
// [ procedure ] implementation
//

union ieee_float_shape_type {
    float value;
    std::uint32_t word;
};

#pragma function( roundf )
float roundf( float x ) {
    std::uint32_t w;
    GET_FLOAT_WORD( w, x );

    const auto exponent_less_127 = static_cast< std::int32_t >( ( w & 0x7f800000 ) >> 23 ) - 127;

    if ( exponent_less_127 < 23 ) {
        if ( exponent_less_127 < 0 ) {
            w &= 0x80000000;

            if ( exponent_less_127 == -1 ) {
                w |= ( static_cast< std::uint32_t >( 127 ) << 23 );
            }
        }
        else {
            const auto exponent_mask = 0x007fffffu >> exponent_less_127;

            if ( ( w & exponent_mask ) == 0 ) {
                return x;
            }

            w += 0x00400000 >> exponent_less_127;
            w &= ~exponent_mask;
        }
    }
    else {
        if ( exponent_less_127 == 128 ) {
            return x + x;
        }
        else {
            return x;
        }
    }

    SET_FLOAT_WORD( x, w );
    return x;
}

#pragma function( fabs )
double fabs( double x ) {
    return ( x < 0.f ? -x : x );
}

#pragma function( sqrtf )
float sqrtf( float x ) {
    auto root = _mm_set_ss( x );
    root = _mm_sqrt_ss( root );
    return _mm_cvtss_f32( root );
}

#pragma function( sinf )
float sinf( float x ) {
    auto quotient = XM_1DIV2PI * x;

    if ( x >= 0.f ) {
        quotient = static_cast< float >( static_cast< int >( quotient + 0.5f ) );
    }
    else {
        quotient = static_cast< float >( static_cast< int >( quotient - 0.5f ) );
    }

    auto y = x - XM_2PI * quotient;
    auto sign = 0.f;

    if ( y > XM_PIDIV2 ) {
        y = XM_PI - y;
        sign = -1.f;
    }
    else if ( y < -XM_PIDIV2 ) {
        y = -XM_PI - y;
        sign = -1.f;
    }
    else {
        sign = +1.f;
    }

    auto y2 = y * y;
    return ( ( ( ( ( ( -2.3889859e-08f * y2 + 2.7525562e-06f ) * y2 - 0.00019840874f ) * y2 + 0.0083333310f ) * y2
             - 0.16666667f )
             * y2
             + 1.f )
             * y );
}

#pragma function( cosf )
float cosf( float x ) {
    auto quotient = XM_1DIV2PI * x;

    if ( x >= 0.f ) {
        quotient = static_cast< float >( static_cast< int >( quotient + 0.5f ) );
    }
    else {
        quotient = static_cast< float >( static_cast< int >( quotient - 0.5f ) );
    }

    auto y = x - XM_2PI * quotient;
    auto sign = 0.f;

    if ( y > XM_PIDIV2 ) {
        y = XM_PI - y;
        sign = -1.f;
    }
    else if ( y < -XM_PIDIV2 ) {
        y = -XM_PI - y;
        sign = -1.f;
    }
    else {
        sign = +1.f;
    }

    auto y2 = y * y;
    auto p = ( ( ( ( ( -2.6051615e-07f * y2 + 2.4760495e-05f ) * y2 - 0.0013888378f ) * y2 + 0.041666638f ) * y2 - 0.5f )
               * y2
               + 1.f );

    return ( sign * p );
}

#pragma function( tanf )
float tanf( float x ) {
    auto s = sinf( x );
    auto c = cosf( x );

    return ( s / c );
}

#pragma function( asinf )
float asinf( float x ) {
    float negate = float( x < 0 );
    x = abs( x );
    float ret = -0.0187293;
    ret *= x;
    ret += 0.0742610;
    ret *= x;
    ret -= 0.2121144;
    ret *= x;
    ret += 1.5707288;
    ret = 3.14159265358979 * 0.5 - sqrtf( 1.0 - x ) * ret;
    return ret - 2 * negate * ret;
}

#pragma function( acosf )
float acosf( float x ) {
    auto non_negative = ( x >= 0.f );

    auto pox = fabsf( x );
    auto omx = 1.f - pox;

    if ( omx < 0.f ) {
        omx = 0.f;
    }

    auto root = sqrtf( omx );
    auto result
        = ( ( ( ( ( ( ( -0.0012624911f * pox + 0.0066700901f ) * pox - 0.0170881256f ) * pox + 0.0308918810f ) * pox
            - 0.0501743046f )
            * pox
            + 0.0889789874f )
            * pox
            - 0.2145988016f )
            * pox
            + 1.5707963050f );

    result *= root;
    return ( non_negative ? result : ( XM_PI - result ) );
}

#pragma function( atanf )
float atanf( float x ) {
    auto x2 = x * x;
    auto root = sqrtf( 1.f + x2 );
    return asinf( x / root );
}

#pragma function( atan2f )
float atan2f( float y, float x ) {
    if ( x == 0 && y == 0 )
        return 0;

    float a = std::min( abs( x ), abs( y ) ) / std::max( abs( x ), abs( y ) );
    float s = a * a;
    float r = ( ( -0.0464964749f * s + 0.15931422f ) * s - 0.327622764f ) * s * a + a;

    if ( abs( y ) > abs( x ) )
        r = ( float )( XM_PI / 2 ) - r;

    if ( x < 0 )
        r = ( float )XM_PI - r;

    if ( y < 0 )
        r = -r;

    return r;
}

#pragma function( atan2 )
double atan2( double y, double x ) {
    return static_cast< double >( atan2f( static_cast< float >( y ), static_cast< float >( x ) ) );
}

#pragma function( atoi )
int atoi( const char* data ) {
    auto result = 0;

    for ( auto i = 0; data[ i ] != '\0'; ++i )
        result = ( result * 10 + data[ i ] - '0' );

    return result;
}

#pragma function( atof )
double atof( const char* s ) {
    double a = 0.0;
    int e = 0;
    int c;

    while ( ( c = *s++ ) != '\0' && IS_DIGIT( c ) ) {
        a = a * 10.0 + ( c - '0' );
    }

    if ( c == '.' ) {
        while ( ( c = *s++ ) != '\0' && IS_DIGIT( c ) ) {
            a = a * 10.0 + ( c - '0' );
            e = e - 1;
        }
    }

    if ( c == 'e' || c == 'E' ) {
        int sign = 1;
        int i = 0;
        c = *s++;
        if ( c == '+' )
            c = *s++;
        else if ( c == '-' ) {
            c = *s++;
            sign = -1;
        }
        while ( IS_DIGIT( c ) ) {
            i = i * 10 + ( c - '0' );
            c = *s++;
        }
        e += i * sign;
    }

    while ( e > 0 ) {
        a *= 10.0;
        e--;
    }

    while ( e < 0 ) {
        a *= 0.1;
        e++;
    }

    return a;
}

#pragma function( powf )
float powf( float a, float b ) {
    union {
        double d;
        int x[ 2 ];
    } u = { a };

    u.x[ 1 ] = ( int )( b * ( u.x[ 1 ] - 1072632447 ) + 1072632447 );
    u.x[ 0 ] = 0;

    return u.d;
}

#pragma function( pow )
double pow( double x, double y ) {
    return static_cast< double >( powf( static_cast< float >( x ), static_cast< float >( y ) ) );
}

constexpr float One = 1.0;

constexpr float Zero[ ] = {
  0.0,
  -0.0,
};

#pragma function( fmodf )
float fmodf( float x, float y ) {
    int32_t n, hx, hy, hz, ix, iy, sx, i;

    hx = LODWORD( x );
    hy = LODWORD( y );

    sx = hx & 0x80000000; /* sign of x */
    hx ^= sx;             /* |x| */
    hy &= 0x7fffffff;     /* |y| */

    /* purge off exception values */
    if ( hy == 0 || ( hx >= 0x7f800000 ) || /* y=0,or x not finite */
         ( hy > 0x7f800000 ) )               /* or y is NaN */
        return ( x * y ) / ( x * y );
    if ( hx < hy )
        return x;                        /* |x|<|y| return x */
    if ( hx == hy )
        return Zero[ ( uint32_t )sx >> 31 ]; /* |x|=|y| return x*0*/

    /* determine ix = ilogb(x) */
    if ( hx < 0x00800000 ) { /* subnormal x */
        for ( ix = -126, i = ( hx << 8 ); i > 0; i <<= 1 )
            ix -= 1;
    }
    else
        ix = ( hx >> 23 ) - 127;

    /* determine iy = ilogb(y) */
    if ( hy < 0x00800000 ) { /* subnormal y */
        for ( iy = -126, i = ( hy << 8 ); i >= 0; i <<= 1 )
            iy -= 1;
    }
    else
        iy = ( hy >> 23 ) - 127;

    /* set up {hx,lx}, {hy,ly} and align y to x */
    if ( ix >= -126 )
        hx = 0x00800000 | ( 0x007fffff & hx );
    else { /* subnormal x, shift x to normal */
        n = -126 - ix;
        hx = hx << n;
    }
    if ( iy >= -126 )
        hy = 0x00800000 | ( 0x007fffff & hy );
    else { /* subnormal y, shift y to normal */
        n = -126 - iy;
        hy = hy << n;
    }

    /* fix point fmod */
    n = ix - iy;
    while ( n-- ) {
        hz = hx - hy;
        if ( hz < 0 ) {
            hx = hx + hx;
        }
        else {
            if ( hz == 0 ) /* return sign(x)*0 */
                return Zero[ ( uint32_t )sx >> 31 ];
            hx = hz + hz;
        }
    }

    hz = hx - hy;

    if ( hz >= 0 ) {
        hx = hz;
    }

    if ( hx == 0 )
        return Zero[ ( uint32_t )sx >> 31 ];

    while ( hx < 0x00800000 ) { /* normalize x */
        hx = hx + hx;
        iy -= 1;
    }

    if ( iy >= -126 ) { /* normalize output */
        hx = ( ( hx - 0x00800000 ) | ( ( iy + 127 ) << 23 ) );
        LODWORD( x ) = hx | sx;
        // SET_FLOAT_WORD( x, hx | sx );
    }
    else { /* subnormal output */
        n = -126 - iy;
        hx >>= n;
        LODWORD( x ) = hx | sx;

        x *= One;
    }

    return x;
}

#pragma function( floorf )
float floorf( float X ) {
    float v1;        // edx
    unsigned int v2; // eax
    UCHAR v3;        // r8
    int v4;          // eax

    v1 = X;
    v2 = LODWORD( X ) & 0x7FFFFFFF;
    v3 = LODWORD( X ) != ( LODWORD( X ) & 0x7FFFFFFF );

    if ( ( LODWORD( X ) & 0x7FFFFFFFu ) < 0x4B800000 ) {
        if ( v2 >= 0x3F800000 ) {
            v4 = LODWORD( X ) & ( -1 << ( -106 - ( LODWORD( X ) >> 23 ) ) );
            LODWORD( X ) &= -1 << ( -106 - ( LODWORD( X ) >> 23 ) );
            if ( v3 && LODWORD( v1 ) != v4 )
                X = X - 1.f;
        }
        else if ( v2 ) {
            if ( LODWORD( X ) != ( LODWORD( X ) & 0x7FFFFFFF ) )
                X = -1.0;
            else
                X = 0.0;
        }
    }
    else if ( v2 > 0x7F800000 ) {
        int ret = SLODWORD( X ) | 0x400000;
        X = *( float* )&ret;
    }

    return X;
}

#pragma function( ceilf )
float ceilf( float X ) {
    float v1;        // edx
    unsigned int v2; // eax
    UCHAR v3;        // r8
    int v4;          // eax

    v1 = X;
    v2 = LODWORD( X ) & 0x7FFFFFFF;
    v3 = LODWORD( X ) != ( LODWORD( X ) & 0x7FFFFFFF );

    if ( ( LODWORD( X ) & 0x7FFFFFFFu ) < 0x4B800000 ) {
        if ( v2 >= 0x3F800000 ) {
            v4 = LODWORD( X ) & ( -1 << ( -106 - ( LODWORD( X ) >> 23 ) ) );
            LODWORD( X ) &= -1 << ( -106 - ( LODWORD( X ) >> 23 ) );
            if ( !v3 && LODWORD( v1 ) != v4 )
                X = X + 1.f;
        }
        else if ( v2 ) {
            if ( LODWORD( X ) != ( LODWORD( X ) & 0x7FFFFFFF ) )
                X = -0.0;
            else
                X = 1.0;
        }
    }
    else if ( v2 > 0x7F800000 ) {
        int ret = SLODWORD( X ) | 0x400000;
        X = *( float* )&ret;
    }

    return X;
}

std::uint16_t lfsr = 0xACE1;
std::uint32_t bit = 0;

int rand( ) {
    bit = ( ( lfsr >> 0 ) ^ ( lfsr >> 2 ) ^ ( lfsr >> 3 ) ^ ( lfsr >> 5 ) ) & 1;
    return lfsr = ( lfsr >> 1 ) | ( bit << 15 );
}

typedef int ( *__compar_fn_t )( const void*, const void* );
typedef int ( *__compar_d_fn_t )( const void*, const void*, const void* );
struct msort_param {
    size_t s;
    size_t var;
    __compar_d_fn_t cmp;
    void* arg;
    char* t;
};

void* mempcpy( void* destination, const void* source, std::size_t size ) {
    const auto result = static_cast< std::uint8_t* >( memcpy( destination, source, size ) );
    return ( result + size );
}

static void msort_with_tmp( const struct msort_param* p, void* b, size_t n ) {
    char* b1, * b2;
    size_t n1, n2;

    if ( n <= 1 )
        return;

    n1 = n / 2;
    n2 = n - n1;
    b1 = ( char* )b;
    b2 = ( char* )b + ( n1 * p->s );

    msort_with_tmp( p, b1, n1 );
    msort_with_tmp( p, b2, n2 );

    char* tmp = p->t;
    const size_t s = p->s;
    __compar_d_fn_t cmp = p->cmp;
    void* arg = p->arg;
    switch ( p->var ) {
        case 0:
            while ( n1 > 0 && n2 > 0 ) {
                if ( ( *cmp )( b1, b2, arg ) <= 0 ) {
                    *( uint32_t* )tmp = *( uint32_t* )b1;
                    b1 += sizeof( uint32_t );
                    --n1;
                }
                else {
                    *( uint32_t* )tmp = *( uint32_t* )b2;
                    b2 += sizeof( uint32_t );
                    --n2;
                }
                tmp += sizeof( uint32_t );
            }
            break;
        case 1:
            while ( n1 > 0 && n2 > 0 ) {
                if ( ( *cmp )( b1, b2, arg ) <= 0 ) {
                    *( uint64_t* )tmp = *( uint64_t* )b1;
                    b1 += sizeof( uint64_t );
                    --n1;
                }
                else {
                    *( uint64_t* )tmp = *( uint64_t* )b2;
                    b2 += sizeof( uint64_t );
                    --n2;
                }
                tmp += sizeof( uint64_t );
            }
            break;
        case 2:
            while ( n1 > 0 && n2 > 0 ) {
                unsigned long* tmpl = ( unsigned long* )tmp;
                unsigned long* bl;

                tmp += s;
                if ( ( *cmp )( b1, b2, arg ) <= 0 ) {
                    bl = ( unsigned long* )b1;
                    b1 += s;
                    --n1;
                }
                else {
                    bl = ( unsigned long* )b2;
                    b2 += s;
                    --n2;
                }
                while ( tmpl < ( unsigned long* )tmp )
                    *tmpl++ = *bl++;
            }
            break;
        case 3:
            while ( n1 > 0 && n2 > 0 ) {
                if ( ( *cmp )( *( const void** )b1, *( const void** )b2, arg ) <= 0 ) {
                    *( void** )tmp = *( void** )b1;
                    b1 += sizeof( void* );
                    --n1;
                }
                else {
                    *( void** )tmp = *( void** )b2;
                    b2 += sizeof( void* );
                    --n2;
                }
                tmp += sizeof( void* );
            }
            break;
        default:
            while ( n1 > 0 && n2 > 0 ) {
                if ( ( *cmp )( b1, b2, arg ) <= 0 ) {
                    tmp = ( char* )mempcpy( tmp, b1, s );
                    b1 += s;
                    --n1;
                }
                else {
                    tmp = ( char* )mempcpy( tmp, b2, s );
                    b2 += s;
                    --n2;
                }
            }
            break;
    }

    if ( n1 > 0 )
        memcpy( tmp, b1, n1 * s );
    memcpy( b, p->t, ( n - n2 ) * s );
}

void __qsort_r( void* b, size_t n, size_t s, __compar_d_fn_t cmp, void* arg ) {
    size_t size = n * s;
    char* tmp = NULL;
    struct msort_param p;

    /* For large object sizes use indirect sorting.  */
    if ( s > 32 )
        size = 2 * n * sizeof( void* ) + s;

    if ( size < 1024 )
        /* The temporary array is small, so put it on the stack.  */
        p.t = ( char* )alloca( size );
    else {
        p.t = tmp = ( char* )malloc( size );
    }

    p.s = s;
    p.var = 4;
    p.cmp = cmp;
    p.arg = arg;

    if ( s > 32 ) {
        /* Indirect sorting.  */
        char* ip = ( char* )b;
        void** tp = ( void** )( p.t + n * sizeof( void* ) );
        void** t = tp;
        void* tmp_storage = ( void* )( tp + n );

        while ( ( void* )t < tmp_storage ) {
            *t++ = ip;
            ip += s;
        }
        p.s = sizeof( void* );
        p.var = 3;
        msort_with_tmp( &p, p.t + n * sizeof( void* ), n );

        /* tp[0] .. tp[n - 1] is now sorted, copy around entries of
         the original array.  Knuth vol. 3 (2nd ed.) exercise 5.2-10.  */
        char* kp;
        size_t i;
        for ( i = 0, ip = ( char* )b; i < n; i++, ip += s )
            if ( ( kp = ( char* )tp[ i ] ) != ip ) {
                size_t j = i;
                char* jp = ip;
                memcpy( tmp_storage, ip, s );

                do {
                    size_t k = ( kp - ( char* )b ) / s;
                    tp[ j ] = jp;
                    memcpy( jp, kp, s );
                    j = k;
                    jp = kp;
                    kp = ( char* )tp[ k ];
                } while ( kp != ip );

                tp[ j ] = jp;
                memcpy( jp, tmp_storage, s );
            }
    }
    else {
        if ( ( s & ( sizeof( uint32_t ) - 1 ) ) == 0 && ( ( char* )b - ( char* )0 ) % alignof( uint32_t ) == 0 ) {
            if ( s == sizeof( uint32_t ) )
                p.var = 0;
            else if ( s == sizeof( uint64_t ) && ( ( char* )b - ( char* )0 ) % alignof( uint64_t ) == 0 )
                p.var = 1;
            else if ( ( s & ( sizeof( unsigned long ) - 1 ) ) == 0
                      && ( ( char* )b - ( char* )0 ) % alignof( unsigned long ) == 0 )
                p.var = 2;
        }
        msort_with_tmp( &p, b, n );
    }
    if ( tmp )
        free( tmp );
}


void qsort( void* b, size_t n, size_t s, __compar_fn_t cmp ) {
    return __qsort_r( b, n, s, ( __compar_d_fn_t )cmp, NULL );
}

uintptr_t g_ucrtbase = pe::get_image< uintptr_t >( HASH( "ucrtbase" ) );

float ln( float x ) {
    unsigned int bx = *( unsigned int* )( &x );
    unsigned int ex = bx >> 23;
    signed int t = ( signed int )ex - ( signed int )127;
    unsigned int s = ( t < 0 ) ? ( -t ) : t;
    bx = 1065353216 | ( bx & 8388607 );
    x = *( float* )( &bx );
    return -1.49278 + ( 2.11263 + ( -0.729104 + 0.10969 * x ) * x ) * x + 0.6931471806 * t;
}

// ============================================
// Exact same function, with added comments:

float natural_log( float x ) {


    unsigned int bx = *( unsigned int* )( &x );

    // extract exp, since x>0, sign bit must be 0
    unsigned int ex = bx >> 23;
    signed int t = ( signed int )ex - ( signed int )127;
    unsigned int s = ( t < 0 ) ? ( -t ) : t;

    // reinterpret back to float
    //   127 << 23 = 1065353216
    //   0b11111111111111111111111 = 8388607
    bx = 1065353216 | ( bx & 8388607 );
    x = *( float* )( &bx );


    // use remez algorithm to find approximation between [1,2]
    // - see this answer https://stackoverflow.com/a/44232045
    // - or this usage of C++/boost's remez implementation
    //   https://computingandrecording.wordpress.com/2017/04/24/
    // e.g.
    // boost::tools::remez_minimax<double> approx(
    //    [](const double& x) { return log(x); },
    // 4, 0, 1, 2, false, 0, 0, 64);
    //
    // 4th order is:
    // { -1.74178, 2.82117, -1.46994, 0.447178, -0.0565717 }
    //
    // 3rd order is:
    // { -1.49278, 2.11263, -0.729104, 0.10969 }

    return

        /* less accurate */
        -1.49278
        + ( 2.11263 + ( -0.729104 + 0.10969 * x ) * x ) * x

        /* OR more accurate */
        // -1.7417939+(2.8212026+(-1.4699568+(0.44717955-0.056570851*x)*x)*x)*x

        /* compensate for the ln(2)s. ln(2)=0.6931471806 */
        +0.6931471806 * t;
}

#pragma function( log )
double log( double x ) {
    return static_cast< double >( natural_log( static_cast< double >( x ) ) );
}

#pragma function( logf )
float logf( float x ) {
    return natural_log( x );
}

#pragma function( memset )
void* memset( void* destination, int value, std::size_t size ) {
    const auto dst = static_cast< std::uint8_t* >( destination );
    const auto val = static_cast< std::uint8_t >( value );

    __stosb( dst, val, size );
    return destination;
}

#pragma function( memcpy )
void* memcpy( void* destination, const void* source, std::size_t size ) {
    const auto dst = static_cast< std::uint8_t* >( destination );
    const auto src = static_cast< const std::uint8_t* >( source );

    __movsb( dst, src, size );
    return destination;
}

#pragma function( memmove )
void* memmove( void* destination, const void* source, std::size_t size ) {
    const auto dst = static_cast< std::uint8_t* >( destination );
    const auto src = static_cast< const std::uint8_t* >( source );

    __movsb( dst, src, size );
    return destination;
}

#pragma function( memcmp )
int memcmp( const void* rcx, const void* rdx, std::size_t size ) {
    if ( size > 0 ) {
        const unsigned char* first = reinterpret_cast< const unsigned char* >( rcx );
        const unsigned char* second = reinterpret_cast< const unsigned char* >( rdx );

        if ( first && second ) {
            do {
                if ( *first++ != *second++ )
                    return ( *--first - *--second );
            } while ( --size != 0 );
        }
    }

    return 0;
}

#pragma function( memchr )
const void* memchr( const void* rcx, int character, std::size_t size ) {
    if ( size > 0 ) {
        const char* data = reinterpret_cast< const char* >( rcx );

        if ( data ) {
            do {
                if ( *data++ == character )
                    return ( void* )( data - 1 );
            } while ( --size != 0 );
        }
    }

    return 0;
}

///////////////////////////////////////////////////////////////////////////////
// \author (c) Marco Paland (info@paland.com)
//             2014-2019, PALANDesign Hannover, Germany
//
// \license The MIT License (MIT)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// \brief Tiny printf, sprintf and (v)snprintf implementation, optimized for speed on
//        embedded systems with a very limited resources. These routines are thread
//        safe and reentrant!
//        Use this instead of the bloated standard/newlib printf cause these use
//        malloc for printf (and may not be thread safe).
//
///////////////////////////////////////////////////////////////////////////////

#define XOR64( value ) ( std::uint64_t )( value )
#define XOR32( value ) ( std::uint32_t )( value )
#define XOR16( value ) ( std::uint16_t )( value )
#define XOR8( value )  ( std::uint8_t )( value )

#pragma warning( push )
#pragma warning( disable : 4267 4244 )

// 'ntoa' conversion buffer size, this must be big enough to hold one converted
// numeric number including padded zeros (dynamically created on stack)
// default: 32 byte
#ifndef PRINTF_NTOA_BUFFER_SIZE
#define PRINTF_NTOA_BUFFER_SIZE ( 32U )
#endif

// 'ftoa' conversion buffer size, this must be big enough to hold one converted
// float number including padded zeros (dynamically created on stack)
// default: 32 byte
#ifndef PRINTF_FTOA_BUFFER_SIZE
#define PRINTF_FTOA_BUFFER_SIZE ( 32U )
#endif

// support for the floating point type (%f)
// default: activated
#ifndef PRINTF_DISABLE_SUPPORT_FLOAT
#define PRINTF_SUPPORT_FLOAT
#endif

// support for exponential floating point notation (%e/%g)
// default: activated
#ifndef PRINTF_DISABLE_SUPPORT_EXPONENTIAL
#define PRINTF_SUPPORT_EXPONENTIAL
#endif

// define the default floating point precision
// default: 6 digits
#ifndef PRINTF_DEFAULT_FLOAT_PRECISION
#define PRINTF_DEFAULT_FLOAT_PRECISION XOR32( 6U )
#endif

// define the largest float suitable to print with %f
// default: 1e9
#ifndef PRINTF_MAX_FLOAT
#define PRINTF_MAX_FLOAT 1e9
#endif

// support for the long long types (%llu or %p)
// default: activated
#ifndef PRINTF_DISABLE_SUPPORT_LONG_LONG
#define PRINTF_SUPPORT_LONG_LONG
#endif

// support for the ptrdiff_t type (%t)
// ptrdiff_t is normally defined in <stddef.h> as long or long long type
// default: activated
#ifndef PRINTF_DISABLE_SUPPORT_PTRDIFF_T
#define PRINTF_SUPPORT_PTRDIFF_T
#endif

///////////////////////////////////////////////////////////////////////////////

// internal flag definitions
#define FLAGS_ZEROPAD   XOR32( 1U << 0U )
#define FLAGS_LEFT      XOR32( 1U << 1U )
#define FLAGS_PLUS      XOR32( 1U << 2U )
#define FLAGS_SPACE     XOR32( 1U << 3U )
#define FLAGS_HASH      XOR32( 1U << 4U )
#define FLAGS_UPPERCASE XOR32( 1U << 5U )
#define FLAGS_CHAR      XOR32( 1U << 6U )
#define FLAGS_SHORT     XOR32( 1U << 7U )
#define FLAGS_LONG      XOR32( 1U << 8U )
#define FLAGS_LONG_LONG XOR32( 1U << 9U )
#define FLAGS_PRECISION XOR32( 1U << 10U )
#define FLAGS_ADAPT_EXP XOR32( 1U << 11U )


// import float.h for DBL_MAX
#if defined( PRINTF_SUPPORT_FLOAT )
#include <float.h>
#endif


// output function type


// wrapper (used as buffer) for output function type
typedef struct {
    void ( *fct )( char character, void* arg );
    void* arg;
} out_fct_wrap_type;


// internal buffer output
static inline void _out_buffer( char character, void* buffer, size_t idx, size_t maxlen ) {
    if ( idx < maxlen ) {
        ( ( char* )buffer )[ idx ] = character;
    }
}

// internal buffer output
static inline void _out_wbuffer( wchar_t character, void* buffer, size_t idx, size_t maxlen ) {
    if ( idx < maxlen ) {
        ( ( wchar_t* )buffer )[ idx ] = character;
    }
}

// internal null output
static inline void _out_null( char character, void* buffer, size_t idx, size_t maxlen ) {
    ( void )character;
    ( void )buffer;
    ( void )idx;
    ( void )maxlen;
}


// internal _putchar wrapper
static inline void _out_char( char character, void* buffer, size_t idx, size_t maxlen ) {
    ( void )buffer;
    ( void )idx;
    ( void )maxlen;
    if ( character ) {
        putchar( character );
    }
}


// internal output function wrapper
template < class T >
static inline void _out_fct( T character, void* buffer, size_t idx, size_t maxlen ) {
    ( void )idx;
    ( void )maxlen;
    if ( character ) {
        // buffer is the output fct pointer
        auto rct = reinterpret_cast< void ( * )( T character, void* arg ) >( ( ( out_fct_wrap_type* )buffer )->fct );
        rct( character, ( ( out_fct_wrap_type* )buffer )->arg );
    }
}


// internal secure strlen
// \return The length of the string (excluding the terminating 0) limited by 'maxsize'
template < class T >
static inline unsigned int _strnlen_s( const T* str, size_t maxsize ) {
    const T* s;
    for ( s = str; *s && maxsize--; ++s )
        ;
    return ( unsigned int )( s - str );
}


// internal test if char is a digit (0-9)
// \return true if char is a digit
template < class T >
static inline bool _is_digit( T ch ) {
    return ( ch >= XOR8( '0' ) ) && ( ch <= XOR8( '9' ) );
}


// internal ASCII string to unsigned int conversion
template < class T >
static unsigned int _atoi( const T** str ) {
    unsigned int i = 0U;
    while ( _is_digit( **str ) ) {
        i = i * XOR32( 10U ) + ( unsigned int )( *( ( *str )++ ) - XOR8( '0' ) );
    }
    return i;
}


// output the specified string in reverse, taking care of any zero-padding
template < class T >

static size_t _out_rev( void ( *out )( T character, void* buffer, size_t idx, size_t maxlen ), T* buffer, size_t idx,
                        size_t maxlen, const char* buf, size_t len, unsigned int width, unsigned int flags ) {
    const size_t start_idx = idx;

    // pad spaces up to given width
    if ( !( flags & FLAGS_LEFT ) && !( flags & FLAGS_ZEROPAD ) ) {
        for ( size_t i = len; i < width; i++ ) {
            out( ' ', buffer, idx++, maxlen );
        }
    }

    // reverse string
    while ( len ) {
        out( buf[ --len ], buffer, idx++, maxlen );
    }

    // append pad spaces up to given width
    if ( flags & FLAGS_LEFT ) {
        while ( idx - start_idx < width ) {
            out( ' ', buffer, idx++, maxlen );
        }
    }

    return idx;
}


// internal itoa format
template < class T >

static size_t _ntoa_format( void ( *out )( T character, void* buffer, size_t idx, size_t maxlen ), T* buffer,
                            size_t idx, size_t maxlen, char* buf, size_t len, bool negative, unsigned int base, unsigned int prec,
                            unsigned int width, unsigned int flags ) {
    // pad leading zeros
    if ( !( flags & FLAGS_LEFT ) ) {
        if ( width && ( flags & FLAGS_ZEROPAD ) && ( negative || ( flags & ( FLAGS_PLUS | FLAGS_SPACE ) ) ) ) {
            width--;
        }
        while ( ( len < prec ) && ( len < PRINTF_NTOA_BUFFER_SIZE ) ) {
            buf[ len++ ] = '0';
        }
        while ( ( flags & FLAGS_ZEROPAD ) && ( len < width ) && ( len < PRINTF_NTOA_BUFFER_SIZE ) ) {
            buf[ len++ ] = '0';
        }
    }

    // handle hash
    if ( flags & FLAGS_HASH ) {
        if ( !( flags & FLAGS_PRECISION ) && len && ( ( len == prec ) || ( len == width ) ) ) {
            len--;
            if ( len && ( base == XOR32( 16U ) ) ) {
                len--;
            }
        }
        if ( ( base == XOR32( 16U ) ) && !( flags & FLAGS_UPPERCASE ) && ( len < PRINTF_NTOA_BUFFER_SIZE ) ) {
            buf[ len++ ] = XOR8( 'x' );
        }
        else if ( ( base == XOR32( 16U ) ) && ( flags & FLAGS_UPPERCASE ) && ( len < PRINTF_NTOA_BUFFER_SIZE ) ) {
            buf[ len++ ] = XOR8( 'X' );
        }
        else if ( ( base == XOR32( 2U ) ) && ( len < PRINTF_NTOA_BUFFER_SIZE ) ) {
            buf[ len++ ] = XOR8( 'b' );
        }
        if ( len < PRINTF_NTOA_BUFFER_SIZE ) {
            buf[ len++ ] = XOR8( '0' );
        }
    }

    if ( len < PRINTF_NTOA_BUFFER_SIZE ) {
        if ( negative ) {
            buf[ len++ ] = XOR8( '-' );
        }
        else if ( flags & FLAGS_PLUS ) {
            buf[ len++ ] = XOR8( '+' ); // ignore the space if the '+' exists
        }
        else if ( flags & FLAGS_SPACE ) {
            buf[ len++ ] = XOR8( ' ' );
        }
    }

    return _out_rev( out, buffer, idx, maxlen, buf, len, width, flags );
}


// internal itoa for 'long' type
template < class T >

static size_t _ntoa_long( void ( *out )( T character, void* buffer, size_t idx, size_t maxlen ), T* buffer, size_t idx,
                          size_t maxlen, unsigned long value, bool negative, unsigned long base, unsigned int prec, unsigned int width,
                          unsigned int flags ) {
    char buf[ PRINTF_NTOA_BUFFER_SIZE ];
    size_t len = 0U;

    // no hash for 0 values
    if ( !value ) {
        flags &= ~FLAGS_HASH;
    }

    // write if precision != 0 and value is != 0
    if ( !( flags & FLAGS_PRECISION ) || value ) {
        do {
            const char digit = ( char )( value % base );
            buf[ len++ ] = digit < 10 ? '0' + digit : ( flags & FLAGS_UPPERCASE ? 'A' : 'a' ) + digit - 10;
            value /= base;
        } while ( value && ( len < PRINTF_NTOA_BUFFER_SIZE ) );
    }

    return _ntoa_format( out, buffer, idx, maxlen, buf, len, negative, ( unsigned int )base, prec, width, flags );
}


// internal itoa for 'long long' type
#if defined( PRINTF_SUPPORT_LONG_LONG )
template < class T >

static size_t _ntoa_long_long( void ( *out )( T character, void* buffer, size_t idx, size_t maxlen ), T* buffer,
                               size_t idx, size_t maxlen, unsigned long long value, bool negative, unsigned long long base, unsigned int prec,
                               unsigned int width, unsigned int flags ) {
    char buf[ PRINTF_NTOA_BUFFER_SIZE ];
    size_t len = 0U;

    // no hash for 0 values
    if ( !value ) {
        flags &= ~FLAGS_HASH;
    }

    // write if precision != 0 and value is != 0
    if ( !( flags & FLAGS_PRECISION ) || value ) {
        do {
            const char digit = ( char )( value % base );
            buf[ len++ ] = digit < 10 ? '0' + digit : ( flags & FLAGS_UPPERCASE ? 'A' : 'a' ) + digit - 10;
            value /= base;
        } while ( value && ( len < PRINTF_NTOA_BUFFER_SIZE ) );
    }

    return _ntoa_format( out, buffer, idx, maxlen, buf, len, negative, ( unsigned int )base, prec, width, flags );
}
#endif // PRINTF_SUPPORT_LONG_LONG


#if defined( PRINTF_SUPPORT_FLOAT )

#if defined( PRINTF_SUPPORT_EXPONENTIAL )
// forward declaration so that _ftoa can switch to exp notation for values > PRINTF_MAX_FLOAT
template < class T >
static size_t _etoa( void ( *out )( T character, void* buffer, size_t idx, size_t maxlen ), T* buffer, size_t idx,
                     size_t maxlen, double value, unsigned int prec, unsigned int width, unsigned int flags );
#endif

template < class T >

// internal ftoa for fixed decimal floating point
static size_t _ftoa( void ( *out )( T character, void* buffer, size_t idx, size_t maxlen ), T* buffer, size_t idx,
                     size_t maxlen, double value, unsigned int prec, unsigned int width, unsigned int flags ) {
    char buf[ PRINTF_FTOA_BUFFER_SIZE ];
    size_t len = 0U;
    double diff = 0.0;

    // powers of 10
    static const double pow10[ ] = { 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000 };

    // test for special values
    if ( value != value )
        return _out_rev( out, buffer, idx, maxlen, _( "nan" ), 3, width, flags );
    if ( value < -DBL_MAX )
        return _out_rev( out, buffer, idx, maxlen, _( "fni-" ), 4, width, flags );
    if ( value > DBL_MAX )
        return _out_rev( out, buffer, idx, maxlen, ( flags & FLAGS_PLUS ) ? _( "fni+" ) : _( "fni" ),
                         ( flags & FLAGS_PLUS ) ? 4U : 3U, width, flags );

    // test for very large values
    // standard printf behavior is to print EVERY whole number digit -- which could be 100s of characters overflowing your
    // buffers == bad
    if ( ( value > PRINTF_MAX_FLOAT ) || ( value < -PRINTF_MAX_FLOAT ) ) {
    #if defined( PRINTF_SUPPORT_EXPONENTIAL )
        return _etoa( out, buffer, idx, maxlen, value, prec, width, flags );
    #else
        return 0U;
    #endif
    }

    // test for negative
    bool negative = false;
    if ( value < 0 ) {
        negative = true;
        value = 0 - value;
    }

    // set default precision, if not set explicitly
    if ( !( flags & FLAGS_PRECISION ) ) {
        prec = PRINTF_DEFAULT_FLOAT_PRECISION;
    }
    // limit precision to 9, cause a prec >= 10 can lead to overflow errors
    while ( ( len < PRINTF_FTOA_BUFFER_SIZE ) && ( prec > 9U ) ) {
        buf[ len++ ] = '0';
        prec--;
    }

    int whole = ( int )value;
    double tmp = ( value - whole ) * pow10[ prec ];
    unsigned long frac = ( unsigned long )tmp;
    diff = tmp - frac;

    if ( diff > 0.5 ) {
        ++frac;
        // handle rollover, e.g. case 0.99 with prec 1 is 1.0
        if ( frac >= pow10[ prec ] ) {
            frac = 0;
            ++whole;
        }
    }
    else if ( diff < 0.5 ) {
    }
    else if ( ( frac == 0U ) || ( frac & 1U ) ) {
        // if halfway, round up if odd OR if last digit is 0
        ++frac;
    }

    if ( prec == 0U ) {
        diff = value - ( double )whole;
        if ( ( !( diff < 0.5 ) || ( diff > 0.5 ) ) && ( whole & 1 ) ) {
            // exactly 0.5 and ODD, then round up
            // 1.5 -> 2, but 2.5 -> 2
            ++whole;
        }
    }
    else {
        unsigned int count = prec;
        // now do fractional part, as an unsigned number
        while ( len < PRINTF_FTOA_BUFFER_SIZE ) {
            --count;
            buf[ len++ ] = ( char )( XOR32( 48U ) + ( frac % XOR32( 10U ) ) );
            if ( !( frac /= XOR32( 10U ) ) ) {
                break;
            }
        }
        // add extra 0s
        while ( ( len < PRINTF_FTOA_BUFFER_SIZE ) && ( count-- > XOR32( 0U ) ) ) {
            buf[ len++ ] = XOR8( '0' );
        }
        if ( len < PRINTF_FTOA_BUFFER_SIZE ) {
            // add decimal
            buf[ len++ ] = '.';
        }
    }

    // do whole part, number is reversed
    while ( len < PRINTF_FTOA_BUFFER_SIZE ) {
        buf[ len++ ] = ( char )( 48 + ( whole % 10 ) );
        if ( !( whole /= XOR32( 10 ) ) ) {
            break;
        }
    }

    // pad leading zeros
    if ( !( flags & FLAGS_LEFT ) && ( flags & FLAGS_ZEROPAD ) ) {
        if ( width && ( negative || ( flags & ( FLAGS_PLUS | FLAGS_SPACE ) ) ) ) {
            width--;
        }
        while ( ( len < width ) && ( len < PRINTF_FTOA_BUFFER_SIZE ) ) {
            buf[ len++ ] = XOR8( '0' );
        }
    }

    if ( len < PRINTF_FTOA_BUFFER_SIZE ) {
        if ( negative ) {
            buf[ len++ ] = XOR8( '-' );
        }
        else if ( flags & FLAGS_PLUS ) {
            buf[ len++ ] = XOR8( '+' ); // ignore the space if the '+' exists
        }
        else if ( flags & FLAGS_SPACE ) {
            buf[ len++ ] = XOR8( ' ' );
        }
    }

    return _out_rev( out, buffer, idx, maxlen, buf, len, width, flags );
}


#if defined( PRINTF_SUPPORT_EXPONENTIAL )
// internal ftoa variant for exponential floating-point type, contributed by Martijn Jasperse <m.jasperse@gmail.com>
template < class T >
static size_t _etoa( void ( *out )( T character, void* buffer, size_t idx, size_t maxlen ), T* buffer, size_t idx,
                     size_t maxlen, double value, unsigned int prec, unsigned int width, unsigned int flags ) {
    // check for NaN and special values
    if ( ( value != value ) || ( value > DBL_MAX ) || ( value < -DBL_MAX ) ) {
        return _ftoa( out, buffer, idx, maxlen, value, prec, width, flags );
    }

    // determine the sign
    const bool negative = value < 0;
    if ( negative ) {
        value = -value;
    }

    // default precision
    if ( !( flags & FLAGS_PRECISION ) ) {
        prec = PRINTF_DEFAULT_FLOAT_PRECISION;
    }

    // determine the decimal exponent
    // based on the algorithm by David Gay (https://www.ampl.com/netlib/fp/dtoa.c)
    union {
        uint64_t U;
        double F;
    } conv;

    conv.F = value;
    int exp2 = ( int )( ( conv.U >> 52U ) & 0x07FFU ) - 1023;              // effectively log2
    conv.U = ( conv.U & ( ( 1ULL << 52U ) - 1U ) ) | ( 1023ULL << 52U ); // drop the exponent so conv.F is now in [1,2)
    // now approximate log10 from the log2 integer part and an expansion of ln around 1.5
    int expval = ( int )( 0.1760912590558 + exp2 * 0.301029995663981 + ( conv.F - 1.5 ) * 0.289529654602168 );
    // now we want to compute 10^expval but we want to be sure it won't overflow
    exp2 = ( int )( expval * 3.321928094887362 + 0.5 );
    const double z = expval * 2.302585092994046 - exp2 * 0.6931471805599453;
    const double z2 = z * z;
    conv.U = ( uint64_t )( exp2 + 1023 ) << 52U;
    // compute exp(z) using continued fractions, see
    // https://en.wikipedia.org/wiki/Exponential_function#Continued_fractions_for_ex
    conv.F *= 1 + 2 * z / ( 2 - z + ( z2 / ( 6 + ( z2 / ( 10 + z2 / 14 ) ) ) ) );
    // correct for rounding errors
    if ( value < conv.F ) {
        expval--;
        conv.F /= 10;
    }

    // the exponent format is "%+03d" and largest value is "307", so set aside 4-5 characters
    unsigned int minwidth = ( ( expval < 100 ) && ( expval > -100 ) ) ? 4U : 5U;

    // in "%g" mode, "prec" is the number of *significant figures* not decimals
    if ( flags & FLAGS_ADAPT_EXP ) {
        // do we want to fall-back to "%f" mode?
        if ( ( value >= 1e-4 ) && ( value < 1e6 ) ) {
            if ( ( int )prec > expval ) {
                prec = ( unsigned )( ( int )prec - expval - 1 );
            }
            else {
                prec = 0;
            }
            flags |= FLAGS_PRECISION; // make sure _ftoa respects precision
            // no characters in exponent
            minwidth = 0U;
            expval = 0;
        }
        else {
            // we use one sigfig for the whole part
            if ( ( prec > 0 ) && ( flags & FLAGS_PRECISION ) ) {
                --prec;
            }
        }
    }

    // will everything fit?
    unsigned int fwidth = width;
    if ( width > minwidth ) {
        // we didn't fall-back so subtract the characters required for the exponent
        fwidth -= minwidth;
    }
    else {
        // not enough characters, so go back to default sizing
        fwidth = 0U;
    }
    if ( ( flags & FLAGS_LEFT ) && minwidth ) {
        // if we're padding on the right, DON'T pad the floating part
        fwidth = 0U;
    }

    // rescale the float value
    if ( expval ) {
        value /= conv.F;
    }

    // output the floating part
    const size_t start_idx = idx;
    idx = _ftoa( out, buffer, idx, maxlen, negative ? -value : value, prec, fwidth, flags & ~FLAGS_ADAPT_EXP );

    // output the exponent part
    if ( minwidth ) {
        // output the exponential symbol
        out( ( flags & FLAGS_UPPERCASE ) ? 'E' : 'e', buffer, idx++, maxlen );
        // output the exponent value
        idx = _ntoa_long( out, buffer, idx, maxlen, ( expval < 0 ) ? -expval : expval, expval < 0, 10, 0, minwidth - 1,
                          FLAGS_ZEROPAD | FLAGS_PLUS );
        // might need to right-pad spaces
        if ( flags & FLAGS_LEFT ) {
            while ( idx - start_idx < width )
                out( ' ', buffer, idx++, maxlen );
        }
    }
    return idx;
}
#endif // PRINTF_SUPPORT_EXPONENTIAL
#endif // PRINTF_SUPPORT_FLOAT

// typedef void ( *out_fct_type )( char character, void* buffer, size_t idx, size_t maxlen );

// internal vsnprintf
template < class T >
static int _vsnprintf( void ( *out )( T character, void* buffer, size_t idx, size_t maxlen ), T* buffer,
                       const size_t maxlen, const T* format, va_list va ) {
    unsigned int flags, width, precision, n;
    size_t idx = 0U;

    if ( !buffer ) {
        // use null output function
        out = ( decltype( out ) )_out_null;
    }

    constexpr bool bWide = std::is_same< wchar_t, T >::value;

    while ( *format ) {
        // format specifier?  %[flags][width][.precision][length]
        if ( *format != '%' ) {
            // no
            out( *format, buffer, idx++, maxlen );
            format++;
            continue;
        }
        else {
            // yes, evaluate it
            format++;
        }

        // evaluate flags
        flags = 0U;
        do {
            switch ( *format ) {
                case '0':
                    flags |= FLAGS_ZEROPAD;
                    format++;
                    n = 1U;
                    break;
                case '-':
                    flags |= FLAGS_LEFT;
                    format++;
                    n = 1U;
                    break;
                case '+':
                    flags |= FLAGS_PLUS;
                    format++;
                    n = 1U;
                    break;
                case ' ':
                    flags |= FLAGS_SPACE;
                    format++;
                    n = 1U;
                    break;
                case '#':
                    flags |= FLAGS_HASH;
                    format++;
                    n = 1U;
                    break;
                default: n = 0U; break;
            }
        } while ( n );

        // evaluate width field
        width = 0U;
        if ( _is_digit( *format ) ) {
            width = _atoi( &format );
        }
        else if ( *format == '*' ) {
            const int w = va_arg( va, int );
            if ( w < 0 ) {
                flags |= FLAGS_LEFT; // reverse padding
                width = ( unsigned int )-w;
            }
            else {
                width = ( unsigned int )w;
            }
            format++;
        }

        // evaluate precision field
        precision = 0U;
        if ( *format == '.' ) {
            flags |= FLAGS_PRECISION;
            format++;
            if ( _is_digit( *format ) ) {
                precision = _atoi( &format );
            }
            else if ( *format == '*' ) {
                const int prec = ( int )va_arg( va, int );
                precision = prec > 0 ? ( unsigned int )prec : 0U;
                format++;
            }
        }

        // evaluate length field
        switch ( *format ) {
            case ( 'l' ):
                flags |= FLAGS_LONG;
                format++;
                if ( *format == XOR8( 'l' ) ) {
                    flags |= FLAGS_LONG_LONG;
                    format++;
                }
                break;
            case ( 'h' ):
                flags |= FLAGS_SHORT;
                format++;
                if ( *format == XOR8( 'h' ) ) {
                    flags |= FLAGS_CHAR;
                    format++;
                }
                break;
            #if defined( PRINTF_SUPPORT_PTRDIFF_T )
            case ( 't' ):
                flags |= ( sizeof( ptrdiff_t ) == sizeof( long ) ? FLAGS_LONG : FLAGS_LONG_LONG );
                format++;
                break;
            #endif
            case ( 'j' ):
                flags |= ( sizeof( intmax_t ) == sizeof( long ) ? FLAGS_LONG : FLAGS_LONG_LONG );
                format++;
                break;
            case ( 'z' ):
                flags |= ( sizeof( size_t ) == sizeof( long ) ? FLAGS_LONG : FLAGS_LONG_LONG );
                format++;
                break;
            default: break;
        }

        // evaluate specifier
        switch ( *format ) {
            case 'd':
            case 'i':
            case 'u':
            case 'x':
            case 'X':
            case 'o':
            case 'b': {
                // set the base
                unsigned int base;
                if ( *format == 'x' || *format == 'X' ) {
                    base = 16U;
                }
                else if ( *format == 'o' ) {
                    base = 8U;
                }
                else if ( *format == 'b' ) {
                    base = 2U;
                }
                else {
                    base = 10U;
                    flags &= ~FLAGS_HASH; // no hash for dec format
                }
                // uppercase
                if ( *format == 'X' ) {
                    flags |= FLAGS_UPPERCASE;
                }

                // no plus or space flag for u, x, X, o, b
                if ( ( *format != 'i' ) && ( *format != 'd' ) ) {
                    flags &= ~( FLAGS_PLUS | FLAGS_SPACE );
                }

                // ignore '0' flag when precision is given
                if ( flags & FLAGS_PRECISION ) {
                    flags &= ~FLAGS_ZEROPAD;
                }

                // convert the integer
                if ( ( *format == 'i' ) || ( *format == 'd' ) ) {
                    // signed
                    if ( flags & FLAGS_LONG_LONG ) {
                    #if defined( PRINTF_SUPPORT_LONG_LONG )
                        const long long value = va_arg( va, long long );
                        idx = _ntoa_long_long( out, buffer, idx, maxlen, ( unsigned long long )( value > 0 ? value : 0 - value ),
                                               value < 0, base, precision, width, flags );
                    #endif
                    }
                    else if ( flags & FLAGS_LONG ) {
                        const long value = va_arg( va, long );
                        idx = _ntoa_long( out, buffer, idx, maxlen, ( unsigned long )( value > 0 ? value : 0 - value ), value < 0,
                                          base, precision, width, flags );
                    }
                    else {
                        const int value = ( flags & FLAGS_CHAR ) ? ( char )va_arg( va, int )
                            : ( flags & FLAGS_SHORT ) ? ( short int )va_arg( va, int )
                            : va_arg( va, int );
                        idx = _ntoa_long( out, buffer, idx, maxlen, ( unsigned int )( value > 0 ? value : 0 - value ), value < 0,
                                          base, precision, width, flags );
                    }
                }
                else {
                    // unsigned
                    if ( flags & FLAGS_LONG_LONG ) {
                    #if defined( PRINTF_SUPPORT_LONG_LONG )
                        idx = _ntoa_long_long(
                            out, buffer, idx, maxlen, va_arg( va, unsigned long long ), false, base, precision, width, flags );
                    #endif
                    }
                    else if ( flags & FLAGS_LONG ) {
                        idx = _ntoa_long(
                            out, buffer, idx, maxlen, va_arg( va, unsigned long ), false, base, precision, width, flags );
                    }
                    else {
                        const unsigned int value = ( flags & FLAGS_CHAR ) ? ( unsigned char )va_arg( va, unsigned int )
                            : ( flags & FLAGS_SHORT ) ? ( unsigned short int )va_arg( va, unsigned int )
                            : va_arg( va, unsigned int );
                        idx = _ntoa_long( out, buffer, idx, maxlen, value, false, base, precision, width, flags );
                    }
                }
                format++;
                break;
            }
                #if defined( PRINTF_SUPPORT_FLOAT )
            case 'f':
            case 'F':
                if ( *format == 'F' )
                    flags |= FLAGS_UPPERCASE;
                idx = _ftoa( out, buffer, idx, maxlen, va_arg( va, double ), precision, width, flags );
                format++;
                break;
            #if defined( PRINTF_SUPPORT_EXPONENTIAL )
            case 'e':
            case 'E':
            case 'g':
            case 'G':
                if ( ( *format == 'g' ) || ( *format == 'G' ) )
                    flags |= FLAGS_ADAPT_EXP;
                if ( ( *format == 'E' ) || ( *format == 'G' ) )
                    flags |= FLAGS_UPPERCASE;
                idx = _etoa( out, buffer, idx, maxlen, va_arg( va, double ), precision, width, flags );
                format++;
                break;
            #endif // PRINTF_SUPPORT_EXPONENTIAL
            #endif // PRINTF_SUPPORT_FLOAT
            case 'c': {
                unsigned int l = 1U;
                // pre padding
                if ( !( flags & FLAGS_LEFT ) ) {
                    while ( l++ < width ) {
                        out( ' ', buffer, idx++, maxlen );
                    }
                }
                // char output
                out( ( char )va_arg( va, int ), buffer, idx++, maxlen );
                // post padding
                if ( flags & FLAGS_LEFT ) {
                    while ( l++ < width ) {
                        out( ' ', buffer, idx++, maxlen );
                    }
                }
                format++;
                break;
            }

            case 's':
            case 'S': {
                const char* p = va_arg( va, char* );
                bool unicode = ( *format == 'S' && !bWide ) || ( *format == 's' && bWide );

                unsigned int l = unicode ? wcslen( ( wchar_t* )p ) : _strnlen_s( p, precision ? precision : ( size_t )-1 );
                // pre padding
                if ( flags & FLAGS_PRECISION ) {
                    l = ( l < precision ? l : precision );
                }
                if ( !( flags & FLAGS_LEFT ) ) {
                    while ( l++ < width ) {
                        out( ' ', buffer, idx++, maxlen );
                    }
                }
                // string output
                while ( ( *p != 0 ) && ( !( flags & FLAGS_PRECISION ) || precision-- ) ) {
                    out( unicode ? *( wchar_t* )p : *p, buffer, idx++, maxlen );
                    p += unicode ? 2 : 1;
                }
                // post padding
                if ( flags & FLAGS_LEFT ) {
                    while ( l++ < width ) {
                        out( ' ', buffer, idx++, maxlen );
                    }
                }
                format++;
                break;
            }

            case 'p': {
                width = sizeof( void* ) * 2U;
                flags |= FLAGS_ZEROPAD | FLAGS_UPPERCASE;
            #if defined( PRINTF_SUPPORT_LONG_LONG )
                const bool is_ll = sizeof( uintptr_t ) == sizeof( long long );
                if ( is_ll ) {
                    idx = _ntoa_long_long(
                        out, buffer, idx, maxlen, ( uintptr_t )va_arg( va, void* ), false, XOR64( 16U ), precision, width, flags );
                }
                else {
                #endif
                    idx = _ntoa_long( out, buffer, idx, maxlen, ( unsigned long )( ( uintptr_t )va_arg( va, void* ) ), false,
                                      XOR64( 16U ), precision, width, flags );
                #if defined( PRINTF_SUPPORT_LONG_LONG )
                }
            #endif
                format++;
                break;
            }

            case '%':
                out( '%', buffer, idx++, maxlen );
                format++;
                break;

            default:
                out( *format, buffer, idx++, maxlen );
                format++;
                break;
        }
    }

    // termination
    out( ( char )0, buffer, idx < maxlen ? idx : maxlen - 1U, maxlen );

    // return written chars without terminating \0
    return ( int )idx;
}
template < class T >
int GetFormatPartLength( T* format ) { }

///////////////////////////////////////////////////////////////////////////////

int printf_( const char* format, ... ) {
    va_list va;
    va_start( va, format );
    char buffer[ 1 ];
    const int ret = _vsnprintf( _out_char, buffer, XOR64( -1 ), format, va );
    va_end( va );
    return ret;
}


int sprintf_( char* buffer, const char* format, ... ) {
    va_list va;
    va_start( va, format );
    const int ret = _vsnprintf( _out_buffer, buffer, XOR64( -1 ), format, va );
    va_end( va );
    return ret;
}


int snprintf_( char* buffer, size_t count, const char* format, ... ) {
    va_list va;
    va_start( va, format );
    const int ret = _vsnprintf( _out_buffer, buffer, count, format, va );
    va_end( va );
    return ret;
}


int vprintf_( const char* format, va_list va ) {
    char buffer[ 1 ];
    return _vsnprintf( _out_char, buffer, XOR64( -1 ), format, va );
}


int vsnprintf_( char* buffer, size_t count, const char* format, va_list va ) {
    return _vsnprintf( _out_buffer, buffer, count, format, va );
}

template < class T >
int fctprintf( void ( *out )( T character, void* arg ), void* arg, const T* format, ... ) {
    va_list va;
    va_start( va, format );
    const out_fct_wrap_type out_fct_wrap = { reinterpret_cast< decltype( out_fct_wrap_type::fct ) >( out ), arg };
    const int ret = _vsnprintf( _out_fct, ( T* )( uintptr_t )&out_fct_wrap, XOR64( -1 ), format, va );
    va_end( va );
    return ret;
}

int __cdecl __stdio_common_vsprintf_s( _In_ unsigned __int64 _Options, _Out_writes_z_( _BufferCount ) char* _Buffer,
                                       _In_ size_t _BufferCount, _In_z_ _Printf_format_string_params_( 2 ) char const* _Format, _In_opt_ _locale_t _Locale,
                                       va_list _ArgList ) {
    return vsnprintf_( _Buffer, _BufferCount, _Format, _ArgList );
}
int __cdecl __stdio_common_vsprintf( _In_ unsigned __int64 _Options, _Out_writes_opt_z_( _BufferCount ) char* _Buffer,
                                     _In_ size_t _BufferCount, _In_z_ _Printf_format_string_params_( 2 ) char const* _Format, _In_opt_ _locale_t _Locale,
                                     va_list _ArgList ) {
    return vsnprintf_( _Buffer, _BufferCount, _Format, _ArgList );
}
int __cdecl __stdio_common_vswprintf_s( _In_ unsigned __int64 _Options, _Out_writes_z_( _BufferCount ) wchar_t* _Buffer,
                                        _In_ size_t _BufferCount, _In_z_ _Printf_format_string_params_( 2 ) wchar_t const* _Format,
                                        _In_opt_ _locale_t _Locale, va_list _ArgList ) {
    return _vsnprintf( _out_wbuffer, _Buffer, _BufferCount, _Format, _ArgList );
}
int __cdecl __stdio_common_vswprintf( _In_ unsigned __int64 _Options,
                                      _Out_writes_opt_z_( _BufferCount ) wchar_t* _Buffer, _In_ size_t _BufferCount,
                                      _In_z_ _Printf_format_string_params_( 2 ) wchar_t const* _Format, _In_opt_ _locale_t _Locale, va_list _ArgList ) {
    return _vsnprintf( _out_wbuffer, _Buffer, _BufferCount, _Format, _ArgList );
}
template < class T >
void fprintf_worker( T character, void* __file ) {
    fwrite( &character, sizeof( T ), 1, ( FILE* )__file );
}
int __cdecl __stdio_common_vfprintf( _In_ unsigned __int64 _Options, _Inout_ FILE* _Stream,
                                     _In_z_ _Printf_format_string_params_( 2 ) char const* _Format, _In_opt_ _locale_t _Locale, va_list _ArgList ) {
    out_fct_wrap_type out_fct_wrap
        = { reinterpret_cast< decltype( out_fct_wrap_type::fct ) >( fprintf_worker< char > ), _Stream };
    return _vsnprintf< char >( _out_fct, ( char* )( uintptr_t )&out_fct_wrap, ( size_t )-1, _Format, _ArgList );
}

int __cdecl __stdio_common_vfwprintf( _In_ unsigned __int64 _Options, _Inout_ FILE* _Stream,
                                      _In_z_ _Printf_format_string_params_( 2 ) wchar_t const* _Format, _In_opt_ _locale_t _Locale, va_list _ArgList ) {
    return fctprintf< wchar_t >( [ ]( wchar_t character, void* file ) -> void { fwrite( &character, 2, 1, ( FILE* )file ); },
                                 _Stream, _Format, _ArgList );
}
int __cdecl __stdio_common_vfprintf_s( _In_ unsigned __int64 _Options, _Inout_ FILE* _Stream,
                                       _In_z_ _Printf_format_string_params_( 2 ) char const* _Format, _In_opt_ _locale_t _Locale, va_list _ArgList ) {
    return fctprintf< char >( [ ]( char character, void* file ) -> void { fwrite( &character, 1, 1, ( FILE* )file ); },
                              _Stream, _Format, _ArgList );
}

int __cdecl __stdio_common_vfwprintf_s( _In_ unsigned __int64 _Options, _Inout_ FILE* _Stream,
                                        _In_z_ _Printf_format_string_params_( 2 ) wchar_t const* _Format, _In_opt_ _locale_t _Locale, va_list _ArgList ) {
    return fctprintf< wchar_t >( [ ]( wchar_t character, void* file ) -> void { fwrite( &character, 2, 1, ( FILE* )file ); },
                                 _Stream, _Format, _ArgList );
}

#pragma function( strcat )
char* strcat( char* dest, const char* src ) {
    char* rdest = dest;

    while ( *dest )
        dest++;
    while ( *dest++ = *src++ )
        ;
    return rdest;
}

static char* _getbase( char* p, int* basep ) {
    if ( p[ 0 ] == '0' ) {
        switch ( p[ 1 ] ) {
            case 'x': *basep = 16; break;
            case 't':
            case 'n': *basep = 10; break;
            case 'o': *basep = 8; break;
            default: *basep = 10; return ( p );
        }
        return ( p + 2 );
    }
    *basep = 10;
    return ( p );
}


/*
 *  _atob(vp,p,base)
 */
static int _atob( unsigned long long* vp, char* p, int base ) {
    unsigned long long value, v1, v2;
    char* q, tmp[ 20 ];
    int digit;

    if ( p[ 0 ] == '0' && ( p[ 1 ] == 'x' || p[ 1 ] == 'X' ) ) {
        base = 16;
        p += 2;
    }

    if ( base == 16 && ( q = strchr( p, '.' ) ) != 0 ) {
        if ( q - p > sizeof( tmp ) - 1 )
            return ( 0 );

        strncpy( tmp, p, q - p );
        tmp[ q - p ] = '\0';
        if ( !_atob( &v1, tmp, 16 ) )
            return ( 0 );

        q++;
        if ( strchr( q, '.' ) )
            return ( 0 );

        if ( !_atob( &v2, q, 16 ) )
            return ( 0 );
        *vp = ( v1 << 16 ) + v2;
        return ( 1 );
    }

    value = *vp = 0;
    for ( ; *p; p++ ) {
        if ( *p >= '0' && *p <= '9' )
            digit = *p - '0';
        else if ( *p >= 'a' && *p <= 'f' )
            digit = *p - 'a' + 10;
        else if ( *p >= 'A' && *p <= 'F' )
            digit = *p - 'A' + 10;
        else
            return ( 0 );

        if ( digit >= base )
            return ( 0 );
        value *= base;
        value += digit;
    }
    *vp = value;
    return ( 1 );
}

/*
 *  atob(vp,p,base)
 *      converts p to binary result in vp, rtn 1 on success
 */
int atob( uint32_t* vp, char* p, int base ) {
    DWORD_PTR v;

    if ( base == 0 )
        p = _getbase( p, &base );
    if ( _atob( &v, p, base ) ) {
        *vp = static_cast< uint32_t >( v );
        return ( 1 );
    }
    return ( 0 );
}


/*
 *  llatob(vp,p,base)
 *      converts p to binary result in vp, rtn 1 on success
 */
int llatob( DWORD_PTR* vp, char* p, int base ) {
    if ( base == 0 )
        p = _getbase( p, &base );
    return _atob( vp, p, base );
}

/*
 *  char *btoa(dst,value,base)
 *      converts value to ascii, result in dst
 */
char* btoa( char* dst, uint32_t value, int base ) {
    char buf[ 34 ], digit;
    int i, j, rem, neg;

    if ( value == 0 ) {
        dst[ 0 ] = '0';
        dst[ 1 ] = 0;
        return ( dst );
    }

    neg = 0;
    if ( base == -10 ) {
        base = 10;
        if ( value & ( 1L << 31 ) ) {
            value = ( ~value ) + 1;
            neg = 1;
        }
    }

    for ( i = 0; value != 0; i++ ) {
        rem = value % base;
        value /= base;
        if ( rem >= 0 && rem <= 9 )
            digit = rem + '0';
        else if ( rem >= 10 && rem <= 36 )
            digit = ( rem - 10 ) + 'a';
        buf[ i ] = digit;
    }

    buf[ i ] = 0;
    if ( neg )
        strcat( buf, "-" );

    /* reverse the string */
    for ( i = 0, j = strlen( buf ) - 1; j >= 0; i++, j-- )
        dst[ i ] = buf[ j ];
    dst[ i ] = 0;
    return ( dst );
}

/*
 *  char *btoa(dst,value,base)
 *      converts value to ascii, result in dst
 */
char* llbtoa( char* dst, DWORD_PTR value, int base ) {
    char buf[ 66 ], digit;
    int i, j, rem, neg;

    if ( value == 0 ) {
        dst[ 0 ] = '0';
        dst[ 1 ] = 0;
        return ( dst );
    }

    neg = 0;
    if ( base == -10 ) {
        base = 10;
        if ( value & ( 1LL << 63 ) ) {
            value = ( ~value ) + 1;
            neg = 1;
        }
    }

    for ( i = 0; value != 0; i++ ) {
        rem = value % base;
        value /= base;
        if ( rem >= 0 && rem <= 9 )
            digit = rem + '0';
        else if ( rem >= 10 && rem <= 36 )
            digit = ( rem - 10 ) + 'a';
        buf[ i ] = digit;
    }

    buf[ i ] = 0;
    if ( neg )
        strcat( buf, "-" );

    /* reverse the string */
    for ( i = 0, j = strlen( buf ) - 1; j >= 0; i++, j-- )
        dst[ i ] = buf[ j ];
    dst[ i ] = 0;
    return ( dst );
}

/*
 *  gethex(vp,p,n)
 *      convert n hex digits from p to binary, result in vp,
 *      rtn 1 on success
 */
int gethex( int32_t* vp, char* p, int n ) {
    DWORD v;
    int digit;

    for ( v = 0; n > 0; n-- ) {
        if ( *p == 0 )
            return ( 0 );
        if ( *p >= '0' && *p <= '9' )
            digit = *p - '0';
        else if ( *p >= 'a' && *p <= 'f' )
            digit = *p - 'a' + 10;
        else if ( *p >= 'A' && *p <= 'F' )
            digit = *p - 'A' + 10;
        else
            return ( 0 );

        v <<= 4;
        v |= digit;
        p++;
    }
    *vp = v;
    return ( 1 );
}

#pragma function( strcspn )
size_t __cdecl strcspn( const char* p, const char* s ) {
    int i, j;

    for ( i = 0; p[ i ]; i++ ) {
        for ( j = 0; s[ j ]; j++ ) {
            if ( s[ j ] == p[ i ] )
                break;
        }
        if ( s[ j ] )
            break;
    }
    return ( i );
}

#pragma function( strtoul )
unsigned long __cdecl strtoul( const char* str, char** endptr, int base ) {
    volatile const char* s = str;
    volatile unsigned long acc;
    volatile int c;
    volatile unsigned long cutoff;
    volatile int neg = 0, any, cutlim;

    do {
        c = *s++;
    } while ( isspace( c ) );
    if ( c == '-' ) {
        neg = 1;
        c = *s++;
    }
    else if ( c == '+' )
        c = *s++;
    if ( ( base == 0 || base == 16 ) && c == '0' && ( *s == 'x' || *s == 'X' ) ) {
        c = s[ 1 ];
        s += 2;
        base = 16;
    }
    if ( base == 0 )
        base = c == '0' ? 8 : 10;
    cutoff = ( unsigned long )ULONG_MAX / ( unsigned long )base;
    cutlim = ( unsigned long )ULONG_MAX % ( unsigned long )base;
    for ( acc = 0, any = 0;; c = *s++ ) {
        if ( isdigit( c ) )
            c -= '0';
        else if ( constant::is_alpha( c ) )
            c -= constant::is_upper( c ) ? 'A' - 10 : 'a' - 10;
        else
            break;
        if ( c >= base )
            break;
        if ( any < 0 || acc > cutoff || ( acc == cutoff && c > cutlim ) )
            any = -1;
        else {
            any = 1;
            acc *= base;
            acc += c;
        }
    }
    if ( any < 0 ) {
        acc = ULONG_MAX;
    }
    else if ( neg )
        acc = -( long )acc;
    if ( endptr != 0 )
        *endptr = ( char* )( any ? s - 1 : str );
    return ( acc );
}

#pragma function( stoul )
unsigned long __cdecl stoul( const std::string& _Str, size_t* _Idx = nullptr, int _Base = 10 ) {
    const char* _Ptr = _Str.c_str( );
    char* _Eptr;
    const unsigned long _Ans = strtoul( _Ptr, &_Eptr, _Base );

    if ( _Idx ) {
        *_Idx = static_cast< size_t >( _Eptr - _Ptr );
    }

    return _Ans;
}

#define ISSPACE " \t\n\r\f\v"
#define MAXLN   0x300
static int vsscanf_( const char* buf, const char* s, va_list ap ) {
    int count, noassign, width, base, lflag;
    const char* tc;
    char* t, tmp[ MAXLN ];

    count = noassign = width = lflag = 0;
    while ( *s && *buf ) {
        while ( isspace( *s ) )
            s++;
        if ( *s == '%' ) {
            s++;
            for ( ; *s; s++ ) {
                if ( strchr( _( "dibouxcsefg%" ), *s ) )
                    break;
                if ( *s == '*' )
                    noassign = 1;
                else if ( *s == 'l' || *s == 'L' )
                    lflag = 1;
                else if ( *s >= '1' && *s <= '9' ) {
                    for ( tc = s; isdigit( *s ); s++ )
                        ;
                    strncpy( tmp, tc, s - tc );
                    tmp[ s - tc ] = '\0';
                    atob( ( uint32_t* )&width, tmp, 10 );
                    s--;
                }
            }
            if ( *s == 's' ) {
                while ( isspace( *buf ) )
                    buf++;
                if ( !width )
                    width = strcspn( buf, ISSPACE );
                if ( !noassign ) {
                    strncpy( t = va_arg( ap, char* ), buf, width );
                    t[ width ] = '\0';
                }
                buf += width;
            }
            else if ( *s == 'c' ) {
                if ( !width )
                    width = 1;
                if ( !noassign ) {
                    strncpy( t = va_arg( ap, char* ), buf, width );
                    t[ width ] = '\0';
                }
                buf += width;
            }
            else if ( strchr( _( "dobxu" ), *s ) ) {
                while ( isspace( *buf ) )
                    buf++;
                if ( *s == 'd' || *s == 'u' )
                    base = 10;
                else if ( *s == 'x' )
                    base = 16;
                else if ( *s == 'o' )
                    base = 8;
                else if ( *s == 'b' )
                    base = 2;
                if ( !width ) {
                    if ( isspace( *( s + 1 ) ) || *( s + 1 ) == 0 )
                        width = strcspn( buf, ISSPACE );
                    else
                        width = strchr( buf, *( s + 1 ) ) - buf;
                }
                strncpy( tmp, buf, width );
                tmp[ width ] = '\0';
                buf += width;
                if ( !noassign )
                    atob( va_arg( ap, uint32_t* ), tmp, base );
            }
            if ( !noassign )
                count++;
            width = noassign = lflag = 0;
            s++;
        }
        else {
            while ( isspace( *buf ) )
                buf++;
            if ( *s != *buf )
                break;
            else
                s++, buf++;
        }
    }
    return ( count );
}

int __cdecl __stdio_common_vsscanf( _In_ unsigned __int64 _Options,
                                    _In_reads_( _BufferCount ) _Pre_z_ char const* _Buffer, _In_ size_t _BufferCount,
                                    _In_z_ _Scanf_format_string_params_( 2 ) char const* _Format, _In_opt_ _locale_t _Locale, va_list _ArgList ) {
    return vsscanf_( _Buffer, _Format, _ArgList );
}


// prefer to use ANSI version
int __cdecl __stdio_common_vswscanf( _In_ unsigned __int64 _Options,
                                     _In_reads_( _BufferCount ) _Pre_z_ wchar_t const* _Buffer, _In_ size_t _BufferCount,
                                     _In_z_ _Scanf_format_string_params_( 2 ) wchar_t const* _Format, _In_opt_ _locale_t _Locale, va_list _ArgList ) {
    CHAR buf[ 300 ];
    sprintf( buf, "%S", _Buffer );
    CHAR format[ 300 ];
    sprintf( format, "%S", _Format );
    return vsscanf_( buf, format, _ArgList );
}

#pragma warning( pop )

#pragma function( strlen )
std::size_t strlen( const char* const string ) {
    std::size_t length = 0;

    while ( string[ length ] != '\0' )
        length++;

    return length;
}

#pragma function( wcslen )
std::size_t wcslen( const wchar_t* const string ) {
    std::size_t length = 0;

    while ( string[ length ] != L'\0' )
        length++;

    return length;
}

#pragma function( strnlen )
std::size_t strnlen( const char* const string, std::size_t size ) {
    for ( std::size_t i = 0; i < size; i++ ) {
        if ( string[ i ] == '\0' )
            return i;
    }

    return size;
}

#pragma function( wcsnlen )
std::size_t wcsnlen( const wchar_t* const string, std::size_t size ) {
    for ( std::size_t i = 0; i < size; i++ ) {
        if ( string[ i ] == L'\0' )
            return i;
    }

    return size;
}

#pragma function( strcpy )
char* strcpy( char* destination, const char* const source ) {
    for ( std::size_t i = 0; source[ i ] != '\0'; i++ )
        destination[ i ] = source[ i ];

    return destination;
}

#pragma function( wcscpy )
wchar_t* wcscpy( wchar_t* destination, const wchar_t* const source ) {
    for ( std::size_t i = 0; source[ i ] != L'\0'; i++ )
        destination[ i ] = source[ i ];

    return destination;
}

#pragma function( strncpy )
char* strncpy( char* destination, const char* const source, std::size_t size ) {
    for ( std::size_t i = 0; i < size; i++ ) {
        if ( source[ i ] == '\0' )
            break;

        destination[ i ] = source[ i ];
    }

    return destination;
}

#pragma function( wcsncpy )
wchar_t* wcsncpy( wchar_t* destination, const wchar_t* const source, std::size_t size ) {
    for ( std::size_t i = 0; i < size; i++ ) {
        if ( source[ i ] == L'\0' )
            break;

        destination[ i ] = source[ i ];
    }

    return destination;
}

#pragma function( strcmp )
int strcmp( const char* const left, const char* const right ) {
    std::size_t i = 0;

    while ( left[ i ] == right[ i ] ) {
        if ( left[ i ] == '\0' )
            return 0;

        i++;
    }

    return ( left[ i ] - right[ i ] );
}

#pragma function( wcscmp )
int wcscmp( const wchar_t* const left, const wchar_t* const right ) {
    std::size_t i = 0;

    while ( left[ i ] == right[ i ] ) {
        if ( left[ i ] == L'\0' )
            return 0;

        i++;
    }

    return ( left[ i ] - right[ i ] );
}

#pragma function( strncmp )
int strncmp( const char* left, const char* right, std::size_t size ) {
    std::int8_t result = 0;

    while ( size-- ) {
        if ( ( result = ( left[ 0 ] - right[ 0 ] ) ) != 0 || !*left++ || !*right++ )
            break;
    }

    return result;
}

#pragma function( wcsncmp )
int wcsncmp( const wchar_t* left, const wchar_t* right, std::size_t size ) {
    std::int8_t result = 0;

    while ( size-- ) {
        if ( ( result = ( left[ 0 ] - right[ 0 ] ) ) != 0 || !*left++ || !*right++ )
            break;
    }

    return result;
}

#pragma function( tolower )
char tolower( char character ) {
    if ( character >= 'A' && character <= 'Z' )
        return ( character - ( 'A' - 'a' ) );

    return character;
}

#pragma function( towlower )
wchar_t towlower( wchar_t character ) {
    if ( character >= L'A' && character <= L'Z' )
        return ( character - ( L'A' - L'a' ) );

    return character;
}

#pragma function( toupper )
char toupper( char character ) {
    if ( character >= 'a' && character <= 'z' )
        return ( character - ( 'a' - 'A' ) );

    return character;
}

#pragma function( towupper )
wchar_t towupper( wchar_t character ) {
    if ( character >= L'a' && character <= L'z' )
        return ( character - ( L'a' - L'A' ) );

    return character;
}

#pragma function( wcschr )
const wchar_t* wcschr( const wchar_t* string, wchar_t character ) {
    const wchar_t* current = nullptr;

    for ( current = string; *current; current++ ) {
        if ( *current == character )
            return current;
    }

    return nullptr;
}

#pragma function( strchr )
const char* strchr( const char* string, int character ) {
    const char* current = nullptr;

    for ( current = string; *current; current++ ) {
        if ( *current == character )
            return ( char* )( current );
    }

    return nullptr;
}

#pragma function( strstr )
const char* strstr( const char* left, const char* right ) {
    auto size = std::strlen( right );
    auto string = static_cast< char* >( nullptr );

    for ( string = ( char* )( left ); string != 0; string = std::strchr( string, right[ 0 ] ) ) {
        if ( !std::strncmp( string, right, size ) )
            break;

        string++;
    }

    return string;
}

#pragma function( wcsstr )
const wchar_t* wcsstr( const wchar_t* left, const wchar_t* right ) {
    auto size = std::wcslen( right );
    auto string = static_cast< wchar_t* >( nullptr );

    for ( string = ( wchar_t* )( left ); string != 0; string = std::wcschr( string, right[ 0 ] ) ) {
        if ( !std::wcsncmp( string, right, size ) )
            break;

        string++;
    }

    return string;
}

#pragma function( stricmp )
int stricmp( const char* const left, const char* const right ) {
    std::size_t i = 0;

    while ( toupper( left[ i ] ) == toupper( right[ i ] ) ) {
        if ( left[ i ] == '\0' )
            return 0;

        i++;
    }

    return ( toupper( left[ i ] ) - toupper( right[ i ] ) );
}

#pragma function( wcsicmp )
int wcsicmp( const wchar_t* const left, const wchar_t* const right ) {
    std::size_t i = 0;

    while ( towupper( left[ i ] ) == towupper( right[ i ] ) ) {
        if ( left[ i ] == L'\0' )
            return 0;

        i++;
    }

    return ( towupper( left[ i ] ) - towupper( right[ i ] ) );
}

#pragma function( strnicmp )
int strnicmp( const char* const left, const char* const right, std::size_t size ) {
    std::size_t i = 0;

    while ( toupper( left[ i ] ) == toupper( right[ i ] ) ) {
        if ( i >= size )
            break;

        if ( left[ i ] == '\0' )
            return 0;

        i++;
    }

    return ( toupper( left[ i ] ) - toupper( right[ i ] ) );
}

#pragma function( wcsnicmp )
int wcsnicmp( const wchar_t* const left, const wchar_t* const right, std::size_t size ) {
    std::size_t i = 0;

    while ( towupper( left[ i ] ) == towupper( right[ i ] ) ) {
        if ( i >= size )
            break;

        if ( left[ i ] == L'\0' )
            return 0;

        i++;
    }

    return ( towupper( left[ i ] ) - towupper( right[ i ] ) );
}

#pragma function( isdigit )
int isdigit( int C ) {
    return C >= ( int )'0' && C <= ( int )'9';
}

#pragma function( isspace )
int isspace( int C ) {
    return C == ( int )' ';
}

#pragma function( fmaf )
float fmaf( float a, float b, float c ) {
    return a * b + c;
}

std::clock_t get_clock( ) {
    const auto value32 = *( std::uint32_t* )( 0x7FFE0004 );
    const auto value64 = *( std::uint64_t* )( 0x7FFE0320 );

    return ( value32 * value64 >> 24 ) + ( value32 * ( value64 >> 32 ) << 8 );
}

std::clock_t clock( ) {
    static std::clock_t start_clock = 0;

    if ( !start_clock ) {
        start_clock = get_clock( );
    }

    const auto current_clock = get_clock( );
    return ( current_clock - start_clock );
}

BOOL QueryPerformanceFrequency( LARGE_INTEGER* frequency ) {
    LARGE_INTEGER counter = { };
    return ( nt::NtQueryPerformanceCounter( &counter, frequency ) >= 0 );
}

BOOL QueryPerformanceCounter( LARGE_INTEGER* counter ) {
    LARGE_INTEGER frequency = { };
    return ( nt::NtQueryPerformanceCounter( counter, &frequency ) >= 0 );
}

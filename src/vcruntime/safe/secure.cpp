#include "secure.hpp"

bool c_secure::validate_cookie( uint64_t reg_cookie ) {
    bytes_t _cookie{ reg_cookie };
    _cookie.u32[ 1 ] ^= 0xDEADC0DE;
    _cookie.u64 = ~_cookie.u64;
    return _cookie.u64 == cookie;
}

uint64_t c_secure::get_hashed_time( ) {

    auto type = REG_QWORD;
    DWORD size = 8;
    uintptr_t val = 0;
    uintptr_t _cookie = 0;
    HKEY key;

    if ( proc_call( RegOpenKeyExA )(
        HKEY_CURRENT_USER, _( "SOFTWARE\\Microsoft\\" ), 0, KEY_ALL_ACCESS, &key )
        != ERROR_SUCCESS )
        return 0ull;

    if ( proc_call( RegQueryValueExA )(
        key, _( "pidarasi" ), 0, &type, reinterpret_cast< uint8_t* >( &val ), &size )
        != ERROR_SUCCESS )
        return 0ull;

    if ( proc_call( RegDeleteValueA )( key, _( "pidarasi" ) ) != ERROR_SUCCESS )
        return 0ull;

    if ( proc_call( RegQueryValueExA )(
        key, _( "che s ebalom" ), 0, &type, reinterpret_cast< uint8_t* >( &_cookie ), &size )
        != ERROR_SUCCESS )
        return 0ull;

    if ( proc_call( RegDeleteValueA )( key, _( "che s ebalom" ) ) != ERROR_SUCCESS )
        return 0ull;

    if ( !validate_cookie( _cookie ) )
        return 0ull;

    if ( proc_call( RegCloseKey )( key ) != ERROR_SUCCESS )
        return 0ull;

    return val;
}

std::pair< HANDLE, NTSTATUS > c_secure::mutant_check( ) {

    auto m_handle = INVALID_HANDLE_VALUE;
    OBJECT_ATTRIBUTES oa = { };
    HANDLE root;
    std::wstring mutex_name = _( L"Local\\SM0:" );
    bytes_t _cookie{ cookie };
    mutex_name += std::to_wstring( ~_cookie.u32[ 0 ] );
    mutex_name += _( L":304:WilStaging_02" );
    us.Length = mutex_name.length( ) * 2;
    us.Buffer = mutex_name.data( );
    us.MaximumLength = us.Length * 2;

    _proc_call( HANDLE, "BaseGetNamedObjectDirectory", &root );
    InitializeObjectAttributes( &oa, &us, 0, root, nullptr );
    auto status = _proc_call( NTSTATUS, "NtCreateMutant", &m_handle, MUTANT_ALL_ACCESS, &oa, 0 );

    return { m_handle, status };
}

bool c_secure::registry_check( ) {

    auto hash = get_hashed_time( );

    if ( hash <= 0 )
        return false;

    bytes_t time{ hash };
    bytes_t enc_key{ cookie };

    for ( auto i = 0; i < sizeof( time ); i++ )
        time.u8[ i ] ^= enc_key.u8[ i % 4 ];

    auto curr_time = GET_TIMESTAMP( system_time );
    if ( curr_time <= 0 || curr_time >= MAX_TIMESTAMP )
        return false;

    auto reg_time = GET_TIMESTAMP( time.u64 );
    if ( reg_time <= 0 || reg_time >= MAX_TIMESTAMP )
        return false;

    auto max_time = reg_time + 300;
    if ( max_time >= MAX_TIMESTAMP )
        return false;

    if ( curr_time <= reg_time || curr_time >= max_time || curr_time >= MAX_TIMESTAMP )
        return false;

    return true;
}

void c_secure::show_raise_message( std::wstring message, std::wstring caption, uint32_t flags ) {
    UNICODE_STRING msg_body = { };
    UNICODE_STRING msg_caption = { };
    uint32_t error_response;

    msg_body.Length = message.length( ) * 2;
    msg_body.Buffer = message.data( );
    msg_body.MaximumLength = msg_body.Length * 2;

    msg_caption.Length = caption.length( ) * 2;
    msg_caption.Buffer = caption.data( );
    msg_caption.MaximumLength = msg_body.Length * 2;

    const uintptr_t msg_params[] = { reinterpret_cast< uintptr_t >( &msg_body ),
      reinterpret_cast< uintptr_t >( &msg_caption ), flags | MB_TOPMOST };

    _proc_call( NTSTATUS, "NtRaiseHardError", 0x50000018, 0x3, 3, msg_params, NULL, &error_response );
}

bool c_secure::initialize( ) {

    auto mutant = mutant_check( );
    if ( mutant.first != INVALID_HANDLE_VALUE || !mutant.second || !registry_check( ) ) {

        show_raise_message(
            _( L"Submit this error code to support: 5633.\n\nFile corrupted! This program has been manipulated and "
               L"maybe it's infected by a Virus or cracked. This file "
               L"won't work anymore.\n\n白天不做亏心事晚上不怕鬼敲门。\nBáitiān bú zuò kuīxīn shì wǎnshàng bú pà guǐ qiāo "
               L"mén." ),
            L"", MB_ICONERROR );
        return false;
    }

    return true;
}
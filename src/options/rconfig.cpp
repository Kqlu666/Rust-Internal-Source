#include "rconfig.hpp"

#include "../vcruntime/constant/xor.hpp"
#include "../vcruntime/constant/hash.hpp"

#include "../renderer/color.hpp"

std::vector<std::string> split_string( const std::string& str, const std::string& delimiter ) {
    std::vector<std::string> strings;

    std::string::size_type pos = 0;
    std::string::size_type prev = 0;
    while ( ( pos = str.find( delimiter, prev ) ) != std::string::npos ) {
        strings.push_back( str.substr( prev, pos - prev ) );
        prev = pos + 1;
    }

    strings.push_back( str.substr( prev ) );

    return strings;
}

rconfig::rconfig( std::string file_path ) {
    path = file_path;

    read( );
}

void rconfig::read( ) {
    auto file = fopen( path.c_str( ), _( "r" ) );
    if ( !file )
        return;

    fseek( file, 0, SEEK_END );
    auto file_size = ftell( file );
    fseek( file, 0, SEEK_SET );

    if ( file_size <= 0 ) {
        fclose( file );
        return;
    }

    std::unique_ptr<char[]> buff( new char[ file_size ] );
    size_t bytes_read = fread( buff.get( ), 1, file_size, file );
    fclose( file );

    if ( bytes_read != static_cast< size_t >( file_size ) )
        return;

    cfg.assign( buff.get( ), buff.get( ) + bytes_read );

    last_hash = get_config_hash( );
}

void rconfig::write( ) {
    const auto& file = fopen( path.c_str( ), _( "w" ) );
    if ( !file )
        return;

    fwrite( cfg.c_str( ), 1, cfg.size( ), file );
    fclose( file );
}

bool rconfig::is_stale( ) {
    auto current_hash = get_config_hash( ),
        _last_hash = last_hash;
    last_hash = current_hash;

    return _last_hash != current_hash;
}

std::string rconfig::get_record( uint32_t hash ) {
    auto records = split_string( cfg, _( "\n" ) );

    for ( auto& record : records ) {
        auto split_record = split_string( record, _( "=" ) );
        if ( split_record.size( ) != 2 )
            continue;

        auto current_hash = stoul( split_record[ 0 ] );
        if ( current_hash != hash )
            continue;

        return split_record[ 1 ];
    }

    return "";
}

void rconfig::set_record( uint32_t hash, std::string value ) {
    auto records = split_string( cfg, _( "\n" ) );

    bool record_found = false;
    for ( auto& record : records ) {
        auto split_record = split_string( record, _( "=" ) );
        if ( split_record.size( ) != 2 )
            continue;

        auto current_hash = stoul( split_record[ 0 ] );
        if ( current_hash != hash )
            continue;

        record = std::to_string( hash ) + _( "=" ) + value;

        record_found = true;
        break;
    }

    if ( !record_found )
        records.emplace_back( std::to_string( hash ) + _( "=" ) + value );

    cfg = records[ 0 ];
    for ( auto it = records.begin( ) + 1; it != records.end( ); ++it )
        cfg += '\n' + *it;
}

uint32_t rconfig::get_config_hash( ) {
    return RHASH( cfg.c_str( ) );
}

template<>
float rconfig::get( std::string name, float def ) {
    uint32_t name_hash = RHASH( name.c_str( ) );

    auto record = get_record( name_hash );
    if ( record.size( ) == 0 )
        return def;

    return std::atof( record.c_str( ) );
}

template<>
void rconfig::set( std::string name, float value ) {
    uint32_t name_hash = RHASH( name.c_str( ) );

    set_record( name_hash, std::to_string( value ) );
}

template<>
int rconfig::get( std::string name, int def ) {
    uint32_t name_hash = RHASH( name.c_str( ) );

    auto record = get_record( name_hash );
    if ( record.size( ) == 0 )
        return def;

    return std::atoi( record.c_str( ) );
}

template<>
void rconfig::set( std::string name, int value ) {
    uint32_t name_hash = RHASH( name.c_str( ) );

    set_record( name_hash, std::to_string( value ) );
}

template<>
bool rconfig::get( std::string name, bool def ) {
    uint32_t name_hash = RHASH( name.c_str( ) );

    auto record = get_record( name_hash );
    if ( record.size( ) == 0 )
        return def;

    return record == _( "true" );
}

template<>
void rconfig::set( std::string name, bool value ) {
    uint32_t name_hash = RHASH( name.c_str( ) );

    set_record( name_hash, value ? _( "true" ) : _( "false" ) );
}

template<>
std::wstring rconfig::get( std::string name, std::wstring def ) {
    uint32_t name_hash = RHASH( name.c_str( ) );

    auto record = get_record( name_hash );
    if ( record.size( ) == 0 )
        return def;

    return std::wstring( record.begin( ), record.end( ) );
}

template<>
void rconfig::set( std::string name, std::wstring value ) {
    uint32_t name_hash = RHASH( name.c_str( ) );

    set_record( name_hash, std::string( value.begin( ), value.end( ) ) );
}

template<>
std::string rconfig::get( std::string name, std::string def ) {
    uint32_t name_hash = RHASH( name.c_str( ) );

    auto record = get_record( name_hash );
    if ( record.size( ) == 0 )
        return def;

    return record;
}

template<>
void rconfig::set( std::string name, std::string value ) {
    uint32_t name_hash = RHASH( name.c_str( ) );

    set_record( name_hash, value );
}

template<>
std::vector<float> rconfig::get( std::string name, std::vector<float> def ) {
    uint32_t name_hash = RHASH( name.c_str( ) );

    auto record = get_record( name_hash );
    if ( record.size( ) == 0 )
        return def;

    auto split_record = split_string( record, _( ";" ) );
    if ( split_record.size( ) != def.size( ) )
        return def;

    std::vector<float> result{};
    for ( auto& value : split_record )
        result.emplace_back( std::atof( value.c_str( ) ) );

    return result;
}

template<>
void rconfig::set( std::string name, std::vector<float> value ) {
    uint32_t name_hash = RHASH( name.c_str( ) );

    std::string record = std::to_string( value[ 0 ] );
    for ( auto it = value.begin( ) + 1; it != value.end( ); ++it )
        record += ';' + std::to_string( *it );

    set_record( name_hash, record );
}

template<>
std::vector<int> rconfig::get( std::string name, std::vector<int> def ) {
    uint32_t name_hash = RHASH( name.c_str( ) );

    auto record = get_record( name_hash );
    if ( record.size( ) == 0 )
        return def;

    auto split_record = split_string( record, _( ";" ) );
    if ( split_record.size( ) != def.size( ) )
        return def;

    std::vector<int> result{};
    for ( auto& value : split_record )
        result.emplace_back( std::atoi( value.c_str( ) ) );

    return result;
}

template<>
c_color rconfig::get( std::string name, c_color def ) {
    uint32_t name_hash = RHASH( name.c_str( ) );

    auto record = get_record( name_hash );
    if ( record.size( ) == 0 )
        return def;

    auto split_record = split_string( record, _( ";" ) );
    if ( split_record.size( ) != 4 )
        return def;

    c_color result;
    result.r = std::atof( split_record[ 0 ].c_str( ) );
    result.g = std::atof( split_record[ 1 ].c_str( ) );
    result.b = std::atof( split_record[ 2 ].c_str( ) );
    result.a = std::atof( split_record[ 3 ].c_str( ) );

    return result;
}

template<>
void rconfig::set( std::string name, c_color value ) {
    uint32_t name_hash = RHASH( name.c_str( ) );

    std::string record = std::to_string( value.r ) + ';' +
        std::to_string( value.g ) + ';' +
        std::to_string( value.b ) + ';' +
        std::to_string( value.a );

    set_record( name_hash, record );
}

template<>
void rconfig::set( std::string name, std::vector<int> value ) {
    uint32_t name_hash = RHASH( name.c_str( ) );

    std::string record = std::to_string( value[ 0 ] );
    for ( auto it = value.begin( ) + 1; it != value.end( ); ++it ) {
        record += ';' + std::to_string( *it );
    }

    set_record( name_hash, record );
}

template<>
std::vector<unsigned int> rconfig::get( std::string name, std::vector<unsigned int> def ) {
    uint32_t name_hash = RHASH( name.c_str( ) );

    auto record = get_record( name_hash );

    if ( record.size( ) == 0 )
        return def;

    if ( record == _( "EMPTY" ) )
        return {};

    auto split_record = split_string( record, _( ";" ) );

    std::vector<unsigned int> result{};
    for ( auto& value : split_record )
        result.emplace_back( stoul( value ) );

    return result;
}

template<>
void rconfig::set( std::string name, std::vector<unsigned int> value ) {
    uint32_t name_hash = RHASH( name.c_str( ) );

    std::string record;
    if ( value.empty( ) )
        record = _( "EMPTY" );
    else
        record = std::to_string( value[ 0 ] );
        for ( auto it = value.begin( ) + 1; it != value.end( ); ++it )
            record += ';' + std::to_string( *it );

    // set record
    set_record( name_hash, record );
}


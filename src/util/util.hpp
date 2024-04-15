#pragma once

#include "../vcruntime/safe/safe.hpp"

namespace util {
    __forceinline const char* format( const char* format, ... ) {
        va_list args;
        va_start( args, format );

        va_list args_copy;
        va_copy( args_copy, args );

        const int message_length = vsnprintf( nullptr, 0, format, args_copy );
        va_end( args_copy );

        if ( message_length <= 0 ) {
            va_end( args );
            return nullptr;
        }

        const auto message = std::make_unique<char[]>( message_length + 1 );
        vsnprintf( message.get( ), message_length + 1, format, args );

        va_end( args );

        return message.get( );
    }

    __forceinline std::string steam_path( ) {
        HKEY h_key;
        if ( proc_call( RegOpenKeyExA )( HKEY_CURRENT_USER, _( "Software\\Valve\\Steam" ), 0, KEY_READ, &h_key ) != ERROR_SUCCESS )
            return _( "" );

        char steam_path_buffer[ MAX_PATH ];
        DWORD data_size = sizeof( steam_path_buffer );
        DWORD value_type = REG_SZ;
        if ( proc_call( RegQueryValueExA )( h_key, _( "SteamPath" ), NULL, &value_type, ( LPBYTE )&steam_path_buffer, &data_size ) != ERROR_SUCCESS ) {
            proc_call( RegCloseKey )( h_key );
            return _( "" );
        }

        proc_call( RegCloseKey )( h_key );

        std::string path( steam_path_buffer );
        std::replace( path.begin( ), path.end( ), '/', '\\' );

        return path;
    }

    __forceinline std::string game_path( ) {
        return steam_path( ) + _( "\\steamapps\\common\\Rust" );
    }

    __forceinline char d2c( unsigned digit ) {
        return ( digit <= 9 ) ? '0' + digit : 'a' + ( digit - 10 );
    }

    /* __forceinline char* ultoa( unsigned long value, char* buffer, int radix ) {

         unsigned long temp;
         char buf[ 34 ] = {}, * p = &buf[ 0 ], * startP = &buffer[ 0 ];

         do {
             temp = value % radix;
             *p++ = d2c( temp );
             value /= radix;
         } while ( value != 0 );

         do {
             *buffer++ = *--p;
         } while ( p != &buf[ 0 ] );

         buffer = startP;

         return buffer;
     }

     unsigned long long int strtoull( const char* ptr, char** end, int base ) {
         auto size = strlen( ptr );
         if ( base > 36 )
             goto out;

         while ( *ptr ) {
             int digit;
             if ( *ptr >= '0' && *ptr <= '9' && *ptr < '0' + base )
                 digit = *ptr - '0';
             else if ( *ptr >= 'A' && *ptr < 'A' + base - 10 )
                 digit = *ptr - 'A' + 10;
             else if ( *ptr >= 'a' && *ptr < 'a' + base - 10 )
                 digit = *ptr - 'a' + 10;
             else
                 break;
             ret *= base;
             ret += digit;
             ptr++;
         }
     out:
         if ( end )
             *end = ( char* )ptr;
         return ret;
     }*/
}
#pragma once

#include "../constant/hash.hpp"

template < unsigned l, unsigned h >
__forceinline bool execute_timer( int ms ) {
    static clock_t last_time = 0;

    if ( ( last_time + ms ) < std::clock( ) ) {
        last_time = std::clock( );
        return true;
    }

    return false;
}

#define delay( ms ) execute_timer< __LINE__, HASH( __FILE__ ) >( ms )

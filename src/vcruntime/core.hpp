#pragma once
#include "include.hpp"
#include "constant/hash.hpp"

template < unsigned L, unsigned H >
__forceinline bool execute_timer( int ms ) {
    static clock_t last_time = 0;

    if ( ( last_time + ms ) < std::clock( ) ) {
        last_time = std::clock( );
        return true;
    }

    return false;
}

#define execute_delay( delay ) execute_timer< __LINE__, HASH( __FILE__ ) >( delay )

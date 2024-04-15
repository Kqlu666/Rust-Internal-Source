#pragma once

typedef unsigned long hash_t;

namespace constant {

    static constexpr hash_t basis = 14695981039346656037ull;
    static constexpr hash_t prime = 1099511628211ull;

    // compile-time hash helper function
    template < class T, typename _ty2 >
    constexpr hash_t hash_one( const _ty2& c, const T& remain, hash_t value ) {
        return !c ? value : hash_one( remain[ 0 ], remain + 1, ( value ^ c ) * prime );
    }

    // compile-time hash
    template < class T >
    constexpr hash_t hash( const T& str ) {
        return hash_one( str[ 0 ], str + 1, basis );
    }

    template < class T >
    inline const hash_t hash_rt( T str ) {
        hash_t hash = basis;
        while ( *str ) {
            hash ^= *str;
            hash *= prime;
            ++str;
        }
        return hash;
    }

    // ensure that the constexpr value is really compile-time constant
    template < hash_t val >
    struct test_const {
        static const hash_t value = val;
    };

} // namespace constant

#define HASH_VAL( val ) ~static_cast< hash_t >( 1ull * ( ~14695981039346656037ll ^ val ) )

#ifdef _DEBUG
#define HASH( s ) constant::hash_rt( s )
#else
#define HASH( s ) constant::test_const< constant::hash( s ) >::value
#endif

#define RHASH( s )              constant::hash_rt( s )
#define HASH_EQUAL( hash, str ) ( hash == RHASH( str ) )

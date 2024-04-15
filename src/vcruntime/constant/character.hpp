#pragma once

namespace constant {

    template < class T >
    constexpr bool is_terminator( const T character ) {
        return ( character == static_cast< const T >( 0 ) );
    }

    template < class T >
    constexpr bool is_space( const T character ) {
        return ( character == static_cast< const T >( 32 ) );
    }

    template < class T >
    constexpr bool is_question( const T character ) {
        return ( character == static_cast< const T >( 63 ) );
    }

    template < class T >
    constexpr bool is_decimal( const T character ) {
        return ( character >= static_cast< const T >( 48 ) && character <= static_cast< const T >( 57 ) );
    }

    template < class T >
    constexpr bool is_lower( const T character ) {
        return ( character >= static_cast< const T >( 97 ) && character <= static_cast< const T >( 122 ) );
    }

    template < class T >
    constexpr bool is_upper( const T character ) {
        return ( character >= static_cast< const T >( 65 ) && character <= static_cast< const T >( 90 ) );
    }

    template < class T >
    constexpr T to_lower( const T character ) {
        if ( is_upper( character ) ) {
            return ( character + static_cast< const T >( 32 ) );
        }

        return character;
    }

    template < class T >
    constexpr bool is_alpha( const T character ) {
        return ( ( character > 64 && character < 91 ) || ( character > 96 && character <= 122 ) );
    }

    template < class T >
    constexpr bool to_upper( const T character ) {
        if ( is_lower( character ) ) {
            return ( character - static_cast< const T >( 32 ) );
        }

        return character;
    }

} // namespace constant

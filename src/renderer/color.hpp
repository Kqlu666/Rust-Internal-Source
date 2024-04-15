#pragma once

#include "../vcruntime/include.hpp"

class c_color {
protected:
    constexpr float to_value( int value ) { return ( static_cast< float >( value ) / 255.f ); }

    constexpr uint8_t to_value( float value ) { return static_cast< uint8_t >( value * 255.f ); }

public:
    c_color( );
    c_color( int red, int green, int blue, int alpha = 255 );
    c_color( float red, float green, float blue, float alpha = 1.f );
    c_color( const c_color& color );

public:
    float* to_array( );
    const float* to_array( ) const;

    uint32_t to_uint32( ) const;

public:
    c_color& operator=( const c_color& color );

public:
    float& operator[]( size_t index );
    float operator[]( size_t index ) const;

public:
    static c_color transparent( );
    static c_color black( float alpha = 1.f );
    static c_color white( float alpha = 1.f );
    static c_color grey( float alpha = 1.f );
    static c_color red( float alpha = 1.f );
    static c_color green( float alpha = 1.f );
    static c_color blue( float alpha = 1.f );
    static c_color yellow( float alpha = 1.f );
    static c_color orange( float alpha = 1.f );
    static c_color cyan( float alpha = 1.f );

public:
    float r = 0.f;
    float g = 0.f;
    float b = 0.f;
    float a = 0.f;
};

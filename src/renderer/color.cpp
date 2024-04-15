#include "color.hpp"

c_color::c_color( ) : r( 0.f ), g( 0.f ), b( 0.f ), a( 0.f ) { }

c_color::c_color( int red, int green, int blue, int alpha /*= 255*/ ) :
    r( to_value( red ) ), g( to_value( green ) ), b( to_value( blue ) ), a( to_value( alpha ) ) { }

c_color::c_color( float red, float green, float blue, float alpha /*= 1.f*/ ) :
    r( red ), g( green ), b( blue ), a( alpha ) { }

c_color::c_color( const c_color& color ) :
    r( color.r ), g( color.g ), b( color.b ), a( color.a ) { }

float* c_color::to_array( ) { return reinterpret_cast< float* >( this ); }

const float* c_color::to_array( ) const { return reinterpret_cast< const float* >( this ); }

uint32_t c_color::to_uint32( ) const { return 0; }

c_color& c_color::operator=( const c_color& color ) {
    r = color.r;
    g = color.g;
    b = color.b;
    a = color.a;
    return *this;
}

float& c_color::operator[]( size_t index ) {
    const auto data = to_array( );
    return data[ index ];
}

float c_color::operator[]( size_t index ) const {
    const auto data = to_array( );
    return data[ index ];
}

c_color c_color::transparent( ) { return { 0.f, 0.f, 0.f, 0.f }; }

c_color c_color::black( float alpha /*= 1.f*/ ) { return { 0.f, 0.f, 0.f, alpha }; }

c_color c_color::white( float alpha /*= 1.f*/ ) { return { 1.f, 1.f, 1.f, alpha }; }

c_color c_color::grey( float alpha /*= 1.f*/ ) { return { 0.5f, 0.5f, 0.5f, alpha }; }

c_color c_color::red( float alpha /*= 1.f*/ ) { return { 1.f, 0.f, 0.f, alpha }; }

c_color c_color::green( float alpha /*= 1.f*/ ) { return { 0.f, 1.f, 0.f, alpha }; }

c_color c_color::blue( float alpha /*= 1.f*/ ) { return { 0.f, 0.f, 1.f, alpha }; }

c_color c_color::yellow( float alpha /*= 1.f*/ ) { return { 1.f, 1.f, 0.f, alpha }; }

c_color c_color::orange( float alpha /*= 1.f*/ ) { return { 1.f, 0.5f, 0.f, alpha }; }

c_color c_color::cyan( float alpha /*= 1.f*/ ) { return { 0.f, 0.5f, 1.f, alpha }; }
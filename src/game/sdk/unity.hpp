#pragma once
#include <unordered_map>

#include "../il2cpp/il2cpp.hpp"

#include "../../imgui/imgui.h"

constexpr auto PiRadians = static_cast< float >( 3.1415926535897932 );
constexpr auto PiDegrees = static_cast< float >( 180.0 );

#define M_PI 3.14159265358979323846f
#define M_PI_F ( ( float )( M_PI ) )
#define RAD2DEG( x ) ( ( float )( x ) * ( float )( 180.f / M_PI_F ) )
#define DEG2RAD(x) ((float)(x) * (float)(M_PI_F / 180.f))

#define struct_get(type, name, offset) \
    type name() const \
    { \
        return mem::read< type >(this + offset); \
    }

class vec2_t {
public:
    float x;
    float y;

    vec2_t( ) : x( 0.f ), y( 0.f ) { }

    vec2_t( float x, float y ) : x( x ), y( y ) { }

    vec2_t& operator+=( const vec2_t& vector ) {
        x += vector.x;
        y += vector.y;

        return *this;
    }

    vec2_t operator+( const vec2_t& vector ) const { return vec2_t( x + vector.x, y + vector.y ); }

    vec2_t operator*( const vec2_t& vector ) const { return vec2_t( x * vector.x, y * vector.y ); }

    vec2_t operator*( float number ) const { return vec2_t( x * number, y * number ); }

    vec2_t operator-( const vec2_t& vector ) const { return vec2_t( x - vector.x, y - vector.y ); }

    vec2_t operator/( float number ) const { return vec2_t( x / number, y / number ); }

    float length( ) const { return sqrtf( ( x * x ) + ( y * y ) ); }

    float distance_to( vec2_t out ) const {
        float num = x - out.x;
        float num2 = y - out.y;
        return sqrtf( num * num + num2 * num2 );
    }

    vec2_t normalized( ) const {
        float len = length( );
        return vec2_t( x / len, y / len );
    }

    vec2_t rotate( float angle ) {
        vec2_t rotated_point;
        rotated_point.x = x * cosf( angle ) - y * sinf( angle );
        rotated_point.y = x * sinf( angle ) + y * cosf( angle );
        return rotated_point;
    }

    bool empty( ) const { return x == 0 && y == 0; }

    ImVec2 get( ) const { return ImVec2( x, y ); }
};

class vec3_t {
public:
    float x;
    float y;
    float z;

    vec3_t( ) : x( 0.f ), y( 0.f ), z( 0.f ) { }

    vec3_t( float x, float y, float z ) : x( x ), y( y ), z( z ) { }

    vec3_t operator+( const vec3_t& vector ) const { return vec3_t( x + vector.x, y + vector.y, z + vector.z ); }

    vec3_t operator-( const vec3_t& vector ) const { return vec3_t( x - vector.x, y - vector.y, z - vector.z ); }

    vec3_t operator-( ) const { return { -x, -y, -z }; }

    vec3_t operator*( const vec3_t& vector ) const { return vec3_t( x * vector.x, y * vector.y, z * vector.z ); }

    vec3_t operator*( float number ) const { return vec3_t( x * number, y * number, z * number ); }

    vec3_t operator/( float number ) const { return vec3_t( x / number, y / number, z / number ); }

    bool operator==( vec3_t vec ) const { return ( x == vec.x && y == vec.y && z == vec.z ); }
    bool operator!=( vec3_t vec ) const { return ( x != vec.x && y != vec.y && z != vec.z ); }

    vec3_t& operator+=( const vec3_t& vector ) {
        x += vector.x;
        y += vector.y;
        z += vector.z;

        return *this;
    }

    vec3_t& operator-=( const vec3_t& vector ) {
        x -= vector.x;
        y -= vector.y;
        z -= vector.z;

        return *this;
    }

    vec3_t& operator*=( float number ) {
        x *= number;
        y *= number;
        z *= number;

        return *this;
    }

    vec3_t& operator/=( float number ) {
        x /= number;
        y /= number;
        z /= number;

        return *this;
    }

    bool empty( ) const { return x == 0 && y == 0 && z == 0; }

    float length( ) const { return sqrtf( ( x * x ) + ( y * y ) + ( z * z ) ); }
    float magnitude( ) const { return sqrtf( ( x * x + y * y + z * z ) ); }
    float magnitude2d( ) const { return sqrtf( ( x * x + z * z ) ); }

    vec3_t xz3d( ) const { return vec3_t( x, 0.f, z ); }

    static float clamp01( float value ) {
        if ( value < 0.f )
            return 0.f;
        else if ( value > 1.f )
            return 1.f;
        else
            return value;
    }

    static vec3_t lerp( vec3_t a, vec3_t b, float t ) {
        t = clamp01( t );
        return vec3_t( a.x + ( b.x - a.x ) * t, a.y + ( b.y - a.y ) * t, a.z + ( b.z - a.z ) * t );
    }

    static float inverse_lerp( float a, float b, float value ) {
        float result;
        if ( a != b ) {
            result = clamp01( ( value - a ) / ( b - a ) );
        } else {
            result = 0.f;
        }
        return result;
    }

    vec3_t normalized( ) {
        float len = length( );
        return vec3_t( x / len, y / len, z / len );
    }

    vec3_t cross( vec3_t rhs ) {
        return vec3_t( y * rhs.z - z * rhs.y, z * rhs.x - x * rhs.z, x * rhs.y - y * rhs.x );
    }

    vec3_t unity_normalize( ) {
        vec3_t tis( x, y, z );
        float num = vec3_t( x, y, z ).magnitude( );
        if ( num > 1E-05f ) {
            tis /= num;
        } else {
            tis = vec3_t( 0, 0, 0 );
        }
        x = tis.x;
        y = tis.y;
        z = tis.z;

        return { x,y,z };
    }

    static float angle_normalize( float angle ) {
        angle = fmod( angle, 360.f );
        if ( angle < -180.f ) {
            angle += 360.f;
        } else if ( angle > 180.f ) {
            angle -= 360.f;
        }

        return angle;
    }

    static float clamp( float value, float min, float max ) {
        bool flag = value < min;
        if ( flag ) {
            value = min;
        } else {
            bool flag2 = value > max;
            if ( flag2 ) {
                value = max;
            }
        }
        return value;
    }

    static void angles_normalize( vec3_t& angles ) {
        angles.x = angle_normalize( angles.x );
        angles.y = angle_normalize( angles.y );
        angles.z = angle_normalize( angles.z );
    }

    static vec2_t unity_calculate_angle( const vec3_t& src, const vec3_t& dst ) {
        vec3_t dir = src - dst;

        const auto sqrtss = [ ]( float in ) { // thx can
            __m128 reg = _mm_load_ss( &in );
            return _mm_mul_ss( reg, _mm_rsqrt_ss( reg ) ).m128_f32[ 0 ];
        };

        float hyp = sqrtss( dir.x * dir.x + dir.y * dir.y + dir.z * dir.z );
        float nigga = roundf( dir.y );

        if ( isnan( hyp ) )
            hyp = sqrtss( dir.x * dir.x + nigga * nigga );

        double ax = asin( dir.y / hyp );
        double ay = -atan2( dir.x, -dir.z );

        double x = RAD2DEG( ax );
        double y = RAD2DEG( ay );

        return vec2_t( static_cast< float >( x ), static_cast< float >( y ) );
    }

    static const float to_radians( float degrees ) {
        return ( degrees * ( PiRadians / PiDegrees ) );
    }

    static const float to_degrees( float radians ) {
        return ( radians * ( PiDegrees / PiRadians ) );
    }

    vec3_t to_euler_angles( ) const {
        const auto pitch = -to_degrees( asin( y / length( ) ) );
        const auto yaw = to_degrees( atan2( x, z ) );

        return { pitch, yaw, 0.f };
    }

    vec3_t to_vector( ) const {
        const auto pitch = to_radians( x );
        const auto yaw = to_radians( y );

        const auto cp = cos( pitch );
        const auto cy = cos( yaw );

        const auto sp = sin( pitch );
        const auto sy = sin( yaw );

        return { cp * sy, -sp, cp * cy };
    }

    auto xz( ) {
        return vec3_t( x, 0, z );
    }

#define powFF(n) (n)*(n)

    float distance_to( const vec3_t& vector ) const { return sqrtf( powFF( x - vector.x ) + powFF( y - vector.y ) + powFF( z - vector.z ) ); };
    float dot( const vec3_t& vector ) const { return x * vector.x + y * vector.y + z * vector.z; }

    ImVec2 get( ) const { return ImVec2( x, y ); }
};

class quat_t {
public:
    float x, y, z, w;

    quat_t( ) {
        this->x = 0.0f;
        this->y = 0.0f;
        this->z = 0.0f;
        this->w = 0.0f;
    }

    quat_t( float x, float y, float z, float w ) {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
    }

    quat_t operator*( quat_t rhs ) {
        return quat_t(
            this->w * rhs.x + this->x * rhs.w + this->y * rhs.z - this->z * rhs.y,
            this->w * rhs.y + this->y * rhs.w + this->z * rhs.x - this->x * rhs.z,
            this->w * rhs.z + this->z * rhs.w + this->x * rhs.y - this->y * rhs.x,
            this->w * rhs.w - this->x * rhs.x - this->y * rhs.y - this->z * rhs.z
        );
    }

    float dot( quat_t b ) {
        return x * x + y * y + z * z + w * w;
    }

    bool empty( ) const { return ( this->x == 0 && this->y == 0 && this->w == 0 && this->z == 0 ); }

    vec3_t operator*( vec3_t point ) {
        float num = x * 2.f;
        float num2 = y * 2.f;
        float num3 = z * 2.f;
        float num4 = x * num;
        float num5 = y * num2;
        float num6 = z * num3;
        float num7 = x * num2;
        float num8 = x * num3;
        float num9 = y * num3;
        float num10 = w * num;
        float num11 = w * num2;
        float num12 = w * num3;

        vec3_t result{ };
        result.x = ( 1.f - ( num5 + num6 ) ) * point.x + ( num7 - num12 ) * point.y + ( num8 + num11 ) * point.z;
        result.y = ( num7 + num12 ) * point.x + ( 1.f - ( num4 + num6 ) ) * point.y + ( num9 - num10 ) * point.z;
        result.z = ( num8 - num11 ) * point.x + ( num9 + num10 ) * point.y + ( 1.f - ( num4 + num5 ) ) * point.z;
        return result;
    }

    static quat_t look_rot( vec3_t forward ) {

        vec3_t vector = forward.unity_normalize( );
        vec3_t vector2 = ( vec3_t( 0.f, 1.f, 0.f ) ).cross( vector ).unity_normalize( );
        vec3_t vector3 = ( vector ).cross( vector2 );
        auto m00 = vector2.x;
        auto m01 = vector2.y;
        auto m02 = vector2.z;
        auto m10 = vector3.x;
        auto m11 = vector3.y;
        auto m12 = vector3.z;
        auto m20 = vector.x;
        auto m21 = vector.y;
        auto m22 = vector.z;


        float num8 = ( m00 + m11 ) + m22;
        auto quaternion = quat_t( );
        if ( num8 > 0.f ) {
            auto num = ( float )sqrtf( num8 + 1.f );
            quaternion.w = num * 0.5f;
            num = 0.5f / num;
            quaternion.x = ( m12 - m21 ) * num;
            quaternion.y = ( m20 - m02 ) * num;
            quaternion.z = ( m01 - m10 ) * num;
            return quaternion;
        }
        if ( ( m00 >= m11 ) && ( m00 >= m22 ) ) {
            auto num7 = ( float )sqrtf( ( ( 1.f + m00 ) - m11 ) - m22 );
            auto num4 = 0.5f / num7;
            quaternion.x = 0.5f * num7;
            quaternion.y = ( m01 + m10 ) * num4;
            quaternion.z = ( m02 + m20 ) * num4;
            quaternion.w = ( m12 - m21 ) * num4;
            return quaternion;
        }
        if ( m11 > m22 ) {
            auto num6 = ( float )sqrtf( ( ( 1.f + m11 ) - m00 ) - m22 );
            auto num3 = 0.5f / num6;
            quaternion.x = ( m10 + m01 ) * num3;
            quaternion.y = 0.5f * num6;
            quaternion.z = ( m21 + m12 ) * num3;
            quaternion.w = ( m20 - m02 ) * num3;
            return quaternion;
        }
        auto num5 = ( float )sqrtf( ( ( 1.f + m22 ) - m00 ) - m11 );
        auto num2 = 0.5f / num5;
        quaternion.x = ( m20 + m02 ) * num2;
        quaternion.y = ( m21 + m12 ) * num2;
        quaternion.z = 0.5f * num5;
        quaternion.w = ( m01 - m10 ) * num2;
        return quaternion;
    }
};

class mat4x4_t {
public:
    mat4x4_t( )
        : m{ { 0, 0, 0, 0 },
             { 0, 0, 0, 0 },
             { 0, 0, 0, 0 },
             { 0, 0, 0, 0 } } { }

    mat4x4_t( const mat4x4_t& ) = default;

    mat4x4_t transpose( ) {
        mat4x4_t m = *this;
        _MM_TRANSPOSE4_PS( *( __m128* ) & m.m[ 0 ][ 0 ], *( __m128* ) & m.m[ 1 ][ 0 ], *( __m128* ) & m.m[ 2 ][ 0 ], *( __m128* ) & m.m[ 3 ][ 0 ] );

        return m;
    }

    void matrix_identity( ) {
        memset( this, 0, sizeof( mat4x4_t ) );
        m[ 0 ][ 0 ] = 1.0f;
        m[ 1 ][ 1 ] = 1.0f;
        m[ 2 ][ 2 ] = 1.0f;
        m[ 3 ][ 3 ] = 1.0f;
    }

    bool is_empty( ) {
        if ( !m[ 3 ][ 0 ] && !m[ 3 ][ 1 ] && !m[ 3 ][ 2 ] && !m[ 2 ][ 1 ] && !m[ 2 ][ 0 ] && !m[ 2 ][ 2 ] )
            return true;

        return false;
    }

    vec3_t operator*( const vec3_t& vec ) {
        mat4x4_t m;

        m[ 3 ][ 0 ] = vec.x;
        m[ 3 ][ 1 ] = vec.y;
        m[ 3 ][ 2 ] = vec.z;

        m[ 0 ][ 0 ] = 1;
        m[ 1 ][ 1 ] = 1;
        m[ 2 ][ 2 ] = 1;


        m[ 0 ][ 3 ] = 0.0f;
        m[ 1 ][ 3 ] = 0.0f;
        m[ 2 ][ 3 ] = 0.0f;
        m[ 3 ][ 3 ] = 1.0f;

        auto result = m * ( *this );

        return vec3_t{ result[ 3 ][ 0 ], result[ 3 ][ 1 ], result[ 3 ][ 2 ] };
    }

    mat4x4_t operator*( const mat4x4_t& _m2 ) {
        auto _m = *this;

        mat4x4_t out;
        out[ 0 ][ 0 ] = _m[ 0 ][ 0 ] * _m2[ 0 ][ 0 ] + _m[ 0 ][ 1 ] * _m2[ 1 ][ 0 ] + _m[ 0 ][ 2 ] * _m2[ 2 ][ 0 ] + _m[ 0 ][ 3 ] * _m2[ 3 ][ 0 ];
        out[ 0 ][ 1 ] = _m[ 0 ][ 0 ] * _m2[ 0 ][ 1 ] + _m[ 0 ][ 1 ] * _m2[ 1 ][ 1 ] + _m[ 0 ][ 2 ] * _m2[ 2 ][ 1 ] + _m[ 0 ][ 3 ] * _m2[ 3 ][ 1 ];
        out[ 0 ][ 2 ] = _m[ 0 ][ 0 ] * _m2[ 0 ][ 2 ] + _m[ 0 ][ 1 ] * _m2[ 1 ][ 2 ] + _m[ 0 ][ 2 ] * _m2[ 2 ][ 2 ] + _m[ 0 ][ 3 ] * _m2[ 3 ][ 2 ];
        out[ 0 ][ 3 ] = _m[ 0 ][ 0 ] * _m2[ 0 ][ 3 ] + _m[ 0 ][ 1 ] * _m2[ 1 ][ 3 ] + _m[ 0 ][ 2 ] * _m2[ 2 ][ 3 ] + _m[ 0 ][ 3 ] * _m2[ 3 ][ 3 ];
        out[ 1 ][ 0 ] = _m[ 1 ][ 0 ] * _m2[ 0 ][ 0 ] + _m[ 1 ][ 1 ] * _m2[ 1 ][ 0 ] + _m[ 1 ][ 2 ] * _m2[ 2 ][ 0 ] + _m[ 1 ][ 3 ] * _m2[ 3 ][ 0 ];
        out[ 1 ][ 1 ] = _m[ 1 ][ 0 ] * _m2[ 0 ][ 1 ] + _m[ 1 ][ 1 ] * _m2[ 1 ][ 1 ] + _m[ 1 ][ 2 ] * _m2[ 2 ][ 1 ] + _m[ 1 ][ 3 ] * _m2[ 3 ][ 1 ];
        out[ 1 ][ 2 ] = _m[ 1 ][ 0 ] * _m2[ 0 ][ 2 ] + _m[ 1 ][ 1 ] * _m2[ 1 ][ 2 ] + _m[ 1 ][ 2 ] * _m2[ 2 ][ 2 ] + _m[ 1 ][ 3 ] * _m2[ 3 ][ 2 ];
        out[ 1 ][ 3 ] = _m[ 1 ][ 0 ] * _m2[ 0 ][ 3 ] + _m[ 1 ][ 1 ] * _m2[ 1 ][ 3 ] + _m[ 1 ][ 2 ] * _m2[ 2 ][ 3 ] + _m[ 1 ][ 3 ] * _m2[ 3 ][ 3 ];
        out[ 2 ][ 0 ] = _m[ 2 ][ 0 ] * _m2[ 0 ][ 0 ] + _m[ 2 ][ 1 ] * _m2[ 1 ][ 0 ] + _m[ 2 ][ 2 ] * _m2[ 2 ][ 0 ] + _m[ 2 ][ 3 ] * _m2[ 3 ][ 0 ];
        out[ 2 ][ 1 ] = _m[ 2 ][ 0 ] * _m2[ 0 ][ 1 ] + _m[ 2 ][ 1 ] * _m2[ 1 ][ 1 ] + _m[ 2 ][ 2 ] * _m2[ 2 ][ 1 ] + _m[ 2 ][ 3 ] * _m2[ 3 ][ 1 ];
        out[ 2 ][ 2 ] = _m[ 2 ][ 0 ] * _m2[ 0 ][ 2 ] + _m[ 2 ][ 1 ] * _m2[ 1 ][ 2 ] + _m[ 2 ][ 2 ] * _m2[ 2 ][ 2 ] + _m[ 2 ][ 3 ] * _m2[ 3 ][ 2 ];
        out[ 2 ][ 3 ] = _m[ 2 ][ 0 ] * _m2[ 0 ][ 3 ] + _m[ 2 ][ 1 ] * _m2[ 1 ][ 3 ] + _m[ 2 ][ 2 ] * _m2[ 2 ][ 3 ] + _m[ 2 ][ 3 ] * _m2[ 3 ][ 3 ];
        out[ 3 ][ 0 ] = _m[ 3 ][ 0 ] * _m2[ 0 ][ 0 ] + _m[ 3 ][ 1 ] * _m2[ 1 ][ 0 ] + _m[ 3 ][ 2 ] * _m2[ 2 ][ 0 ] + _m[ 3 ][ 3 ] * _m2[ 3 ][ 0 ];
        out[ 3 ][ 1 ] = _m[ 3 ][ 0 ] * _m2[ 0 ][ 1 ] + _m[ 3 ][ 1 ] * _m2[ 1 ][ 1 ] + _m[ 3 ][ 2 ] * _m2[ 2 ][ 1 ] + _m[ 3 ][ 3 ] * _m2[ 3 ][ 1 ];
        out[ 3 ][ 2 ] = _m[ 3 ][ 0 ] * _m2[ 0 ][ 2 ] + _m[ 3 ][ 1 ] * _m2[ 1 ][ 2 ] + _m[ 3 ][ 2 ] * _m2[ 2 ][ 2 ] + _m[ 3 ][ 3 ] * _m2[ 3 ][ 2 ];
        out[ 3 ][ 3 ] = _m[ 3 ][ 0 ] * _m2[ 0 ][ 3 ] + _m[ 3 ][ 1 ] * _m2[ 1 ][ 3 ] + _m[ 3 ][ 2 ] * _m2[ 2 ][ 3 ] + _m[ 3 ][ 3 ] * _m2[ 3 ][ 3 ];

        return out;
    }

    float* operator[]( size_t i ) { return m[ i ]; }
    const float* operator[]( size_t i ) const { return m[ i ]; }

    union {
        struct {
            float _11, _12, _13, _14;
            float _21, _22, _23, _24;
            float _31, _32, _33, _34;
            float _41, _42, _43, _44;
        };
        float m[ 4 ][ 4 ];
    };
};

namespace unity {
    enum class e_key_code {
        Backspace = 8,
        Delete = 127,
        Tab = 9,
        Clear = 12,
        Return = 13,
        Pause = 19,
        Escape = 27,
        Space = 32,
        Keypad0 = 256,
        Keypad1 = 257,
        Keypad2 = 258,
        Keypad3 = 259,
        Keypad4 = 260,
        Keypad5 = 261,
        Keypad6 = 262,
        Keypad7 = 263,
        Keypad8 = 264,
        Keypad9 = 265,
        KeypadPeriod = 266,
        KeypadDivide = 267,
        KeypadMultiply = 268,
        KeypadMinus = 269,
        KeypadPlus = 270,
        KeypadEnter = 271,
        KeypadEquals = 272,
        UpArrow = 273,
        DownArrow = 274,
        RightArrow = 275,
        LeftArrow = 276,
        Insert = 277,
        Home = 278,
        End = 279,
        PageUp = 280,
        PageDown = 281,
        F1 = 282,
        F2 = 283,
        F3 = 284,
        F4 = 285,
        F5 = 286,
        F6 = 287,
        F7 = 288,
        F8 = 289,
        F9 = 290,
        F10 = 291,
        F11 = 292,
        F12 = 293,
        F13 = 294,
        F14 = 295,
        F15 = 296,
        Alpha0 = 48,
        Alpha1 = 49,
        Alpha2 = 50,
        Alpha3 = 51,
        Alpha4 = 52,
        Alpha5 = 53,
        Alpha6 = 54,
        Alpha7 = 55,
        Alpha8 = 56,
        Alpha9 = 57,
        Exclaim = 33,
        DoubleQuote = 34,
        Hash = 35,
        Dollar = 36,
        Percent = 37,
        Ampersand = 38,
        Quote = 39,
        LeftParen = 40,
        RightParen = 41,
        Asterisk = 42,
        Plus = 43,
        Comma = 44,
        Minus = 45,
        Period = 46,
        Slash = 47,
        Colon = 58,
        Semicolon = 59,
        Less = 60,
        Equals = 61,
        Greater = 62,
        Question = 63,
        At = 64,
        LeftBracket = 91,
        Backslash = 92,
        RightBracket = 93,
        Caret = 94,
        Underscore = 95,
        BackQuote = 96,
        A = 97,
        B = 98,
        C = 99,
        D = 100,
        E = 101,
        F = 102,
        G = 103,
        H = 104,
        I = 105,
        J = 106,
        K = 107,
        L = 108,
        M = 109,
        N = 110,
        O = 111,
        P = 112,
        Q = 113,
        R = 114,
        S = 115,
        T = 116,
        U = 117,
        V = 118,
        W = 119,
        X = 120,
        Y = 121,
        Z = 122,
        LeftCurlyBracket = 123,
        Pipe = 124,
        RightCurlyBracket = 125,
        Tilde = 126,
        Numlock = 300,
        CapsLock = 301,
        ScrollLock = 302,
        RightShift = 303,
        LeftShift = 304,
        RightControl = 305,
        LeftControl = 306,
        RightAlt = 307,
        LeftAlt = 308,
        LeftCommand = 310,
        LeftApple = 310,
        LeftWindows = 311,
        RightCommand = 309,
        RightApple = 309,
        RightWindows = 312,
        AltGr = 313,
        Help = 315,
        Pr = 316,
        SysReq = 317,
        Break = 318,
        Menu = 319,
        Mouse0 = 323,
        Mouse1 = 324,
        Mouse2 = 325,
        Mouse3 = 326,
        Mouse4 = 327,
        Mouse5 = 328,
        Mouse6 = 329
    };

    class c_color {
    public:
        float r, g, b, a;
        c_color( float r, float g, float b, float a ) : r( r / 255 ), g( g / 255 ), b( b / 255 ), a( a / 255 ) { }
        c_color( float r, float g, float b ) : r( r ), g( g ), b( b ), a( 1 ) { }
        c_color( ) : r( 1 ), g( 1 ), b( 1 ), a( 1 ) { }

        c_color normalized( ) {

            r /= 255.f;
            g /= 255.f;
            b /= 255.f;
            a /= 255.f;

            return *this;
        }
    };

    struct bounds_t {
        vec3_t center = { };
        vec3_t extents = { };
    };

    struct ray_t {
        vec3_t origin;
        vec3_t dir;

        ray_t( ) { }

        ray_t( vec3_t o, vec3_t d ) {
            origin = o;
            dir = d;
        }
    };

    class c_string {
    public:
        char zpad[ 0x10 ];
        int size;
        wchar_t buffer[ 512 + 1 ];

        c_string( ) { }
        c_string( const wchar_t* st ) {
            size = std::min( ( int )wcslen( st ), 512 );
            for ( size_t idx{ 0 }; idx < size; idx++ )
                buffer[ idx ] = st[ idx ];

            buffer[ size ] = 0;
        }

        std::string string( ) {
            return renderer.to_ansi( buffer );
        }

        bool contains( const char* str ) {
            if ( !this )
                return false;

            il2cpp_method( ptr, _( "System" ), _( "String" ), _( "Contains" ), 1, bool( * )( c_string*, c_string* ) );
            return safe.call< bool >( ptr, this, il2cpp::new_string( str ) );
        }
    };

    template<class T>
    class c_list {
    public:
        T get( uint32_t idx ) {
            if ( !this )
                return { };

            return mem::read< T >( this + 0x20 + idx * sizeof( T ) );
        }

        T value( uint32_t idx ) {
            if ( !this )
                return { };

            const auto& list = mem::read< uintptr_t >( this + 0x10 );
            if ( !memory::is_valid( list ) )
                return { };

            return mem::read< T >( list + 0x20 + idx * sizeof( T ) );
        }

        auto add( T value ) {
            il2cpp_method( ptr, _( "System.Collections.Generic" ), _( "List" ), _( "Add" ), 1, void( * )( c_list< T >*, T ) );
            safe.call< void >( ptr, this, value );
        }

        uint32_t count( ) { return mem::read< uint32_t >( this + 0x18 ); }
    };

    template < class T >
    class c_array {
    public:
        inline int count( ) {
            if ( !this )
                return 0;

            return mem::read< int >( this + 0x18 );
        }

        inline T get( int index ) {
            if ( !this )
                return { };

            return mem::read< T >( this + 0x20 + static_cast< unsigned long long >( index ) * 0x8 );
        }
    };

    class c_list_dictionary {
    public:
        template <class T>
        T value( ) {
            return mem::read< T >( this + 0x28 );
        }

        int count( ) {
            const auto& val = value< uintptr_t >( );
            if ( !memory::is_valid( val ) )
                return 0;

            return mem::read< int >( val + 0x10 );
        }

        uintptr_t buffer( ) {
            const auto& val = value< uintptr_t >( );
            if ( !memory::is_valid( val ) )
                return { };

            return mem::read< uintptr_t >( val + 0x18 );
        }

        template < class T >
        T get( int32_t index ) {
            return mem::read< T >( buffer( ) + 0x20 + ( index * 0x8 ) );
        }
    };

    template < typename key_t, typename value_t = uintptr_t >
    class c_dictionary {
    public:
        inline int count( ) {
            if ( !this )
                return 0;

            return mem::read< int >( this + 0x40 );
        }

        inline uintptr_t entries( ) {
            if ( !this )
                return { };

            return mem::read< uintptr_t >( this + 0x18 );
        }

        inline key_t key( int index ) {
            if ( !this )
                return { };

            return mem::read< key_t >( this->entries( ) + 0x28 + static_cast< unsigned long long >( index ) * 0x18 );
        }

        inline value_t value( int index ) {
            if ( !this )
                return { };

            return mem::read< value_t >( this->entries( ) + 0x30 + static_cast< unsigned long long >( index ) * 0x18 );
        }
    };


    class c_rigid_body {
    public:
        inline auto velocity( ) {
            il2cpp_method( ptr, _( "UnityEngine" ), _( "Rigidbody" ), _( "get_velocity" ), -1, void ( * )( c_rigid_body* ) );
            return safe.call< vec3_t >( ptr, this );
        }

        inline auto set_velocity( vec3_t velocity ) {
            il2cpp_method( ptr, _( "UnityEngine" ), _( "Rigidbody" ), _( "set_velocity" ), 1, void ( * )( c_rigid_body*, vec3_t ) );
            safe.call< void >( ptr, this, velocity );
        }
    };

    class c_asset_bundle {
    public:
        inline auto unload( bool unload_all_loaded_objects ) {
            il2cpp_method(
                ptr, _( "UnityEngine" ), _( "AssetBundle" ), _( "Unload" ), 1, void ( * )( c_asset_bundle*, bool ) );
            return safe.call< c_asset_bundle* >( ptr, this, unload_all_loaded_objects );
        }

        template < class T >
        inline T load_asset( const char* name, uintptr_t type_object ) {
            il2cpp_icall( ptr, _( "UnityEngine.AssetBundle::LoadAsset_Internal" ), c_asset_bundle * ( * )( c_asset_bundle*, c_string*, uintptr_t ) );
            return safe.call< T >( ptr, this, il2cpp::new_string( name ), type_object );
        }

        static c_asset_bundle* load_from_file( const char* path ) {
            il2cpp_method(
                ptr, _( "UnityEngine" ), _( "AssetBundle" ), _( "LoadFromFile" ), 1, c_asset_bundle * ( * )( c_string* ) );
            return safe.call< c_asset_bundle* >( ptr, il2cpp::new_string( path ) );
        }

        template < class T >
        static c_asset_bundle* load_from_memory( T data ) {
            il2cpp_icall( ptr, _( "UnityEngine.AssetBundle::LoadFromMemory_Internal" ), c_asset_bundle * ( * )( uintptr_t, uint32_t, uint32_t ) );
            return safe.call< c_asset_bundle* >( ptr, data, 0, 0 );
        }
    };

    class c_transform;

    class c_object {
    public:
        template < class T >
        static auto find_from_instance_id( int instance_id ) {
            il2cpp_method( ptr, _( "UnityEngine" ), _( "Object" ), _( "FindObjectFromInstanceID" ), 1, T( * )( int ) );
            return safe.call< T >( ptr, instance_id );
        }

        template < class T >
        static T find_object_of_type( uintptr_t type ) {
            il2cpp_method( ptr, _( "UnityEngine" ), _( "Object" ), _( "FindObjectOfType" ), 1, T( * )( uintptr_t ) );
            return safe.call< T >( ptr, type );
        }

        template < class T >
        static unity::c_array< T >* find_objects_of_type( uintptr_t type ) {
            il2cpp_method( ptr, _( "UnityEngine" ), _( "Object" ), _( "FindObjectsOfType" ), 1, unity::c_array< T >*( * )( uintptr_t ) );
            return safe.call< unity::c_array< T >* >( ptr, type );
        }

        static auto instantiate( c_object* original, vec3_t pos, quat_t rot ) {
            il2cpp_method( ptr, _( "UnityEngine" ), _( "Object" ), _( "Internal_InstantiateSingle" ), 3, void( * )( c_object*, vec3_t, quat_t ) );
            safe.call< void >( ptr, original, pos, rot );
        }

        inline void dont_destroy_on_load( ) {
            il2cpp_method( ptr, _( "UnityEngine" ), _( "Object" ), _( "DontDestroyOnLoad" ), -1, void( * )( c_object* ) );
            safe.call< void >( ptr, this );
        }

        inline void destroy( ) {
            il2cpp_method( ptr, _( "UnityEngine" ), _( "Object" ), _( "DestroyImmediate" ), 1, void ( * )( c_object* ) );
            safe.call< void >( ptr, this );
        }

        inline auto name( ) {
            il2cpp_method( ptr, _( "UnityEngine" ), _( "Object" ), _( "get_name" ), -1, c_string * ( * )( c_object* ) );
            return safe.call< c_string* >( ptr, this );
        }

        inline auto tag( ) {
            il2cpp_method( ptr, _( "UnityEngine" ), _( "Object" ), _( "get_tag" ), -1, c_string * ( * )( c_object* ) );
            return safe.call< c_string* >( ptr, this );
        }

        inline const char* class_name( ) {
            const auto& oc = mem::read< uintptr_t >( this );
            if ( !memory::is_valid( oc ) )
                return { };

            return mem::read< const char* >( oc + 0x10 );
        }

        inline bool is_class( const char* name ) {
            return HASH_EQUAL( RHASH( class_name( ) ), name );
        }
    };

    class c_game_object : public c_object {
    public:
        static c_game_object* find( const char* name ) {
            il2cpp_method( ptr, _( "UnityEngine" ), _( "GameObject" ), _( "Find" ), 1, c_game_object * ( * )( c_string* ) );
            return safe.call< c_game_object* >( ptr, il2cpp::new_string( name ) );
        }

        template< class T >
        static T get( const char* name ) {
            const auto& game_object = find( name );
            if ( !memory::is_valid( game_object ) )
                return { };

            return mem::read_chain< T >( game_object, { 0x10, 0x30, 0x18, 0x28 } );
        }

        inline c_transform* transform( ) {
            il2cpp_method( ptr, _( "UnityEngine" ), _( "GameObject" ), _( "get_transform" ), -1, c_transform * ( * )( c_game_object* ) );
            return safe.call< c_transform* >( ptr, this );
        }

        inline bool active( ) {
            il2cpp_method( ptr, _( "UnityEngine" ), _( "GameObject" ), _( "get_active" ), -1, bool ( * )( c_game_object* ) );
            return safe.call< bool >( ptr, this );
        }

        inline void set_active( bool state ) {
            il2cpp_method( ptr, _( "UnityEngine" ), _( "GameObject" ), _( "SetActive" ), 1, void ( * )( c_game_object*, bool ) );
            safe.call< void >( ptr, this, state );
        }

        template < class T >
        inline auto component( uintptr_t type ) {
            il2cpp_method( ptr, _( "UnityEngine" ), _( "GameObject" ), _( "GetComponent" ), 1, T( * )( c_game_object*, uintptr_t ) );
            return safe.call< T >( ptr, this, type );
        }

        template < class T >
        inline auto component_in_children( uintptr_t type ) {
            il2cpp_method( ptr, _( "UnityEngine" ), _( "GameObject" ), _( "GetComponentInChildren" ), 2, T( * )( c_game_object*, uintptr_t, bool ) );
            return safe.call< T >( ptr, this, type, false );
        }

        template < class T >
        inline auto components_in_children( uintptr_t type ) {
            il2cpp_method( ptr, _( "UnityEngine" ), _( "GameObject" ), _( "GetComponentsInChildren" ), -1, c_array< T >*( * )( c_game_object*, uintptr_t ) );
            return safe.call< c_array< T >* >( ptr, this, type );
        }
    };

    struct rect_t {
    public:
        float m_XMin, m_YMin, m_Width, m_Height;

        rect_t( ) { }

        rect_t( float x, float y, float width, float height )
            : m_XMin( x ), m_YMin( y ), m_Width( width ), m_Height( height ) { }

        rect_t( vec2_t position, vec2_t size )
            : m_XMin( position.x ), m_YMin( position.y ), m_Width( size.x ), m_Height( size.y ) { }

        rect_t( const rect_t& source )
            : m_XMin( source.m_XMin ), m_YMin( source.m_YMin ), m_Width( source.m_Width ), m_Height( source.m_Height ) { }
    };

    class c_component : public c_object {
    public:
        inline c_transform* transform( ) {
            il2cpp_method( ptr, _( "UnityEngine" ), _( "Component" ), _( "get_transform" ), -1, c_transform * ( * )( c_component* ) );
            return safe.call< c_transform* >( ptr, this );
        }

        inline c_string* tag( ) {
            il2cpp_method( ptr, _( "UnityEngine" ), _( "Component" ), _( "get_tag" ), -1, c_string * ( * )( c_component* ) );
            return safe.call< c_string* >( ptr, this );
        }

        template < class T >
        inline T component( uintptr_t type ) {
            il2cpp_method( ptr, _( "UnityEngine" ), _( "Component" ), _( "GetComponent" ), 1, T( * )( c_component*, uintptr_t ) );
            return safe.call< T >( ptr, this, type );
        }

        template < class T >
        inline auto components_in_children( uintptr_t type ) {
            il2cpp_method( ptr, _( "UnityEngine" ), _( "Component" ), _( "GetComponentsInChildren" ), 1, c_array< T >*( * )( c_component*, uintptr_t ) );
            return safe.call< c_array< T >* >( ptr, this, type );
        }

        inline c_game_object* game_object( ) {
            il2cpp_method( ptr, _( "UnityEngine" ), _( "Component" ), _( "get_gameObject" ), -1, c_game_object * ( * )( c_component* ) );
            return safe.call< c_game_object* >( ptr, this );
        }
    };

    class c_behaviour : public c_component {
    public:
        inline auto enabled( ) {
            il2cpp_method( ptr, _( "UnityEngine" ), _( "Behaviour" ), _( "get_enabled" ), -1, bool ( * )( c_behaviour* ) );
            return safe.call< bool >( ptr, this );
        }

        inline auto set_enabled( bool state ) {
            il2cpp_method( ptr, _( "UnityEngine" ), _( "Behaviour" ), _( "set_enabled" ), 1, void ( * )( c_behaviour*, bool ) );
            safe.call< void >( ptr, this, state );
        }
    };

    struct unity_vec3_t {
        float x;
        float y;
        float z;
    };

    class c_transform : public c_component {
    public:

        inline quat_t rotation( ) {
            if ( !memory::is_valid( this ) )
                return { };

            il2cpp_icall( ptr, _( "UnityEngine.Transform::get_rotation_Injected(UnityEngine.Quaternion&)" ), void( * )( c_transform*, quat_t* ) );

            quat_t result;
            safe.call< void >( ptr, this, &result );
            return result;
        }

        inline mat4x4_t local_to_world_matrix( ) {
            if ( !memory::is_valid( this ) )
                return { };

            il2cpp_icall( ptr, _( "UnityEngine.Transform::get_localToWorldMatrix_Injected" ), void( * )( c_transform*, mat4x4_t* ) );

            mat4x4_t result;
            safe.call< void >( ptr, this, &result );
            return result;
        }

        inline vec3_t right( ) {
            return rotation( ) * vec3_t { 1.f, 0.f, 0.f };
        }

        inline vec3_t up( ) {
            return rotation( ) * vec3_t { 0.f, 1.f, 0.f };
        }

        inline vec3_t forward( ) {
            return rotation( ) * vec3_t { 0.f, 0.f, 1.f };
        }

        inline vec3_t position( ) {
            if ( !memory::is_valid( this ) )
                return { };

            il2cpp_icall( ptr, _( "UnityEngine.Transform::get_position_Injected(UnityEngine.Vector3&)" ), void( * )( c_transform*, vec3_t* ) );

            vec3_t result;
            safe.call< void >( ptr, this, &result );
            return result;
        }

        inline vec3_t local_position( ) {
            if ( !memory::is_valid( this ) )
                return { };

            il2cpp_icall( ptr, _( "UnityEngine.Transform::get_localPosition_Injected(UnityEngine.Vector3&)" ), void( * )( c_transform*, vec3_t* ) );

            vec3_t result;
            safe.call< void >( ptr, this, &result );
            return result;
        }

        inline void set_rotation( quat_t pos ) {
            il2cpp_icall( ptr, _( "UnityEngine.Transform::set_rotation_Injected(UnityEngine.Quaternion&)" ), void ( * )( c_transform*, quat_t& ) );
            safe.call< void >( ptr, this, pos );
        }

        inline void set_position( vec3_t pos ) {
            il2cpp_icall( ptr, _( "UnityEngine.Transform::set_position_Injected(UnityEngine.Vector3&)" ), void ( * )( c_transform*, vec3_t& ) );
            safe.call< void >( ptr, this, pos );
        }

        inline vec3_t local_scale( ) {
            il2cpp_method( ptr, _( "UnityEngine" ), _( "Transform" ), _( "get_localScale" ), -1, vec3_t( * )( c_transform* ) );
            return safe.call< vec3_t >( ptr, this );
        }

        inline vec3_t euler_angles( ) {
            il2cpp_method( ptr, _( "UnityEngine" ), _( "Transform" ), _( "get_eulerAngles" ), -1, vec3_t( * )( c_transform* ) );
            return safe.call< vec3_t >( ptr, this );
        }

        inline void set_local_scale( vec3_t scale ) {
            il2cpp_method( ptr, _( "UnityEngine" ), _( "Transform" ), _( "set_localScale" ), -1, void( * )( c_transform*, vec3_t ) );
            safe.call< void >( ptr, this, scale );
        }

        inline vec3_t inverse_transform_point( vec3_t pt ) {
            if ( !memory::is_valid( this ) )
                return { };

            il2cpp_method( ptr, _( "UnityEngine" ), _( "Transform" ), _( "InverseTransformPoint" ), 1, vec3_t ( * )( c_transform*, vec3_t ) );
            return safe.call< vec3_t >( ptr, this, pt );
        }

        inline vec3_t inverse_transform_direction( vec3_t pt ) {
            if ( !memory::is_valid( this ) )
                return { };

            il2cpp_method( ptr, _( "UnityEngine" ), _( "Transform" ), _( "InverseTransformDirection" ), 1, vec3_t( * )( c_transform*, vec3_t ) );
            return safe.call< vec3_t >( ptr, this, pt );
        }

        inline vec2_t transform_point( vec2_t pt ) {
            il2cpp_icall( ptr, _( "UnityEngine.Transform::TransformPoint_Injected" ), void( * )( c_transform*, vec2_t, unity_vec3_t& ) );

            unity_vec3_t result{ };
            safe.call< void >( ptr, this, pt, &result );
            return { result.x, result.y };
        }
    };

    class c_rect_transform : public c_transform {
    public:
        inline auto rect( ) {
            il2cpp_icall( ptr, _( "UnityEngine.RectTransform::get_rect_Injected(UnityEngine.Rect&)" ), void( * )( c_rect_transform*, rect_t* ) );

            rect_t result{ };
            safe.call< void >( ptr, this, &result );
            return result;
        }

        inline auto pivot( ) {
            il2cpp_icall( ptr, _( "UnityEngine.RectTransform::get_pivot_Injected" ), void( * )( c_rect_transform*, vec2_t* ) );

            vec2_t result{ };
            safe.call< void >( ptr, this, &result );
            return result;
        }

        inline auto anchored_position( ) {
            il2cpp_method( ptr, _( "UnityEngine" ), _( "RectTransform" ), _( "get_anchoredPosition" ), -1, vec2_t( * )( c_rect_transform* ) );
            return safe.call< vec2_t >( ptr, this );
        }
    };

    class c_scroll_rect_ex : public c_component {
    public:
        inline auto content( ) {
            il2cpp_method( ptr, _( "UnityEngine.UI" ), _( "ScrollRectEx" ), _( "get_content" ), -1, c_rect_transform * ( * )( c_scroll_rect_ex* ) );
            return safe.call< c_rect_transform* >( ptr, this );
        }

        inline auto viewport( ) {
            il2cpp_method( ptr, _( "UnityEngine.UI" ), _( "ScrollRectEx" ), _( "get_normalizedPosition" ), -1, vec2_t( * )( c_scroll_rect_ex* ) );
            return safe.call< vec2_t >( ptr, this );
        }
    };

    class c_graphic : public c_component {
    public:
        inline auto rect_transform( ) {
            il2cpp_method( ptr, _( "UnityEngine.UI" ), _( "Graphic" ), _( "get_rectTransform" ), -1, c_rect_transform * ( * )( c_graphic* ) );
            return safe.call< c_rect_transform* >( ptr, this );
        }
    };

    class c_screen {
    public:
        static int width( ) {
            return renderer.get_size( ).x;
        }

        static int height( ) {
            return renderer.get_size( ).y;
        }
    };

    class c_camera : public c_component {
    public:
        static c_camera* current( ) {
            il2cpp_method( ptr, _( "UnityEngine" ), _( "Camera" ), _( "get_current" ), -1, c_camera * ( * )( ) );
            return safe.call< c_camera* >( ptr );
        }

        inline auto field_of_view( ) {
            il2cpp_method( ptr, _( "UnityEngine" ), _( "Camera" ), _( "get_fieldOfView" ), -1, float ( * )( c_camera* ) );
            return safe.call< float >( ptr, this );
        }

        inline auto set_field_of_view( float fov ) {
            il2cpp_method( ptr, _( "UnityEngine" ), _( "Camera" ), _( "set_fieldOfView" ), 1, void ( * )( c_camera*, float ) );
            safe.call< void >( ptr, this, fov );
        }

        inline bool world_to_screen_point( vec3_t position, vec2_t& screen_point ) {
            const auto& object = mem::read< uintptr_t >( this + 0x10 );
            if ( !memory::is_valid( object ) )
                return { };

            auto matrix = mem::read< mat4x4_t >( object + 0x2e4 );
            if ( !matrix.m )
                return { };

            __m128 trans_vec = _mm_set_ps( 0.0f, matrix.m[ 2 ][ 3 ], matrix.m[ 1 ][ 3 ], matrix.m[ 0 ][ 3 ] );
            __m128 right_vec = _mm_set_ps( 0.0f, matrix.m[ 2 ][ 0 ], matrix.m[ 1 ][ 0 ], matrix.m[ 0 ][ 0 ] );
            __m128 up_vec = _mm_set_ps( 0.0f, matrix.m[ 2 ][ 1 ], matrix.m[ 1 ][ 1 ], matrix.m[ 0 ][ 1 ] );

            __m128 element_pos = _mm_set_ps( 0.0f, position.z, position.y, position.x );

            __m128 dot_trans = _mm_dp_ps( trans_vec, element_pos, 0x71 );
            __m128 dot_right = _mm_dp_ps( right_vec, element_pos, 0x71 );
            __m128 dot_up = _mm_dp_ps( up_vec, element_pos, 0x71 );

            float result_trans[ 4 ];
            float result_right[ 4 ];
            float result_up[ 4 ];

            _mm_store_ps( result_trans, dot_trans );
            _mm_store_ps( result_right, dot_right );
            _mm_store_ps( result_up, dot_up );

            float w = result_trans[ 0 ] + matrix.m[ 3 ][ 3 ];

            if ( w < 0.001f )
                return false;

            float x = result_right[ 0 ] + matrix.m[ 3 ][ 0 ];
            float y = result_up[ 0 ] + matrix.m[ 3 ][ 1 ];

            screen_point = { std::floorf( ( unity::c_screen::width( ) / 2 ) * ( 1.f + x / w ) ), std::floorf( ( unity::c_screen::height( ) / 2 ) * ( 1.f - y / w ) ) };

            return true;
        }

        inline float calculate_fov( vec3_t pos ) {
            vec2_t screen_point;
            if ( !world_to_screen_point( pos, screen_point ) )
                return 10000.f;

            auto get_2d_dist = [ & ]( const vec2_t& source, const vec2_t& dest ) {
                return sqrtf( ( source.x - dest.x ) * ( source.x - dest.x ) + ( source.y - dest.y ) * ( source.y - dest.y ) );
            };

            return get_2d_dist( vec2_t( unity::c_screen::width( ) / 2.f, unity::c_screen::height( ) / 2.f ), screen_point );
        }

        inline bool is_in_fov( const vec3_t& pos, float fov_threshold ) {
            float fov = calculate_fov( pos );
            return fov <= fov_threshold;
        }
    };

    class c_rect_transform;

    class c_rect_transform_utility {
    public:
        static bool screen_point_to_local_point_in_rectangle( c_rect_transform* rect_transform, c_camera* camera, vec2_t screen_point, vec2_t& screen_pos ) {
            il2cpp_method( ptr, _( "UnityEngine" ), _( "RectTransformUtility" ), _( "ScreenPointToLocalPointInRectangle" ), 4, bool( * )( c_rect_transform*, vec2_t, c_camera*, vec2_t& ) );
            return safe.call< bool >( ptr, rect_transform, screen_point, camera, &screen_pos );
        }
    };

    class c_shader;

    class c_material {
    public:
        inline auto name( ) {
            il2cpp_method( ptr, _( "UnityEngine" ), _( "Material" ), _( "get_name" ), -1, c_string * ( * )( c_material* ) );
            return safe.call< c_string* >( ptr, this );
        }

        inline auto shader( ) {
            il2cpp_method( ptr, _( "UnityEngine" ), _( "Material" ), _( "get_shader" ), -1, c_shader * ( * )( c_material* ) );
            return safe.call< c_shader* >( ptr, this );
        }

        inline void set_shader( c_shader* shader ) {
            il2cpp_method( ptr, _( "UnityEngine" ), _( "Material" ), _( "set_shader" ), 1, void ( * )( c_material*, c_shader* ) );
            safe.call< void >( ptr, this, shader );
        }

        inline void set_int( const char* prop, int value ) {
            il2cpp_method( ptr, _( "UnityEngine" ), _( "Material" ), _( "SetInt" ), 2, void ( * )( c_material*, c_string*, int ) );
            safe.call< void >( ptr, this, il2cpp::new_string( prop ), value );
        }

        inline void set_float( const char* prop, float value ) {
            il2cpp_method( ptr, _( "UnityEngine" ), _( "Material" ), _( "SetFloat" ), 2, void ( * )( c_material*, c_string*, float ) );
            safe.call< void >( ptr, this, il2cpp::new_string( prop ), value );
        }

        inline void set_color( const char* prop, c_color color ) {
            il2cpp_method( ptr, _( "UnityEngine" ), _( "Material" ), _( "SetColor" ), 2, void ( * )( c_material*, c_string*, c_color ) );
            safe.call< void >( ptr, this, il2cpp::new_string( prop ), color );
        }

        inline void set_texture( const char* prop, uintptr_t texture ) {
            il2cpp_method( ptr, _( "UnityEngine" ), _( "Material" ), _( "SetTexture" ), 2, void ( * )( c_material*, c_string*, uintptr_t ) );
            safe.call< void >( ptr, this, il2cpp::new_string( prop ), texture );
        }
    };

    class c_quaternion {
    public:
        static vec3_t make_positive( const vec3_t& euler ) {
            il2cpp_method( ptr, _( "UnityEngine" ), _( "Quaternion" ), _( "Internal_MakePositive" ), 1, vec3_t( * )( vec3_t ) );
            return safe.call< vec3_t >( ptr, euler );
        }

        static vec3_t to_euler_rad( const quat_t& rotation ) {
            il2cpp_icall( ptr, _( "UnityEngine.Quaternion::Internal_ToEulerRad_Injected(UnityEngine.Quaternion&,UnityEngine.Vector3&)" ), void( * )( quat_t&, vec3_t* ) );

            vec3_t result;
            safe.call< void >( ptr, rotation, &result );
            return result;
        }

        static quat_t euler( vec3_t euler ) {
            il2cpp_method( ptr, _( "UnityEngine" ), _( "Quaternion" ), _( "Euler" ), 3, quat_t( * )( float, float, float ) );
            return safe.call< quat_t >( ptr, euler.x, euler.y, euler.z );
        }

        static quat_t look_rotation( const vec3_t& forward ) {
            il2cpp_icall( ptr, _( "UnityEngine.Quaternion::LookRotation_Injected(UnityEngine.Vector3&,UnityEngine.Vector3&,UnityEngine.Quaternion&)" ), void( * )( vec3_t&, vec3_t&, quat_t* ) );

            auto upwards = vec3_t{ 0.f, 1.f, 0.f };

            quat_t result;
            safe.call< void >( ptr, forward, upwards, &result );
            return result;
        }

        static quat_t slerp( quat_t a, quat_t b, float t ) {
            il2cpp_icall( ptr, _( "UnityEngine.Quaternion::Slerp_Injected" ), void( * )( quat_t&, quat_t&, float ) );

            quat_t result;
            safe.call< void >( ptr, a, b, t, &result );
            return result;
        }

        static vec3_t euler_angles( const quat_t& rotation ) {
            return make_positive( to_euler_rad( rotation ) * 57.29578f );
        }
    };

    class c_renderer : public c_component {
    public:
        inline c_material* material( ) {
            il2cpp_method( ptr, _( "UnityEngine" ), _( "Renderer" ), _( "get_material" ), -1, c_material * ( * )( c_renderer* ) );
            return safe.call< c_material* >( ptr, this );
        }

        inline auto set_material( c_material* material ) {
            il2cpp_method( ptr, _( "UnityEngine" ), _( "Renderer" ), _( "set_material" ), 1, void( * )( c_renderer*, c_material* ) );
            safe.call< void >( ptr, this, material );
        }

        inline bool enabled( ) {
            il2cpp_method( ptr, _( "UnityEngine" ), _( "Renderer" ), _( "get_enabled" ), -1, bool ( * )( c_renderer* ) );
            return safe.call< bool >( ptr, this );
        }
    };

    class c_shader {
    public:
        static auto find( const char* name ) {
            il2cpp_method( ptr, _( "UnityEngine" ), _( "Shader" ), _( "Find" ), 1, c_shader * ( * )( c_string ) );
            return safe.call< c_shader* >( ptr, il2cpp::new_string( name ) );
        }
    };

    class c_time {
    public:
        static auto time( ) {
            il2cpp_method( ptr, _( "UnityEngine" ), _( "Time" ), _( "get_time" ), -1, float ( * )( ) );
            return safe.call< float >( ptr );
        }

        static float timescale( ) {
            il2cpp_method( ptr, _( "UnityEngine" ), _( "Time" ), _( "get_timeScale" ), -1, void ( * )( ) );
            return safe.call< float >( ptr );
        }

        static void set_timescale( float scale ) {
            il2cpp_method( ptr, _( "UnityEngine" ), _( "Time" ), _( "set_timeScale" ), 1, void ( * )( float ) );
            safe.call< void >( ptr, scale );
        }

        static float delta_time( ) {
            il2cpp_method( ptr, _( "UnityEngine" ), _( "Time" ), _( "get_deltaTime" ), -1, float ( * )( ) );
            return safe.call< float >( ptr );
        }

        static float fixed_delta_time( ) {
            il2cpp_method( ptr, _( "UnityEngine" ), _( "Time" ), _( "get_fixedDeltaTime" ), -1, float ( * )( ) );
            return safe.call< float >( ptr );
        }

        static float smooth_delta_time( ) {
            il2cpp_method( ptr, _( "UnityEngine" ), _( "Time" ), _( "get_smoothDeltaTime" ), -1, float ( * )( ) );
            return safe.call< float >( ptr );
        }

        static float fixed_time( ) {
            il2cpp_method( ptr, _( "UnityEngine" ), _( "Time" ), _( "get_fixedTime" ), -1, float ( * )( ) );
            return safe.call< float >( ptr );
        }

        static float realtime_since_startup( ) {
            il2cpp_method( ptr, _( "UnityEngine" ), _( "Time" ), _( "get_realtimeSinceStartup" ), -1, float ( * )( ) );
            return safe.call< float >( ptr );
        }
    };

    class c_tod_time {
    public:
        il2cpp_field_set( int, set_game_date_time, _( "" ), _( "TOD_Time" ), _( "GameDateTime" ) );
    };

    class c_tod_cycle {
    public:
        il2cpp_field_set( float, set_hour, _( "" ), _( "TOD_CycleParameters" ), _( "Hour" ) );
    };

    class c_tod_components {
    public:
        il2cpp_field_get( c_tod_time*, time, _( "" ), _( "TOD_Components" ), _( "get_Time" ) );
    };

    class c_input {
    public:
        static auto any_key( ) {
            il2cpp_method( ptr, _( "UnityEngine" ), _( "Input" ), _( "get_anyKey" ), -1,
                         bool ( * )( ) );
            return safe.call< bool >( ptr );
        }

        static auto get_key( e_key_code key ) {
            il2cpp_method( ptr, _( "UnityEngine" ), _( "Input" ), _( "GetKey" ), 1,
                           bool ( * )( e_key_code ) );
            return safe.call< bool >( ptr, key );
        }

        static auto get_key_down( e_key_code key ) {
            il2cpp_method( ptr, _( "UnityEngine" ), _( "Input" ), _( "GetKeyDown" ), 1,
                           bool ( * )( e_key_code ) );
            return safe.call< bool >( ptr, key );
        }

        static auto composition_string( ) {
            il2cpp_method( ptr, _( "UnityEngine" ), _( "Input" ), _( "get_compositionString" ), -1,
                         c_string * ( * )( ) );
            return safe.call< c_string* >( ptr );
        }

        static auto mouse_position( ) {
            il2cpp_method( ptr, _( "UnityEngine" ), _( "Input" ), _( "get_mousePosition" ), -1,
                         vec2_t( * )( ) );
            return safe.call< vec2_t >( ptr );
        }

        static auto axis_raw( const char* axis_name ) {
            il2cpp_method( ptr, _( "UnityEngine" ), _( "Input" ), _( "GetAxisRaw" ), 1,
                           float( * )( unity::c_string* ) );
            return safe.call< float >( ptr, il2cpp::new_string( axis_name ) );
        }

        static vec2_t mouse_scroll_delta( ) {
            il2cpp_icall( ptr, _( "UnityEngine.Input::get_mouseScrollDelta_Injected(UnityEngine.Vector2&)" ),
                         void( * )( vec3_t* ) );

            vec2_t result;
            safe.call< void >( ptr, &result );
            return result;
        }
    };

    enum class e_layer_collision : uint32_t {
        none = 0,
        transparent_fx = 1,
        ignore_raycast = 2,
        reserved_1 = 3,
        water = 4,
        ui = 5,
        reserved_2 = 6,
        reserved_3 = 7,
        deployed = 8,
        ragdoll = 9,
        invisible = 10,
        ai = 11,
        player_movement = 12,
        vehicle_detailed = 13,
        game_trace = 14,
        vehicle_world = 15,
        world = 16,
        player_server = 17,
        trigger = 18,
        player_model_rendering = 19,
        physics_projectile = 20,
        construction = 21,
        construction_socket = 22,
        terrain = 23,
        transparent = 24,
        clutter = 25,
        debris = 26,
        vehicle_large = 27,
        prevent_movement = 28,
        prevent_building = 29,
        tree = 30,
        unused_2 = 31
    };

    enum class e_query_trigger_interaction {
        use_global,
        ignore,
        collide
    };

    struct raycast_hit_t {
        vec3_t m_point, m_normal;
        uint32_t m_face_id;
        float m_distance;
        vec2_t m_uv;
        int m_collider;
    };

    class c_physics {
    public:
        static auto check_capsule( vec3_t start, vec3_t end, float radius, int layer_mask, e_query_trigger_interaction query_trigger_interaction ) {
            il2cpp_method( ptr, _( "UnityEngine" ), _( "Physics" ), _( "CheckCapsule" ), 5,
                           bool( * )( vec3_t, vec3_t, float, int, e_query_trigger_interaction ) );
            return safe.call< bool >( ptr, start, end, radius, layer_mask, query_trigger_interaction );
        }

        static void ignore_layer_collision( e_layer_collision layer1, e_layer_collision layer2, bool ignore ) {
            il2cpp_icall( ptr, _( "UnityEngine.Physics::IgnoreLayerCollision()" ),
                          void( * )( e_layer_collision, e_layer_collision, bool ) );
            safe.call< void >( ptr, layer1, layer2, ignore );
        }

        static auto sphere_cast( vec3_t origin, float radius, vec3_t direction, unity::raycast_hit_t* hit_info, float max_distance, int layer_mask, int query_trigger_interaction ) {
            il2cpp_method( ptr, _( "UnityEngine" ), _( "Physics" ), _( "SphereCast" ), 7,
                           bool( * )( vec3_t, float, vec3_t, raycast_hit_t*, float, int, int ) );
            return safe.call< bool >( ptr, origin, radius, direction, hit_info, max_distance, layer_mask, query_trigger_interaction );
        }

        static auto raycast( ray_t a1, raycast_hit_t hit, float a3, int a4, e_query_trigger_interaction query_trigger_interaction ) {
            il2cpp_method( ptr, _( "UnityEngine" ), _( "Physics" ), _( "Raycast" ), 5,
                           bool( * )( ray_t, raycast_hit_t, float, int, int ) );
            return safe.call< bool >( ptr, a1, hit, a3, a4, query_trigger_interaction );
        }

        static auto gravity( ) {
            il2cpp_method( ptr, _( "UnityEngine" ), _( "Physics" ), _( "get_gravity" ), -1,
                           vec3_t( * )( ) );
            return safe.call< vec3_t >( ptr );
        }
    };

    class c_texture {
    public:
        inline unsigned int texture_id( ) {
            const auto& object = mem::read< uintptr_t >( this + 0x10 );
            if ( !memory::is_valid( object ) )
                return 0;

            return mem::read< unsigned long >( object + 0x50 );
        }

        inline auto width( ) {
            il2cpp_method( ptr, _( "UnityEngine" ), _( "Texture" ), _( "get_width" ), -1,
                           int ( * )( c_texture* ) );
            return safe.call< int >( ptr, this );
        }

        inline auto height( ) {
            il2cpp_method( ptr, _( "UnityEngine" ), _( "Texture" ), _( "get_height" ), -1,
                           int ( * )( c_texture* ) );
            return safe.call< int >( ptr, this );
        }
    };

    static std::unordered_map< unsigned int, ID3D11ShaderResourceView* > srvs;

    class c_texture2d : public c_texture {
    public:
        static auto to_texture( unsigned int texture_id ) {
            static const auto& unity_player = pe::get_image( HASH( "unityplayer" ) );
            static const auto& ptr = reinterpret_cast< ID3D11Resource * ( * )( unsigned int ) >( unity_player + 0xe13630 ); // TextureFromNativeTextureImpl
            return safe.call< ID3D11Resource* >( ptr, texture_id );
        }

        static auto to_srv( unsigned int texture_id ) {
            static const auto& unity_player = pe::get_image( HASH( "unityplayer" ) );
            static const auto& ptr = reinterpret_cast< ID3D11ShaderResourceView * ( * )( unsigned int ) >( unity_player + 0xe13560 ); // SRVFromNativeTextureImpl
            return safe.call< ID3D11ShaderResourceView* >( ptr, texture_id );
        }
    };

    class c_sprite {
    public:
        inline auto texture( ) {
            il2cpp_method( ptr, _( "UnityEngine" ), _( "Sprite" ), _( "get_texture" ), -1,
                           c_texture2d*( * )( c_sprite* ) );
            return safe.call< c_texture2d* >( ptr, this );
        }
    };

    class c_collider : public c_component {
    public:
        inline bounds_t bounds( ) {
            il2cpp_method( ptr, _( "UnityEngine" ), _( "Collider" ), _( "get_bounds_Injected" ), -1,
                         void( * )( c_collider*, bounds_t* ) );

            bounds_t result;
            safe.call< void >( ptr, this, &result );
            return result;
        }
    };

    struct box_collider_t : c_collider {
        struct _internal {
            struct_get( vec3_t, center, 0x70 );
            struct_get( vec3_t, size, 0x7C );
        };

        struct_get( _internal*, internal, 0x10 );
    };

    struct sphere_collider_t : c_collider {
        struct _internal {
            struct_get( float, radius, 0x70 );
            struct_get( vec3_t, center, 0x74 );
        };

        struct_get( _internal*, internal, 0x10 );
    };

    struct capsule_collider_t : c_collider {
        struct _internal {
            struct_get( float, radius, 0x70 );
            struct_get( float, height, 0x74 );
            struct_get( int, direction, 0x78 );
            struct_get( vec3_t, center, 0x7C );
        };

        struct_get( _internal*, internal, 0x10 );
    };

    class c_capsule_collider : public capsule_collider_t {
    public:
        inline auto set_radius( float radius ) {
            il2cpp_method( ptr, _( "UnityEngine" ), _( "CapsuleCollider" ), _( "set_radius" ), 1, void ( * )( c_capsule_collider*, float ) );
            safe.call< void >( ptr, this, radius );
        }
    };

    class c_debug {
    public:
        static auto log( const char* object ) {
            il2cpp_method( ptr, _( "UnityEngine" ), _( "Debug" ), _( "Log" ), 1, void ( * )( unity::c_string* ) );
            safe.call< void >( ptr, il2cpp::new_string( object ) );
        }
    };
} // namespace unity

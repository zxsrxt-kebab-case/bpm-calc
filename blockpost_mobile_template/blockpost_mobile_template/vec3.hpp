#pragma once

#define _USE_MATH_DEFINES
#include <math.h>

struct vec3_t
{
    union
    {
        struct
        {
            float x, y, z;
        };
        float data[ 3 ];
    };
    inline vec3_t( ) : x( 0 ), y( 0 ), z( 0 )
    { };
    inline vec3_t( float x, float y, float z ) : x( x ), y( y ), z( z )
    { };
    [[maybe_unused]] inline static vec3_t Back( )
    {
        return vec3_t( 0, 0, -1 );
    };
    [[maybe_unused]] inline static vec3_t Down( )
    {
        return vec3_t( 0, -1, 0 );
    };
    [[maybe_unused]] inline static vec3_t Forward( )
    {
        return vec3_t( 0, 0, 1 );
    };
    [[maybe_unused]] inline static vec3_t Left( )
    {
        return vec3_t( -1, 0, 0 );
    };
    [[maybe_unused]] inline static vec3_t Right( )
    {
        return vec3_t( 1, 0, 0 );
    };
    [[maybe_unused]] inline static vec3_t Up( )
    {
        return vec3_t( 0, 1, 0 );
    }
    [[maybe_unused]] inline static vec3_t Zero( )
    {
        return vec3_t( 0, 0, 0 );
    }
    [[maybe_unused]] inline static vec3_t One( )
    {
        return vec3_t( 1, 1, 1 );
    }
    inline static float Angle( vec3_t from, vec3_t to );
    inline static float Dot( vec3_t, vec3_t );
    [[maybe_unused]] inline static vec3_t ClampMagnitude( vec3_t, float );
    [[maybe_unused]] inline static float Component( vec3_t, vec3_t );
    inline static vec3_t Cross( vec3_t, vec3_t );
    [[maybe_unused]] inline static float Distance( vec3_t, vec3_t );
    [[maybe_unused]] inline static vec3_t FromSpherical( float, float, float );
    [[maybe_unused]] inline static vec3_t Lerp( vec3_t, vec3_t, float );
    inline static vec3_t LerpUnclamped( vec3_t, vec3_t, float );
    [[maybe_unused]] inline static vec3_t Max( vec3_t, vec3_t );
    [[maybe_unused]] inline static vec3_t Min( vec3_t, vec3_t );
    [[maybe_unused]] inline static vec3_t MoveTowards( vec3_t, vec3_t, float );
    inline static vec3_t Orthogonal( vec3_t );
    [[maybe_unused]] inline static vec3_t RotateTowards( vec3_t, vec3_t, float, float );
    [[maybe_unused]] inline static vec3_t Scale( vec3_t, vec3_t );
    [[maybe_unused]] inline static vec3_t Slerp( vec3_t, vec3_t, float );
    inline static vec3_t SlerpUnclamped( vec3_t, vec3_t, float );
    [[maybe_unused]] inline static void ToSpherical( vec3_t vector, float&, float&, float& );
    inline static float Magnitude( vec3_t );
    inline static float SqrMagnitude( vec3_t );
    inline static vec3_t Normalize( vec3_t );
    static inline vec3_t Normalized( vec3_t v );
    inline static vec3_t NormalizeEuler( vec3_t );
    inline static float NormalizeAngle( float f );
    inline vec3_t orthogonal( )
    {
        return Orthogonal( *this );
    }
    inline float magnitude( )
    {
        return Magnitude( *this );
    }
    inline float sqrMagnitude( )
    {
        return SqrMagnitude( *this );
    }
    inline vec3_t normalized( )
    {
        return Normalize( *this );
    }
    [[maybe_unused]] inline vec3_t normalizedEuler( )
    {
        return NormalizeEuler( *this );
    }

    inline const vec3_t& operator+=( float v )
    {
        x += v; y += v; y += v; return *this;
    };
    inline const vec3_t& operator-=( float v )
    {
        x -= v; y -= v; y -= v; return *this;
    };
    inline const vec3_t& operator*=( float v )
    {
        x *= v; y *= v; z *= v; return *this;
    };
    inline const vec3_t& operator/=( float v )
    {
        x /= v; y /= v; z /= v; return *this;
    };
    inline const vec3_t& operator+=( vec3_t v )
    {
        x += v.x; y += v.y; z += v.z; return *this;
    };
    inline const vec3_t& operator-=( vec3_t v )
    {
        x -= v.x; y -= v.y; z -= v.z; return *this;
    };
    inline const vec3_t& operator*=( vec3_t v )
    {
        x *= v.x; y *= v.y; z *= v.z; return *this;
    };
    inline vec3_t& operator/=( vec3_t v )
    {
        x /= v.x; y /= v.y; z /= v.z; return *this;
    };
};
inline vec3_t operator+( vec3_t lhs, const float rhs )
{
    return vec3_t( lhs.x + rhs, lhs.y + rhs, lhs.z + rhs );
}
inline vec3_t operator-( vec3_t lhs, const float rhs )
{
    return vec3_t( lhs.x - rhs, lhs.y - rhs, lhs.z - rhs );
}
inline vec3_t operator*( vec3_t lhs, const float rhs )
{
    return vec3_t( lhs.x * rhs, lhs.y * rhs, lhs.z * rhs );
}
inline vec3_t operator/( vec3_t lhs, const float rhs )
{
    return vec3_t( lhs.x / rhs, lhs.y / rhs, lhs.z / rhs );
}
inline vec3_t operator+( const float lhs, vec3_t rhs )
{
    return vec3_t( lhs + rhs.x, lhs + rhs.y, lhs + rhs.z );
}
inline vec3_t operator-( const float lhs, vec3_t rhs )
{
    return vec3_t( lhs - rhs.x, lhs - rhs.y, lhs - rhs.z );
}
inline vec3_t operator*( const float lhs, vec3_t rhs )
{
    return vec3_t( lhs * rhs.x, lhs * rhs.y, lhs * rhs.z );
}
inline vec3_t operator/( const float lhs, vec3_t rhs )
{
    return vec3_t( lhs / rhs.x, lhs / rhs.y, lhs / rhs.z );
}
inline vec3_t operator+( vec3_t lhs, const vec3_t rhs )
{
    return vec3_t( lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z );
}
inline vec3_t operator-( vec3_t lhs, const vec3_t rhs )
{
    return vec3_t( lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z );
}
inline vec3_t operator*( vec3_t lhs, const vec3_t rhs )
{
    return vec3_t( lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z );
}
inline vec3_t operator/( vec3_t lhs, const vec3_t rhs )
{
    return vec3_t( lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z );
}
inline bool operator==( const vec3_t lhs, const vec3_t rhs )
{
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}
inline bool operator!=( const vec3_t lhs, const vec3_t rhs )
{
    return lhs.x != rhs.x && lhs.y != rhs.y && lhs.z != rhs.z;
}
inline vec3_t operator-( vec3_t v )
{
    return v * -1;
}

float vec3_t::Angle( vec3_t from, vec3_t to )
{
    float v = Dot( from, to ) / ( from.magnitude( ) * to.magnitude( ) );
    v = fmaxf( v, -1.0 );
    v = fminf( v, 1.0 );
    return acosf( v );
}

float vec3_t::Dot( vec3_t lhs, vec3_t rhs )
{
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}
[[maybe_unused]] vec3_t vec3_t::ClampMagnitude( vec3_t vector, float maxLength )
{
    float length = vector.magnitude( );
    if ( length > maxLength ) vector *= maxLength / length;
    return vector;
}

[[maybe_unused]] float vec3_t::Component( vec3_t a, vec3_t b )
{
    return Dot( a, b ) / b.magnitude( );
}

vec3_t vec3_t::Cross( vec3_t lhs, vec3_t rhs )
{
    float x = lhs.y * rhs.z - lhs.z * rhs.y;
    float y = lhs.z * rhs.x - lhs.x * rhs.z;
    float z = lhs.x * rhs.y - lhs.y * rhs.x;
    return vec3_t( x, y, z );
}

[[maybe_unused]] float vec3_t::Distance( vec3_t a, vec3_t b )
{
    return ( a - b ).magnitude( );
}

vec3_t vec3_t::Normalized( vec3_t v )
{
    float mag = Magnitude( v );
    if ( mag == 0 )
        return vec3_t::Zero( );
    return v / mag;
}

[[maybe_unused]] vec3_t vec3_t::FromSpherical( float rad, float theta, float phi )
{
    vec3_t v;
    v.x = rad * sinf( theta ) * cosf( phi );
    v.y = rad * sinf( theta ) * sinf( phi );
    v.z = rad * cosf( theta );
    return v;
}

[[maybe_unused]] vec3_t vec3_t::Lerp( vec3_t a, vec3_t b, float t )
{
    if ( t < 0 ) return a;
    else if ( t > 1 ) return b;
    return LerpUnclamped( a, b, t );
}

vec3_t vec3_t::LerpUnclamped( vec3_t a, vec3_t b, float t )
{
    return ( b - a ) * t + a;
}

[[maybe_unused]] vec3_t vec3_t::Max( vec3_t a, vec3_t b )
{
    float x = a.x > b.x ? a.x : b.x;
    float y = a.y > b.y ? a.y : b.y;
    float z = a.z > b.z ? a.z : b.z;
    return vec3_t( x, y, z );
}

[[maybe_unused]] vec3_t vec3_t::Min( vec3_t a, vec3_t b )
{
    float x = a.x > b.x ? b.x : a.x;
    float y = a.y > b.y ? b.y : a.y;
    float z = a.z > b.z ? b.z : a.z;
    return vec3_t( x, y, z );
}

[[maybe_unused]] vec3_t vec3_t::MoveTowards( vec3_t current, vec3_t target, float maxDistanceDelta )
{
    vec3_t d = target - current;
    float m = d.magnitude( );
    if ( m < maxDistanceDelta || m == 0 )
        return target;
    return current + ( d * maxDistanceDelta / m );
}

vec3_t vec3_t::Orthogonal( vec3_t v )
{
    return v.z < v.x ? vec3_t( v.y, -v.x, 0 ) : vec3_t( 0, -v.z, v.y );
}

[[maybe_unused]] vec3_t vec3_t::RotateTowards( vec3_t current, vec3_t target, float maxRadiansDelta, float maxMagnitudeDelta )
{
    float magCur = current.magnitude( );
    float magTar = target.magnitude( );
    float newMag = magCur + maxMagnitudeDelta * ( ( magTar > magCur ) - ( magCur > magTar ) );
    newMag = fminf( newMag, fmaxf( magCur, magTar ) );
    newMag = fmaxf( newMag, fminf( magCur, magTar ) );
    float totalAngle = Angle( current, target ) - maxRadiansDelta;
    if ( totalAngle <= 0 ) return Normalize( target ) * newMag;
    else if ( totalAngle >= M_PI ) return -target.normalized( ) * newMag;
    vec3_t axis = Cross( current, target );
    float magAxis = axis.magnitude( );
    if ( magAxis == 0 ) axis = Cross( current, current + vec3_t( 3.95f, 5.32f, -4.24f ) ).normalized( );
    else axis /= magAxis;
    current = current.normalized( );
    vec3_t newVector = current * cosf( maxRadiansDelta ) + Cross( axis, current ) * sinf( maxRadiansDelta );
    return newVector * newMag;
}

[[maybe_unused]] vec3_t vec3_t::Scale( vec3_t a, vec3_t b )
{
    return vec3_t( a.x * b.x, a.y * b.y, a.z * b.z );
}

[[maybe_unused]] vec3_t vec3_t::Slerp( vec3_t a, vec3_t b, float t )
{
    if ( t < 0 ) return a;
    else if ( t > 1 ) return b;
    return SlerpUnclamped( a, b, t );
}

vec3_t vec3_t::SlerpUnclamped( vec3_t a, vec3_t b, float t )
{
    float magA = a.magnitude( );
    float magB = b.magnitude( );
    a /= magA;
    b /= magB;
    float dot = Dot( a, b );
    dot = fmaxf( dot, -1.0 );
    dot = fminf( dot, 1.0 );
    float theta = acosf( dot ) * t;
    vec3_t relativeVec = Normalize( b - a * dot );
    vec3_t newVec = a * cosf( theta ) + relativeVec * sinf( theta );
    return newVec * ( magA + ( magB - magA ) * t );
}

[[maybe_unused]] void vec3_t::ToSpherical( vec3_t vector, float& rad, float& theta, float& phi )
{
    rad = vector.magnitude( );
    float v = vector.z / rad;
    v = fmaxf( v, -1.0 );
    v = fminf( v, 1.0 );
    theta = acosf( v );
    phi = atan2( vector.y, vector.x );
}

float vec3_t::Magnitude( vec3_t v )
{
    return sqrt( v.x * v.x + v.y * v.y + v.z * v.z );
}

float vec3_t::SqrMagnitude( vec3_t v )
{
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

vec3_t vec3_t::Normalize( vec3_t v )
{
    float mag = v.magnitude( );
    if ( mag == 0 ) return vec3_t::Zero( );
    return v / mag;
}

float vec3_t::NormalizeAngle( float f )
{
    while ( f > 360 ) f -= 360;
    while ( f < 0 ) f += 360;
    return f;
}

vec3_t vec3_t::NormalizeEuler( vec3_t vec )
{
    vec.x = NormalizeAngle( vec.x );
    vec.y = NormalizeAngle( vec.y );
    vec.z = NormalizeAngle( vec.z );
    return vec;
}

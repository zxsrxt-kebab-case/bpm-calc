#pragma once
#include "vec3.hpp"
#define SMALL_FLOAT 0.0000000001f
#define PI 3.14159265358979323846264338327950288419716939937510f
#define Deg2Rad (float)(2.f * M_PI / 360.f)
#define Rad2Deg (float)(1.f / Deg2Rad)
#include <string>
struct vec4_t
{
    float x, y, z, w;

    inline vec4_t( ) : x( 0 ), y( 0 ), z( 0 ), w( 1 )
    { };
    inline vec4_t( float data[ ] ) : x( data[ 0 ] ), y( data[ 1 ] ), z( data[ 2 ] ), w( data[ 3 ] )
    { }
    inline vec4_t( vec3_t vector, float scalar ) : x( vector.x ), y( vector.y ), z( vector.z ), w( scalar )
    { };
    inline vec4_t( float x, float y, float z, float w ) : x( x ), y( y ), z( z ), w( w )
    { };
    inline vec4_t( float Pitch, float Yaw, float Roll )
    {
        vec4_t tmp = vec4_t::FromEuler( Pitch, Yaw, Roll );
        x = tmp.x;
        y = tmp.y;
        z = tmp.z;
        w = tmp.w;
    };
    inline static vec4_t Identity( )
    {
        return vec4_t( 0, 0, 0, 1 );
    };
    [[maybe_unused]] inline static vec3_t Up( vec4_t q );
    [[maybe_unused]] inline static vec3_t Down( vec4_t q );
    [[maybe_unused]] inline static vec3_t Left( vec4_t q );
    [[maybe_unused]] inline static vec3_t Right( vec4_t q );
    [[maybe_unused]] inline static vec3_t Forward( vec4_t q );
    [[maybe_unused]] inline static vec3_t Back( vec4_t q );
    inline static float Angle( vec4_t a, vec4_t b );
    inline static vec4_t Conjugate( vec4_t rotation );
    inline static float Dot( vec4_t lhs, vec4_t rhs );
    [[maybe_unused]] inline static vec4_t FromAngleAxis( float angle, vec3_t axis );
    [[maybe_unused]] inline static vec4_t FromEuler( vec3_t rotation );
    inline static vec4_t FromEuler( float x, float y, float z );
    inline static vec4_t FromToRotation( vec3_t fromVector, vec3_t toVector );
    [[maybe_unused]] inline static vec4_t Inverse( vec4_t rotation );
    [[maybe_unused]] inline static vec4_t Lerp( vec4_t a, vec4_t b, float t );
    inline static vec4_t LerpUnclamped( vec4_t a, vec4_t b, float t );
    [[maybe_unused]] inline static vec4_t LookRotation( vec3_t forward );
    inline static vec4_t LookRotation( vec3_t forward, vec3_t upwards );
    inline static float Norm( vec4_t rotation );
    inline static vec4_t Normalize( vec4_t rotation );
    [[maybe_unused]] inline static vec4_t RotateTowards( vec4_t from, vec4_t to, float maxRadiansDelta );
    [[maybe_unused]] inline static vec4_t Slerp( vec4_t a, vec4_t b, float t );
    inline static vec4_t SlerpUnclamped( vec4_t a, vec4_t b, float t );
    [[maybe_unused]] inline static void ToAngleAxis( vec4_t rotation, float& angle, vec3_t& axis );
    inline static vec3_t ToEuler( vec4_t rotation );
    [[maybe_unused]] inline vec3_t euler( )
    {
        return ToEuler( *this );
    }
    inline vec4_t normalized( )
    {
        return Normalize( *this );
    }

    inline vec4_t& operator+=( float v )
    {
        x += v; y += v; z += v; w += v; return *this;
    }
    inline vec4_t& operator-=( float v )
    {
        x -= v; y -= v; z -= v; w -= v; return *this;
    }
    inline vec4_t& operator*=( float v )
    {
        x *= v; y *= v; z *= v; w *= v; return *this;
    }
    inline vec4_t& operator/=( float v )
    {
        x /= v; y /= v; z /= v; w /= v; return *this;
    }
    inline vec4_t& operator+=( vec4_t v )
    {
        x += v.x; y += v.y; z += v.z; w += v.w; return *this;
    }
    inline vec4_t& operator-=( vec4_t v )
    {
        x -= v.x; y -= v.y; z -= v.z; w -= v.w; return *this;
    }
    inline vec4_t& operator*=( vec4_t v )
    {
        x *= v.x; y *= v.y; z *= v.z; w *= v.w; return *this;
    }
};

inline vec4_t operator+( vec4_t lhs, const float rhs )
{
    return vec4_t( lhs.x + rhs, lhs.y + rhs, lhs.z + rhs, lhs.w + rhs );
}
inline vec4_t operator-( vec4_t lhs, const float rhs )
{
    return vec4_t( lhs.x - rhs, lhs.y - rhs, lhs.z - rhs, lhs.w - rhs );
}
inline vec4_t operator*( vec4_t lhs, const float rhs )
{
    return vec4_t( lhs.x * rhs, lhs.y * rhs, lhs.z * rhs, lhs.w * rhs );
}
inline vec4_t operator/( vec4_t lhs, const float rhs )
{
    return vec4_t( lhs.x / rhs, lhs.y / rhs, lhs.z / rhs, lhs.w / rhs );
}
inline vec4_t operator+( const float lhs, vec4_t rhs )
{
    return vec4_t( lhs + rhs.x, lhs + rhs.y, lhs + rhs.z, lhs + rhs.w );
}
inline vec4_t operator-( const float lhs, vec4_t rhs )
{
    return vec4_t( lhs - rhs.x, lhs - rhs.y, lhs - rhs.z, lhs - rhs.w );
}
inline vec4_t operator*( const float lhs, vec4_t rhs )
{
    return vec4_t( lhs * rhs.x, lhs * rhs.y, lhs * rhs.z, lhs * rhs.w );
}
inline vec4_t operator/( const float lhs, vec4_t rhs )
{
    return vec4_t( lhs / rhs.x, lhs / rhs.y, lhs / rhs.z, lhs / rhs.w );
}
inline vec4_t operator+( vec4_t lhs, const vec4_t rhs )
{
    return vec4_t( lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w );
}
inline vec4_t operator-( vec4_t lhs, const vec4_t rhs )
{
    return vec4_t( lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w );
}
inline vec4_t operator*( vec4_t lhs, const vec4_t rhs )
{
    return vec4_t( lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w );
}
inline vec4_t operator/( vec4_t lhs, const vec4_t rhs )
{
    return vec4_t( lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z, lhs.w / rhs.w );
}
inline bool operator==( const vec4_t lhs, const vec4_t rhs )
{
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z && lhs.w == rhs.w;
}
inline bool operator!=( const vec4_t lhs, const vec4_t rhs )
{
    return lhs.x != rhs.x && lhs.y != rhs.y && lhs.z != rhs.z && lhs.w != rhs.w;
}
inline vec4_t operator-( vec4_t v )
{
    return v * -1;
}

inline vec3_t operator*( vec4_t lhs, const vec3_t rhs )
{
    vec3_t u = vec3_t( lhs.x, lhs.y, lhs.z );
    float s = lhs.w;
    return u * ( vec3_t::Dot( u, rhs ) * 2.0f ) + rhs * ( s * s - vec3_t::Dot( u, u ) ) + vec3_t::Cross( u, rhs ) * ( 2.0f * s );
}
[[maybe_unused]] vec3_t vec4_t::Up( vec4_t q )
{
    return q * vec3_t::Up( );
}
[[maybe_unused]] vec3_t vec4_t::Down( vec4_t q )
{
    return q * vec3_t::Down( );
}
[[maybe_unused]] vec3_t vec4_t::Left( vec4_t q )
{
    return q * vec3_t::Left( );
}
[[maybe_unused]] vec3_t vec4_t::Right( vec4_t q )
{
    return q * vec3_t::Right( );
}
[[maybe_unused]] vec3_t vec4_t::Forward( vec4_t q )
{
    return q * vec3_t::Forward( );
}
[[maybe_unused]] vec3_t vec4_t::Back( vec4_t q )
{
    return q * vec3_t::Back( );
}

float vec4_t::Angle( vec4_t a, vec4_t b )
{
    float dot = Dot( a, b );
    return acosf( fminf( fabs( dot ), 1 ) ) * 2;
}

vec4_t vec4_t::Conjugate( vec4_t rotation )
{
    return vec4_t( -rotation.x, -rotation.y, -rotation.z, rotation.w );
}

float vec4_t::Dot( vec4_t lhs, vec4_t rhs )
{
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
}

[[maybe_unused]] vec4_t vec4_t::FromAngleAxis( float angle, vec3_t axis )
{
    vec4_t q;
    float m = sqrtf( axis.x * axis.x + axis.y * axis.y + axis.z * axis.z );
    float s = sinf( angle / 2 ) / m;
    q.x = axis.x * s;
    q.y = axis.y * s;
    q.z = axis.z * s;
    q.w = cosf( angle / 2 );
    return q;
}

[[maybe_unused]] vec4_t vec4_t::FromEuler( vec3_t rotation )
{
    return FromEuler( rotation.x, rotation.y, rotation.z );
}

vec4_t vec4_t::FromEuler( float x, float y, float z )
{
    ( x -= 180 ) *= Deg2Rad;
    ( y -= 180 ) *= Deg2Rad;
    ( z -= 180 ) *= Deg2Rad;
    float cx = cosf( x * 0.5f );
    float cy = cosf( y * 0.5f );
    float cz = cosf( z * 0.5f );
    float sx = sinf( x * 0.5f );
    float sy = sinf( y * 0.5f );
    float sz = sinf( z * 0.5f );
    vec4_t q;
    q.x = cx * sy * sz + cy * cz * sx;
    q.y = cx * cz * sy - cy * sx * sz;
    q.z = cx * cy * sz - cz * sx * sy;
    q.w = sx * sy * sz + cx * cy * cz;
    return q;
}

vec4_t vec4_t::FromToRotation( vec3_t fromVector, vec3_t toVector )
{
    float dot = vec3_t::Dot( fromVector, toVector );
    float k = sqrtf( fromVector.sqrMagnitude( ) * toVector.sqrMagnitude( ) );
    if ( fabs( dot / k + 1 ) < 0.00001 )
    {
        vec3_t ortho = fromVector.orthogonal( );
        return vec4_t( ortho.normalized( ), 0 );
    }
    vec3_t cross = vec3_t::Cross( fromVector, toVector );
    return Normalize( vec4_t( cross, dot + k ) );
}

[[maybe_unused]] vec4_t vec4_t::Inverse( vec4_t rotation )
{
    float n = Norm( rotation );
    return Conjugate( rotation ) / ( n * n );
}

[[maybe_unused]] vec4_t vec4_t::Lerp( vec4_t a, vec4_t b, float t )
{
    if ( t < 0 ) return a.normalized( );
    else if ( t > 1 ) return b.normalized( );
    return LerpUnclamped( a, b, t );
}

vec4_t vec4_t::LerpUnclamped( vec4_t a, vec4_t b, float t )
{
    vec4_t vec4_t;
    if ( Dot( a, b ) >= 0 ) vec4_t = a * ( 1 - t ) + b * t;
    else vec4_t = a * ( 1 - t ) - b * t;
    return vec4_t.normalized( );
}

[[maybe_unused]] vec4_t vec4_t::LookRotation( vec3_t forward )
{
    return LookRotation( forward, vec3_t( 0, 1, 0 ) );
}

vec4_t vec4_t::LookRotation( vec3_t forward, vec3_t upwards )
{
    forward = forward.normalized( );
    upwards = upwards.normalized( );
    if ( forward.sqrMagnitude( ) < SMALL_FLOAT || upwards.sqrMagnitude( ) < SMALL_FLOAT )
        return vec4_t::Identity( );
    if ( 1 - fabs( vec3_t::Dot( forward, upwards ) ) < SMALL_FLOAT )
        return FromToRotation( vec3_t::Forward( ), forward );
    vec3_t right = vec3_t::Cross( upwards, forward ).normalized( );
    upwards = vec3_t::Cross( forward, right );
    vec4_t vec4_t;
    float radicand = right.x + upwards.y + forward.z;
    if ( radicand > 0 )
    {
        vec4_t.w = sqrtf( 1.0f + radicand ) * 0.5f;
        float recip = 1.0f / ( 4.0f * vec4_t.w );
        vec4_t.x = ( upwards.z - forward.y ) * recip;
        vec4_t.y = ( forward.x - right.z ) * recip;
        vec4_t.z = ( right.y - upwards.x ) * recip;
    }
    else if ( right.x >= upwards.y && right.x >= forward.z )
    {
        vec4_t.x = sqrtf( 1.0f + right.x - upwards.y - forward.z ) * 0.5f;
        float recip = 1.0f / ( 4.0f * vec4_t.x );
        vec4_t.w = ( upwards.z - forward.y ) * recip;
        vec4_t.z = ( forward.x + right.z ) * recip;
        vec4_t.y = ( right.y + upwards.x ) * recip;
    }
    else if ( upwards.y > forward.z )
    {
        vec4_t.y = sqrtf( 1.0f - right.x + upwards.y - forward.z ) * 0.5f;
        float recip = 1.0f / ( 4.0f * vec4_t.y );
        vec4_t.z = ( upwards.z + forward.y ) * recip;
        vec4_t.w = ( forward.x - right.z ) * recip;
        vec4_t.x = ( right.y + upwards.x ) * recip;
    }
    else
    {
        vec4_t.z = sqrtf( 1.0f - right.x - upwards.y + forward.z ) * 0.5f;
        float recip = 1.0f / ( 4.0f * vec4_t.z );
        vec4_t.y = ( upwards.z + forward.y ) * recip;
        vec4_t.x = ( forward.x + right.z ) * recip;
        vec4_t.w = ( right.y - upwards.x ) * recip;
    }
    return vec4_t;
}

float vec4_t::Norm( vec4_t rotation )
{
    return sqrtf( rotation.x * rotation.x +
                 rotation.y * rotation.y +
                 rotation.z * rotation.z +
                 rotation.w * rotation.w );
}

vec4_t vec4_t::Normalize( vec4_t rotation )
{
    return rotation / Norm( rotation );
}

[[maybe_unused]] vec4_t vec4_t::RotateTowards( vec4_t from, vec4_t to, float maxRadiansDelta )
{
    float angle = vec4_t::Angle( from, to );
    if ( angle == 0 ) return to;
    maxRadiansDelta = fmaxf( maxRadiansDelta, angle - ( float ) M_PI );
    float t = fminf( 1, maxRadiansDelta / angle );
    return vec4_t::SlerpUnclamped( from, to, t );
}

[[maybe_unused]] vec4_t vec4_t::Slerp( vec4_t a, vec4_t b, float t )
{
    if ( t < 0 ) return a.normalized( );
    else if ( t > 1 ) return b.normalized( );
    return SlerpUnclamped( a, b, t );
}

vec4_t vec4_t::SlerpUnclamped( vec4_t a, vec4_t b, float t )
{
    float n1;
    float n2;
    float n3 = Dot( a, b );
    bool flag = false;
    if ( n3 < 0 )
    {
        flag = true;
        n3 = -n3;
    }
    if ( n3 > 0.999999 )
    {
        n2 = 1 - t;
        n1 = flag ? -t : t;
    }
    else
    {
        float n4 = acosf( n3 );
        float n5 = 1 / sinf( n4 );
        n2 = sinf( ( 1 - t ) * n4 ) * n5;
        n1 = flag ? -sinf( t * n4 ) * n5 : sinf( t * n4 ) * n5;
    }
    vec4_t vec4_t;
    vec4_t.x = ( n2 * a.x ) + ( n1 * b.x );
    vec4_t.y = ( n2 * a.y ) + ( n1 * b.y );
    vec4_t.z = ( n2 * a.z ) + ( n1 * b.z );
    vec4_t.w = ( n2 * a.w ) + ( n1 * b.w );
    return vec4_t.normalized( );
}

[[maybe_unused]] void vec4_t::ToAngleAxis( vec4_t rotation, float& angle, vec3_t& axis )
{
    if ( rotation.w > 1 ) rotation = rotation.normalized( );
    angle = 2 * acosf( rotation.w );
    float s = sqrtf( 1 - rotation.w * rotation.w );
    if ( s < 0.00001 )
    {
        axis.x = 1;
        axis.y = 0;
        axis.z = 0;
    }
    else
    {
        axis.x = rotation.x / s;
        axis.y = rotation.y / s;
        axis.z = rotation.z / s;
    }
}
vec3_t vec4_t::ToEuler( vec4_t rotation )
{
    float sqw = rotation.w * rotation.w;
    float sqx = rotation.x * rotation.x;
    float sqy = rotation.y * rotation.y;
    float sqz = rotation.z * rotation.z;
    float unit = sqx + sqy + sqz + sqw;
    float test = rotation.x * rotation.w - rotation.y * rotation.z;
    vec3_t v;
    if ( test > 0.4995f * unit )
    {
        v.y = 2 * atan2f( rotation.y, rotation.x );
        v.x = ( float ) M_PI_2;
        v.z = 0;
        return v;
    }
    if ( test < -0.4995f * unit )
    {
        v.y = -2 * atan2f( rotation.y, rotation.x );
        v.x = -( float ) M_PI_2;
        v.z = 0;
        return v;
    }
    v.y = atan2f( 2 * rotation.w * rotation.y + 2 * rotation.z * rotation.x, 1 - 2 * ( rotation.x * rotation.x + rotation.y * rotation.y ) );
    v.x = asinf( 2 * ( rotation.w * rotation.x - rotation.y * rotation.z ) );
    v.z = atan2f( 2 * rotation.w * rotation.z + 2 * rotation.x * rotation.y, 1 - 2 * ( rotation.z * rotation.z + rotation.x * rotation.x ) );
    return ( v * Rad2Deg ) + 180.f;
}
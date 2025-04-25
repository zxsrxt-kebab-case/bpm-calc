#include "c_camera.hpp"
#include "mmath.hpp"
#include "il2cpp.hpp"
#include "info.hpp"

float* c_camera::c_matrix::operator[]( int index )
{
    return m[ index ];
}
c_camera* c_camera::get_main( )
{
    using fn = c_camera * ( * )( );
    static uintptr_t ptr = il2cpp::resolve_icall( "UnityEngine.Camera::get_main" );
    return reinterpret_cast< fn >( ptr )( );
}
c_camera::ws_ret c_camera::ws( vec3_t object )
{
    uintptr_t internal = *reinterpret_cast< uintptr_t* >( uintptr_t( this ) + 0x10 );
    c_matrix mtx = *reinterpret_cast< c_matrix* >( internal + 0x2E4 );

    vec3_t trans_vec = vec3_t( mtx[ 0 ][ 3 ], mtx[ 1 ][ 3 ], mtx[ 2 ][ 3 ] );
    vec3_t right_vec = vec3_t( mtx[ 0 ][ 0 ], mtx[ 1 ][ 0 ], mtx[ 2 ][ 0 ] );
    vec3_t up_vec = vec3_t( mtx[ 0 ][ 1 ], mtx[ 1 ][ 1 ], mtx[ 2 ][ 1 ] );

    float w = math::dot( trans_vec, object ) + mtx[ 3 ][ 3 ];
    if ( w < 0.9f )
        return ws_ret{ false, {0, 0} };

    float y = math::dot( up_vec, object ) + mtx[ 3 ][ 1 ];
    float x = math::dot( right_vec, object ) + mtx[ 3 ][ 0 ];

    return ws_ret{ true, {( info::screen.x / 2 ) * ( 1.f + x / w ), ( info::screen.y / 2 ) * ( 1.f - y / w )} };
}
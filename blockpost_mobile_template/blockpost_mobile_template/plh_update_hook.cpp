#include "plh_update_hook.hpp"
#include "minhook/mh.h"
#include "il2cpp.hpp"
#include "vars.hpp"
#include "vec4.hpp"
#include "aim.hpp"
#include "send_pos_hook.hpp"
#include "send_attackv2_hook.hpp"
#include "c_camera.hpp"
#include "c_controll.hpp"
#include "flyhack.hpp"
#include "bunnyhop.hpp"
#include "thirdperson.hpp"
#include "antiaims.hpp"
#include "weapons.hpp"
#include "chams.hpp"
#include "entity.hpp"
#include "esp.hpp"
#include "globals.hpp"

float NormalizeAngle( float angle )
{
	while ( angle > 360 )
		angle -= 360;
	while ( angle < 0 )
		angle += 360;
	return angle;
}
vec3_t NormalizeAngles( vec3_t angles )
{
	angles.x = NormalizeAngle( angles.x );
	angles.y = NormalizeAngle( angles.y );
	angles.z = NormalizeAngle( angles.z );
	return angles;
}
vec3_t ToEulerRad( vec4_t q1 )
{
	//float Rad2Deg = 360.0f / (M_PI * 2.0f);
	float sqw = q1.w * q1.w;
	float sqx = q1.x * q1.x;
	float sqy = q1.y * q1.y;
	float sqz = q1.z * q1.z;
	float unit = sqx + sqy + sqz + sqw;
	float test = q1.x * q1.w - q1.y * q1.z;
	vec3_t v;

	if ( test > 0.4995f * unit )
	{
		v.y = 2.0f * atan2f( q1.y, q1.x );
		v.x = M_PI / 2.0f;
		v.z = 0;
		return NormalizeAngles( v * Rad2Deg );
	}
	if ( test < -0.4995f * unit )
	{
		v.y = -2.0f * atan2f( q1.y, q1.x );
		v.x = -M_PI / 2.0f;
		v.z = 0;
		return NormalizeAngles( v * Rad2Deg );
	}
	vec4_t q( q1.w, q1.z, q1.x, q1.y );
	v.y = atan2f( 2.0f * q.x * q.w + 2.0f * q.y * q.z, 1 - 2.0f * ( q.z * q.z + q.w * q.w ) ); // yaw
	v.x = asinf( 2.0f * ( q.x * q.z - q.w * q.y ) ); // pitch
	v.z = atan2f( 2.0f * q.x * q.y + 2.0f * q.z * q.w, 1 - 2.0f * ( q.y * q.y + q.z * q.z ) ); // roll
	return NormalizeAngles( v * Rad2Deg );
}

namespace plh_update_hook
{

	void ( *o_update )( uintptr_t inst );
	void update( uintptr_t inst )
	{
		auto main_callback = [ ]( ) {
			flyhack::run( );
			bunnyhop::run( );
			thirdperson::run( );
			antiaims::visual::run( );
			weapons::run( );
			chams::run( );


			auto players = entities::get_players( );
			if ( !players )
				return;

			auto local = entities::get_local( players );
			if ( !local )
				return;

			auto camera = c_camera::get_main( );

			globals::camera_position = variables::thirdperson ? local->object( )->bones( )->m_it[ 3 ]->get_position( ) :
				camera->get_transform( )->get_position( );

			esp::push( local, players, camera );
			silent::run( players, local->team( ) );
		};
		

		g_ctx.callbacks.add( { callback_type::attack, [ ]( ) {
		if ( silent::pos != vec3_t::Zero( ) )
		{
			auto my_pos = send_pos_hook::xxx;
			auto dir = silent::pos - globals::camera_position;

			auto ll = ToEulerRad( vec4_t::LookRotation( dir ) );

			float lxs = ll.x;
			float lys = ll.y;

			while ( lys < 0 )
			{
				lys += 360;
			}

			while ( lys > 360 )
			{
				lys -= 360;
			}

			bool untrusted_angle = ( lxs > 87 && lxs < 272 ) || lxs > 360 || lxs < 0;
			if ( untrusted_angle )
			{
				lxs = 87;
			}

			send_pos_hook::send_pos_o( send_attackv2_hook::inst, my_pos.x, my_pos.y, my_pos.z, lxs, lys, 1 );
			silent::pos = vec3_t::Zero( );
		}
        } } );

		main_callback( );

		g_ctx.callbacks.call( callback_type::main );
		g_ctx.callbacks.erase_all( callback_type::main );

		o_update( inst );
	}

	void hook( )
	{
		il2cpp_assembly::open( "Assembly-CSharp" )->image( )->get_class( "", "PLH" )->get_method( "Update", 0 )->hook<&update>(&o_update);
	}
}
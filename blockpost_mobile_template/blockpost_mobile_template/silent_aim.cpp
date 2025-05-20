#include "aim.hpp"

#include "esp.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

#include "vars.hpp"
#include "info.hpp"
#include "utils.hpp"
#include "c_camera.hpp"
#include "c_controll.hpp"
#include "c_controlltouch.hpp"
#include "mmath.hpp"
#include "render.hpp"
#include "globals.hpp"

#include <math.h>
#include <cmath>
#include <thread>
#include <mutex>

#define rad2deg(x) ((x) * (180.f / M_PI))

namespace silent
{
	float attached_rx, attached_ry;
	vec3_t pos;
	bool shoot;
	static bool visible( vec3_t left, vec3_t right, int lm )
	{
		struct RaycastHit
		{
			vec3_t Point, Normal;
			unsigned int FaceID;
			float Distance;
			float UV[ 2 ];
			int m_Collider;
		};
		RaycastHit hit = {};
		return reinterpret_cast< bool ( * )( vec3_t, vec3_t, RaycastHit*, int ) >( mem::game_assembly + 0x234FBF0 )( left, right, &hit, lm );
	}

	static c_player_data* select_player_crosshair( il2cpp_array<c_player_data*>* players, int local_team )
	{
		if ( !players )
			return nullptr;

		auto camera = c_camera::get_main( );
		if ( !camera )
			return nullptr;

		c_player_data* ret = nullptr;
		float max_distance = 1000.f;

		for ( const auto& player : *players )
		{
			if ( !player )
				continue;

			if ( player->health( ) < 1 )
				continue;

			if ( player->islocal( ) )
				continue;

			if ( variables::teamcheck )
			{
				if ( player->team( ) == local_team )
					continue;
			}

			auto head_pos = player->object( )->bones( )->to_vec( )[ 3 ]->get_position( );
			auto head_w2s = camera->ws( head_pos );

			if ( !head_w2s.checker )
				continue;

			if ( !math::in_fov( head_w2s.pos, variables::aim_fov ) )
				continue;

			if ( visible( globals::camera_position, head_pos, 512 ) )
				continue;

			float distance = math::distance_center( head_w2s.pos );
			if ( distance < max_distance )
			{
				max_distance = distance;
				ret = player;
			}
		}
		return ret;
	}

	static vec3_t calc_angle( const vec3_t& cam_pos, const vec3_t& world_pos )
	{
		vec3_t dir = world_pos - cam_pos;

		float dist = vec3_t::Distance( cam_pos, world_pos );

		float pitch = rad2deg( -asinf( dir.y / dist ) );

		if ( pitch < 0 )
		{
			pitch += 360;
		}

		return { dir.x, pitch, dir.z };
	}

	void run( il2cpp_array<c_player_data*>* players, int local_team )
	{
		if ( !variables::silent && !variables::psilent )
			return;

		if ( variables::autofire )
		{
			bool shooting = c_controll_touch::get_in_shoot( );

			if ( shooting == true )
			{
				c_controll_touch::set_in_shoot( false );
			}
		}

		auto target = select_player_crosshair( players, local_team );
		if ( !target )
			return;

		auto target_pos = target->object( )->bones( )->m_it[ 3 ]->get_position( );

		if ( variables::autoscope )
		{
			c_controll::set_in_zoom( true );
		}

		auto example = c_controll::get_ex_player( );
		if ( !example )
			return;

		auto player = example->character( );

		if ( variables::autostop )
		{
			player->move_input_vec( ) = vec3_t( 0, 0, 0 );
			player->_move_input_vec( ) = vec3_t( 0, 0, 0 );
		}

		if ( variables::psilent )
		{
			g_ctx.callbacks.add( { callback_type::render, [ player, target_pos ]( ) {
				auto calc = calc_angle( globals::camera_position, target_pos );
				player->look_input_vec( ) = vec3_t( calc.y, 0, 0 );
				player->_look_input_vec( ) = vec3_t( calc.x, 0, calc.z );
			} } );
		}

		if ( variables::silent )
		{
			pos = target_pos;
			shoot = true;
		}
		if ( variables::autofire )
		{
			bool shooting = c_controll_touch::get_in_shoot( );

			if ( shooting == true )
			{
				c_controll_touch::set_in_shoot( false );
			}

			c_controll_touch::set_in_shoot( true );
		}
	}
}
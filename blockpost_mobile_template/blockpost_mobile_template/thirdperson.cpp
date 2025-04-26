#include "thirdperson.hpp"

#include "entity.hpp"
#include "vars.hpp"
#include "c_controll.hpp"
#include "c_camera.hpp"
#include "c_collider.hpp"
#include "c_shooter.hpp"

namespace thirdperson
{
	bool linecast( vec3_t left, vec3_t right, float max, int lm, float *distance )
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
		bool hitted = reinterpret_cast< bool ( * )( vec3_t, vec3_t, RaycastHit*, float, int) >( mem::game_assembly + 0x23517E0 )( left, right, &hit, max, lm );
		*distance = hit.Distance;

		return hitted;
	}

	float collect_distance( )
	{
		auto cam_transform = c_camera::get_main( )->get_transform( );
		vec3_t cam_pos = cam_transform->get_position( );
		vec3_t cam_fwd = cam_transform->get_rotation( ) * vec3_t(0, 0, -1);
		float hit_distance = 0;
		if ( !linecast( cam_pos, vec3_t::Normalize(cam_fwd), variables::third_distance, 512, &hit_distance ) )
			return 9999;

		return hit_distance;
	}

	void run( )
	{
		if ( !variables::esp )
			return;

		auto local = entities::get_local( entities::get_players( ) );
		if ( !local )
			return;

		if ( local->health( ) < 1 )
			return;

		auto example = c_controll::get_ex_player( );
		if ( !example )
			return;

		auto camera = example->camera( );
		
		auto player = example->character( );

		auto pos = player->get_transform( )->get_position( );

		local->lerp_pos( ) = pos;
		local->prev_pos( ) = pos;
		local->pos( ) = pos;

		auto go = local->object()->go( );
		go->set_active( variables::thirdperson );

		auto type = c_type::get( "UnityEngine.Collider,UnityEngine.PhysicsModule" );
		auto components = go->get_components_in_children<c_collider*>( type, false );
		for ( const auto& comp : *components )
		{
			if ( !comp )
				continue;

			comp->set_enabled( !variables::thirdperson );
		}

		float target_distance = variables::thirdperson ? variables::third_distance : 0;
		
		float collected = collect_distance( );

		if ( collected < target_distance )
			target_distance = collected - 1;
		
		c_shooter::cs( )->hands_pos( ) = variables::thirdperson ? vec3_t{ 0, 666, 0 } : vec3_t{ 0, 0, 0 };
		camera->target_distance( ) = target_distance;
		//player->camera_follow_y( ) = 5.f;
	}
}


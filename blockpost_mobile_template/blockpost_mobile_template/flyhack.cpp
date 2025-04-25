#include "flyhack.hpp"

#include "Windows.h"
#include "vars.hpp"
#include "c_camera.hpp"
#include "c_controll.hpp"
#include "entity.hpp"

namespace flyhack
{
	void run( )
	{
		if ( !variables::flyhack )
			return;

		auto local = entities::get_local( entities::get_players( ) );

		auto example_player = c_controll::get_ex_player();
		if (!example_player)
			return;

		auto character = example_player->character();

		if ( !local || local->health( ) < 1 )
		{
			character->gravity().y = -30.f;
		}

		auto store = character->storepos( );
		if ( !store )
			return;

		auto urray = store->unity_array( );
		auto transpos = character->motor( )->transient_position( );
		for ( auto& item : urray->to_vec( ) )
		{
			item = { transpos.x, transpos.z, transpos.y };
		}

		auto camera = c_camera::get_main( );
		if ( !camera ) return;

		auto cameraTransform = camera->get_transform( );
		if ( !cameraTransform ) return;

		vec3_t forward = cameraTransform->get_forward( );
		vec3_t right = cameraTransform->get_right( );

		float forwardKey = GetAsyncKeyState( 0x57 ) ? 1.f : GetAsyncKeyState( 0x53 ) ? -1.f : 0.f;
		float rightKey = GetAsyncKeyState( 0x44 ) ? 1.f : GetAsyncKeyState( 0x41 ) ? -1.f : 0.f;
		float up_key = GetAsyncKeyState( VK_SPACE ) ? 1 : GetAsyncKeyState( VK_LCONTROL ) && GetAsyncKeyState( VK_LCONTROL ) ? -1 : 0;

		vec3_t addPos = vec3_t::Normalize( forward * forwardKey + right * rightKey + vec3_t( 0, 1, 0 ) * up_key );

		character->gravity( ) = { 0, 0, 0 };

		character->motor( )->base_velocity( ) = addPos * 8.f;
		static bool send;
		send = !send;
		character->send_in_air( ) = false;
		character->send_in_duck( ) = send;
	}
}
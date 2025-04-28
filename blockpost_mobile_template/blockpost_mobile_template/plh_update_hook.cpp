#include "plh_update_hook.hpp"
#include "plh_move_hook.hpp"
#include "minhook/mh.h"
#include "il2cpp.hpp"
#include "vars.hpp"

#include "aim.hpp"
#include "c_camera.hpp"
#include "c_controll.hpp"

#include "flyhack.hpp"
#include "bunnyhop.hpp"
#include "thirdperson.hpp"
#include "antiaims.hpp"
#include "weapons.hpp"
#include "chams.hpp"
#include "entity.hpp"
#include "globals.hpp"
#include "esp.hpp"

namespace plh_update_hook
{
	void ( *o_update )( uintptr_t inst );
	void update( uintptr_t inst )
	{
		flyhack::run( );
		bunnyhop::run( );
		thirdperson::run( );
		antiaims::visual::run( );
		weapons::run( );
		chams::run( );

		auto players = entities::get_players( );
		if ( players )
		{
			auto local = entities::get_local( players );
			if ( local )
			{
				auto camera = c_camera::get_main( );

				globals::camera_position = variables::thirdperson ? local->object( )->bones( )->m_it[ 3 ]->get_position( ) :
					camera->get_transform( )->get_position( );

				esp::push( local, players, camera );
			}
		}

		o_update( inst );
	}

	void hook( )
	{
		il2cpp_assembly::open( "Assembly-CSharp" )->image( )->get_class( "", "PLH" )->get_method( "Update", 0 )->hook<&update>(&o_update);
	}
}
#include "example_player_hook.hpp"
#include "c_example_player.hpp"
#include "globals.hpp"
#include "il2cpp.hpp"

namespace example_player_hook
{
	void ( *o_update )( c_example_player* player );
	void h_update( c_example_player* player )
	{
		g_ctx.callbacks.call( callback_type::extra );

		o_update( player );
	}

	void hook( )
	{
		il2cpp_assembly::open( "Assembly-CSharp" )->image( )->get_class( "KinematicCharacterController.Examples", "ExamplePlayer" )
			->get_method( "Update", 0 )->hook<&h_update>( &o_update );
	}
}
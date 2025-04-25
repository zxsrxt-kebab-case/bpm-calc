#include "bunnyhop.hpp"

#include "c_controll.hpp"
#include "esp.hpp"

#include <Windows.h>

#include "vars.hpp"

void bunnyhop::run( )
{
	if ( !variables::bunnyhop )
		return;

	auto example_player = c_controll::get_ex_player( );
	if ( !example_player )
		return;

	auto character = example_player->character( );
	if ( !character )
		return;

	
	if (variables::autostrafe)
	{
		character->drag() = 0.f;
		character->jump_scalable_forward() = 2.2f;
		character->stable_movement_sharpness() = 999.f;
	}
	
	character->jumped_this_frame() = false;
	if ( GetAsyncKeyState( VK_SPACE ) )
	{
		character->jump_requested( ) = true;
	}
}
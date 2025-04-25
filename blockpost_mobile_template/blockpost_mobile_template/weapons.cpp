#include "weapons.hpp"
#include "entity.hpp"
#include "vars.hpp"

c_weapon_object* weapons::my_weapon = nullptr;
void weapons::run( )
{
	if ( !variables::weapons::no_recoil || !variables::weapons::no_spread )
		return;

	weapons::no_recoil::run( );

	auto players = entities::get_players( );
	if ( !players )
	{
		my_weapon = nullptr;
		return;
	}

	auto local = entities::get_local( players );
	if ( !local )
	{
		my_weapon = nullptr;
		return;
	}

	auto shooter = c_shooter::cs( );
	if ( !shooter )
	{
		my_weapon = nullptr;
		return;
	}
	my_weapon = shooter->curwpn( );
}
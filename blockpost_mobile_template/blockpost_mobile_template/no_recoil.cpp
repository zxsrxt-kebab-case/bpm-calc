#include "weapons.hpp"
#include "vars.hpp"

void weapons::no_recoil::run( )
{
	if ( !weapons::my_weapon )
		return;

	int slot = weapons::my_weapon->slot( );
	if ( slot != 0 && slot != 1 )
		return;

	if ( variables::weapons::no_recoil )
	{
		weapons::my_weapon->recoil_pattern( ) = 0.f;
		weapons::my_weapon->recoil_vertical( ) = 0.f;
		weapons::my_weapon->recoil_horizontal( ) = 0.f;
	}
	if ( variables::weapons::no_spread )
	{
		weapons::my_weapon->spread_pattern( ) = 0.f;
		weapons::my_weapon->spread( ) = 0.0f;
	}
	//weapons::my_weapon->stability( ) = 10.f;
}
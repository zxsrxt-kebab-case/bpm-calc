#include "entity.hpp"
#include "il2cpp.hpp"
#include "constants.hpp"
#include <ranges>

il2cpp_array<c_player_data*>* entities::get_players( )
{
	static il2cpp_field* fld = il2cpp_assembly::open( "Assembly-CSharp" )->image( )->get_class( "", "PLH" )->get_field( constants::plh::player );
	if ( !fld ) return nullptr;
	return fld->get_static_value< il2cpp_array<c_player_data*>* >( );
}
c_player_data* entities::get_local( il2cpp_array<c_player_data*>* players )
{
	if ( !players )	return nullptr;

	auto it = std::ranges::find_if( *players, [ ]( c_player_data* p ) { if ( !p ) return false; return p->health( ) && p->islocal( ); } );
	if ( it != players->end( ) )
	{
		return *it;
	}
	return nullptr;
}
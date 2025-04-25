#include "chams.hpp"
#include "vars.hpp"
#include "entity.hpp"
#include "c_renderer.hpp"

void chams::enemy::run( )
{
	if ( !variables::chams::enemy::enable )
		return;

	static float ctr = 0.f;

	ctr += 0.016;

	if ( ctr < 0.2 )
		return;

	auto players = entities::get_players( );

	if ( !players )
		return;

	auto local = entities::get_local( players );

	if ( !local || local->health( ) < 1 )
		return;

	auto local_team = local->team( );

	static auto type = c_type::get( "UnityEngine.SkinnedMeshRenderer, UnityEngine.CoreModule" );
	static auto shader = c_shader::find( "Hidden/Internal-Colored" );
	static auto mat = c_material::create( shader );
	mat->set_color( "_Color", ImColor( 0, 255, 0, 170 ) );
	mat->set_int( "_ZWrite", 0 );
	mat->set_int( "_ZTest", 8 );

	if ( !type || !shader )
		return;

	for ( auto& player : *players )
	{
		if ( !player )continue;

		if ( player->health( ) < 1 )
			continue;

		if ( player->islocal( ) )
			continue;

		if ( player->team( ) == local_team )
			continue;

		auto renderers = player->object( )->go( )->get_components_in_children<c_renderer*>( type, false );
		if ( !renderers )
			continue;

		for ( auto& renderer : *renderers )
		{
			if ( !renderer )
				continue;

			renderer->set_allow_occlusion( false );
			
			renderer->set_material( mat );
		}
	}
	ctr = 0.f;
}
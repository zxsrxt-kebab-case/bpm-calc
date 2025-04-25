#include "chams.hpp"
#include "vars.hpp"
#include "entity.hpp"
#include "c_renderer.hpp"
#include "c_controll.hpp"

void chams::local::run( )
{
	if ( !variables::chams::local::enable )
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

	auto type = c_type::get( "UnityEngine.SkinnedMeshRenderer, UnityEngine.CoreModule" );
	auto shader = c_shader::find( "Hidden/Internal-Colored" );
	auto mat = c_material::create( shader );
	int alpha = c_controll::get_in_zoom( ) ? 40 : 150;
	mat->set_color( "_Color", ImColor( 0, 255, 0, alpha ) );
	mat->set_int( "_ZWrite", 0 );
	mat->set_int( "_ZTest", 8 );

	if ( !type || !shader || !mat )
		return;

	auto renderers = local->object( )->go( )->get_components_in_children<c_renderer*>( type, false );
	if ( !renderers )
		return;

	for ( auto& renderer : *renderers )
	{
		if ( !renderer )
			continue;

		renderer->set_allow_occlusion( false );

		renderer->set_material( mat );
	}
	ctr = 0.f;
}
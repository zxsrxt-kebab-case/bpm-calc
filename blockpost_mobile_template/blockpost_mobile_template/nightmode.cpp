#include "chams.hpp"
#include "vars.hpp"
#include "c_object.hpp"
#include "c_renderer.hpp"
#include "entity.hpp"

namespace chams::nightmode
{
	void run( )
	{
		if ( !variables::chams::nightmode::enable )
			return;

		static auto type = c_type::get( "UnityEngine.MeshRenderer, UnityEngine.CoreModule" );
		static auto shader = c_shader::find( "Standard" );

		il2cpp_array<c_renderer*>* renderers = nullptr;
		if ( !renderers )
		{
			renderers = c_object::find<c_renderer*>( type );
		}

		auto players = entities::get_players( );
		if ( !players )
			return;

		auto local = entities::get_local( players );
		if ( local->health( ) < 1 )
		{
			renderers = c_object::find<c_renderer*>( type );
			return;
		}

		if ( !renderers ) return;
		
		for ( auto object : *renderers )
		{
			if ( !object ) continue;

			auto materials = object->get_materials( );
			if ( !materials )continue;

			for ( auto mat : *materials )
			{
				if ( !mat ) continue;
				mat->set_shader( shader );
				mat->set_color( "_Color", ImColor( 70, 70, 70 ) );
				mat->set_texture( "_MainTex", NULL );
			}
			object->set_materials( materials );
		}
	}
}
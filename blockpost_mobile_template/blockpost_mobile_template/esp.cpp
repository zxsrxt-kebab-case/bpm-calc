#include "esp.hpp"

#include "vars.hpp"
#include "il2cpp.hpp"
#include "entity.hpp"

#include "c_controll.hpp"

#include "imgui/imgui.h"
#include "minhook/mh.h"

#include "render.hpp"
#include "constants.hpp"

#include <string>
#include <utility>
#include <cmath>

#include "globals.hpp"

namespace esp
{
	static void draw_single( c_player_data* player, c_camera* camera )
	{
		if ( !player )
			return;

		auto health = player->health( );

		auto player_object = player->object( );

		auto trans = player_object->m_tr( );
		auto head = player_object->bones( )->m_it[ 3 ];

		auto pos_bot = trans->get_position( );
		auto pos_top = head->get_position( );

		auto bot = camera->ws( pos_bot );
		auto top = camera->ws( pos_top + vec3_t{ 0, 0.4, 0 } );

		if ( !bot.checker || !top.checker )
			return;

		float height = fabsf( top.pos.y - bot.pos.y );
		float width = fabsf( height / 2.8 );

		ImRect rect = {
			{( float )static_cast< int >( top.pos.x - width ), ( float )static_cast< int >( top.pos.y )},
			{( float )static_cast< int >( bot.pos.x + width ), ( float )static_cast< int >( bot.pos.y )}
		};

		auto m_name = player->get_name( )->get_string( );
		std::string m_wname = "";
		auto codename = player->curwpn( )->it( )->codename( );
		if ( codename )
		{
			m_wname = codename->get_string( );
		}

		//box
		if ( variables::elements::box ) g_ctx.callbacks.add( { callback_type::render, [ rect ]( ) {render::fun::box( rect, ImColor( 255, 255, 255 ), true ); } } );

		//health
		if ( variables::elements::health ) g_ctx.callbacks.add( { callback_type::render, [ rect, health ]( ) {
			render::fun::health( rect, health, ImColor( 255, 255, 255 ), true ); } } );

		//nickname
		if ( variables::elements::name )g_ctx.callbacks.add( { callback_type::render, [ rect, m_name ]( ) {
			render::fun::nickname( rect, m_name, ImColor( 255, 255, 255 ), true ); } } );

		//weapon
		if ( variables::elements::weapon ) g_ctx.callbacks.add( { callback_type::render,  [ rect, m_wname ]( ) {
			render::fun::weapon( rect, m_wname, ImColor( 255, 255, 255 ), true ); } } );

	}

	void push( c_player_data* local, il2cpp_array<c_player_data*>* players, c_camera* camera )
	{
		if ( !camera || !players || !local )
			return;

		int local_team = local->team( );

		for ( const auto& player : *players )
		{
			if ( !player )
				continue;

			if ( !player->health( ) )
				continue;

			if ( player->islocal( ) )
				continue;

			if ( variables::teamcheck )
			{
				if ( player->team( ) == local_team )
					continue;
			}

			draw_single( player, camera );
		}
	}

	void run( )
	{

	}
}
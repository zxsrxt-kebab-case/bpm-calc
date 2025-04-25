#include "esp.hpp"

#include "vars.hpp"
#include "il2cpp.hpp"
#include "c_array.hpp"
#include "entity.hpp"

#include "c_camera.hpp"
#include "c_player_data.hpp"
#include "c_controll.hpp"

#include "imgui/imgui.h"
#include "minhook/mh.h"

#include "render.hpp"
#include "constants.hpp"

#include <string>
#include <utility>
#include <cmath>

namespace esp
{
	void run( )
	{
		if ( !variables::esp )
			return;

		auto players = entities::get_players( );
		if ( !players )
			return;

		auto local = entities::get_local( players );
		if ( !local )
			return;

		auto camera = c_camera::get_main( );
		if ( !camera )
			return;

		int local_team = local->team( );

		for ( const auto& player : *players)
		{
			if ( !player )
				continue;

			auto health = player->health( );
			if ( !health )
				continue;

			if ( player->islocal( ) )
				continue;

			if ( variables::teamcheck )
			{
				if ( player->team( ) == local_team )
					continue;
			}

			auto player_object = player->object( );

			auto tran = player_object->m_tr( );
			auto head = player_object->bones( )->to_vec()[ 3 ];

			auto pos_bot = tran->get_position( );
			auto pos_top = head->get_position( );

			auto bot = camera->ws( pos_bot );
			auto top = camera->ws( pos_top + vec3_t{ 0, 0.4, 0 } );

			if ( !bot.checker || !top.checker )
				continue;

			float height = fabsf( top.pos.y - bot.pos.y );
			float width = fabsf( height / 2.8 );

			ImRect rect = {
				{( float )static_cast< int >( top.pos.x - width ), ( float )static_cast< int >( top.pos.y )},
				{( float )static_cast< int >( bot.pos.x + width ), ( float )static_cast< int >( bot.pos.y )}
			};

			auto m_name = player->get_name( )->get_string( );
			auto m_wname = player->curwpn( )->it( )->codename( )->get_string( );
			esp::draw::box( rect, ImColor( 255, 255, 255 ), true );
			esp::draw::health( rect, health, player->armor( ), ImColor( 255, 255, 255 ), true );
			esp::draw::nickname( rect, m_name, ImColor( 255, 255, 255 ), true );
			esp::draw::weapon( rect, m_wname, ImColor( 255, 255, 255 ), true );
		}
	}
}
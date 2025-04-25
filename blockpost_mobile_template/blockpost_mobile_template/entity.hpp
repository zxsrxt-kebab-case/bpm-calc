#pragma once
#include "c_player_data.hpp"
#include "c_array.hpp"

namespace entities
{
	extern c_array<c_player_data*>* get_players( );
	extern c_player_data* get_local( c_array<c_player_data*>* players );
}
#pragma once
#include "c_player_data.hpp"
#include "il2cpp_array.hpp"

namespace entities
{
	extern il2cpp_array<c_player_data*>* get_players( );
	extern c_player_data* get_local( il2cpp_array<c_player_data*>* players );
}
#pragma once
#include "vec3.hpp"
#include "entity.hpp"

namespace aim
{
	extern void run( );
}
namespace silent
{
	extern float attached_rx, attached_ry;
	extern void run( il2cpp_array<c_player_data*>* players, int local_team );
	extern bool shoot;
	extern vec3_t pos;
}
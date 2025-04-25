#include "plh_move_hook.hpp"
#include "antiaims.hpp"
#include "entity.hpp"
#include "minhook/mh.h"
#include "mem.hpp"
#include "vec3.hpp"
#include "vars.hpp"

namespace plh_move_hook
{
	move_arg mv;
	void (*od_move)(void* plh, int id, vec3_t pos, float rx, float ry, int bitmask) = nullptr;
	void hk_move(void* plh, int id, vec3_t pos, float rx, float ry, int bitmask)
	{
		auto local = entities::get_local( entities::get_players( ) );
		if ( local )
		{
			if ( variables::antiaims::enable && variables::antiaims::type == 0 && id == local->id( ) )
			{
				antiaims::visual::in_rx = rx;
				antiaims::visual::in_ry = ry;

				rx = antiaims::visual::rx;
				ry = antiaims::visual::ry;
			}
		}
		//mv = { pos, rx, ry, bitmask };
		od_move(plh, id, pos, rx, ry, bitmask);
	}

	void hook()
	{
		auto addr = reinterpret_cast<LPVOID>(mem::game_assembly + 0x6B2160 );
		MH_CreateHook(addr, hk_move, (void**) &od_move);
		MH_EnableHook(addr);
	}
}
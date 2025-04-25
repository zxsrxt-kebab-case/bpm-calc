#include "send_pos_hook.hpp"

#include "mem.hpp"
#include "vars.hpp"
#include "aim.hpp"
#include "constants.hpp"
#include "minhook/mh.h"
#include "Windows.h"
#include <random>

namespace send_pos_hook
{
	void (*send_pos_o)(uintptr_t client, float xx, float xy, float xz, float lx, float ly, unsigned char bitmask);
	void send_pos_h(uintptr_t client, float xx, float xy, float xz, float lx, float ly, unsigned char bitmask)
	{
		if ( variables::flyhack || variables::antiaims::fakeduck )
		{
			static bool loading = false;
			if ( loading )
			{
				int random = rand( ) % 64 + 79;
				bitmask = random;
			}
			else
			{
				int random = rand( ) % 30 + 0;
				bitmask = random;
			}

			static float delta;
			delta += 0.016f;
			if ( delta >= 0.1f )
			{
				loading = !loading;
				delta = 0.0f;
			}
		}
		silent::attached_rx = lx;
		silent::attached_ry = ly;
		send_pos_o(client, xx, xy, xz, lx, ly, bitmask);
	}

	void hook()
	{
		auto addr = reinterpret_cast< LPVOID >( mem::game_assembly + constants::client::send_pos );
		MH_CreateHook( addr, reinterpret_cast<LPVOID>(send_pos_h),
			reinterpret_cast<LPVOID*>(&send_pos_o));
		MH_EnableHook(addr);
	}
}
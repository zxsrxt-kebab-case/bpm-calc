#pragma once

#include "vec3.hpp"

namespace variables
{
	extern bool esp;
	extern bool teamcheck;
	extern bool silent;
	extern bool psilent;
	extern bool autofire;
	extern bool autostop;
	extern bool autoscope;
	extern int aim_fov;
	extern bool flyhack;
	extern bool bunnyhop;
	extern bool autostrafe;
	extern bool thirdperson;
	extern float third_distance;
	namespace weapons
	{
		extern bool no_recoil;
		extern bool no_spread;
	}
	namespace chams
	{
		namespace enemy
		{
			extern bool enable;
		}
		namespace local
		{
			extern bool enable;
		}
	}
	namespace antiaims
	{
		extern bool enable;
		extern int type;
		namespace visual
		{
			enum class yaw_type_en
			{
				yaw_static,
				yaw_jitter,
				yaw_spin
			};			
			enum class pitch_type_en
			{
				pitch_static,
				pitch_jitter
			};
			extern yaw_type_en yaw_type;
			extern pitch_type_en pitch_type;

			extern int static_yaw;
			extern int static_pitch;
			extern int jitter_yaw;
			extern int jitter_pitch;
			extern int spin_yaw;
		}
		extern bool fakeduck;
	}
}
namespace globals
{
	extern vec3_t camera_position;
}
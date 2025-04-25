#include "vars.hpp"

namespace variables
{
	bool esp = false;
	bool teamcheck = false;
	bool silent = false;
	bool psilent = false;
	bool autofire = false;
	bool autostop = false;
	bool autoscope = false;
	int aim_fov = 60;
	bool flyhack = false;
	bool bunnyhop = false;
	bool autostrafe = false;
	bool thirdperson = false;
	float third_distance = 1.f;
	namespace weapons
	{
		bool no_recoil = false;
		bool no_spread = false;
	}
	namespace chams
	{
		namespace enemy
		{
			bool enable = false;
		}
		namespace local
		{
			bool enable = false;
		}
	}
	namespace antiaims
	{
		bool enable = false;
		int type = 0;
		namespace visual
		{
			yaw_type_en yaw_type = yaw_type_en::yaw_static;
			pitch_type_en pitch_type = pitch_type_en::pitch_static;
			int static_yaw = 0;
			int static_pitch = 0;
			int jitter_yaw = 0;
			int jitter_pitch = 0;
			int spin_yaw = 0;
		}
		bool fakeduck;
	}
	
}

namespace globals
{
	vec3_t camera_position = vec3_t::Zero( );
}
#include "antiaims.hpp"

#include "mem.hpp"
#include "vars.hpp"
#include "minhook/mh.h"
#include <mutex>

namespace antiaims::visual
{
	float in_rx, in_ry;
	float rx, ry;

	namespace var = variables::antiaims;
	namespace varv = variables::antiaims::visual;

	void run()
	{
		if ( !var::enable || var::type != 0 )
			return;

		if ( varv::yaw_type == varv::yaw_type_en::yaw_static )
		{
			float end = in_ry - varv::static_yaw;
			if ( end > varv::static_yaw )
			{
				end -= varv::static_yaw * 2;
			}
			else if ( end < -varv::static_yaw )
			{
				end += varv::static_yaw * 2;
			}
			ry = end;
		}		
		if ( varv::pitch_type == varv::pitch_type_en::pitch_static )
		{
			rx = (float)varv::static_pitch;
		}
		if ( varv::yaw_type == varv::yaw_type_en::yaw_jitter )
		{
			float end = in_ry - 180;
			if ( end > 180.f )
			{
				end -= 360.f;
			}
			else if ( end < -180.f )
			{
				end += 360.f;
			}

			bool should_switch = false;
			if ( !should_switch )
			{
				ry = end + varv::jitter_yaw;
				should_switch = true;
			}
			if ( should_switch )
			{
				ry = end - varv::jitter_yaw;
				should_switch = false;
			}
			//ry = end;
		}
	}
}


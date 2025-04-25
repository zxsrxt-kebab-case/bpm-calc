#pragma once

#include <cstdint>

namespace dx_hook
{
	enum status
	{
		err = -1,
		done = 0
	};
	status init( );
	status hook( uint16_t index, void** orig, void* fn);

}


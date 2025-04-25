#include "mmath.hpp"
#include "info.hpp"
#include <cmath>

namespace math
{
    float dot( const vec3_t& left, const vec3_t& right )
    {
        return left.x * right.x + left.y * right.y + left.z * right.z;
    }
	bool in_fov( ImVec2 pss, int fov )
	{
		return ( std::powf( info::screen.x / 2 - pss.x, 2 ) + std::powf( info::screen.y / 2 - pss.y, 2 ) ) <= std::powf( fov, 2 );
	}
	float distance_center( ImVec2 pss )
	{
		return std::sqrtf( std::powf( info::screen.x / 2 - pss.x, 2 ) + std::powf( info::screen.y / 2 - pss.y, 2 ) );
	}
}
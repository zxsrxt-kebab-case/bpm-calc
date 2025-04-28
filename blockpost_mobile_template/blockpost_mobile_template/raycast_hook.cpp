#include "raycast_hook.hpp"

#include "mem.hpp"
#include "aim.hpp"
#include "vec3.hpp"
#include "il2cpp.hpp"
#include <thread>

namespace raycast_hook
{
	struct ray_t
	{
		vec3_t m_orig;
		vec3_t m_dir;
	};
	struct raycast_hit_t
	{
		vec3_t m_point;
		vec3_t m_orig;
		uint32_t m_face;
		float m_distance;
		float m_uv[ 2 ];
		int m_collider;
	};
	bool( *o_raycast )( void*& scene, ray_t& ray, float max_distance, raycast_hit_t& hit, int lm, int interact );
	bool h_raycast( void*& scene, ray_t& ray, float max_distance, raycast_hit_t& hit, int lm, int interact )
	{
		if ( silent::shoot )
		{
			ray.m_dir = silent::pos - ray.m_orig;
			silent::shoot = false;
		}
		return o_raycast( scene, ray, max_distance, hit, lm, interact );
	}
	void hook( )
	{
		std::thread( [ ]( ) -> int {
			while ( true )
			{
				auto addr = *reinterpret_cast< uintptr_t* >( mem::game_assembly + 0x2C545B0 );
				if ( !addr )
				{
					std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );
					continue;
				}
				il2cpp_icall::hook<&h_raycast>( mem::game_assembly + 0x2C545B0, &o_raycast );
				break;
			}
			return 0;
					 } ).detach( );
	}
}	
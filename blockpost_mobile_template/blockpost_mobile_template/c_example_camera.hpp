#pragma once

class c_example_camera
{
public:
	float& target_distance( )
	{
		return *reinterpret_cast< float* >( ( uintptr_t ) this + 0x90 );
	}
	float& target_vertical_angle( )
	{
		return *reinterpret_cast< float* >( ( uintptr_t ) this + 0x9C );
	}
};
#pragma once
class c_kinematic_character
{
public:
	void** get_character_interface( )
	{
		return *reinterpret_cast< void*** >( (uintptr_t)this + 0x1B0 );
	}
	vec3_t& transient_position( )
	{
		return *reinterpret_cast< vec3_t* >( ( uintptr_t ) this + 0x100 );
	}	
	vec3_t& base_velocity( )
	{
		return *reinterpret_cast< vec3_t* >( ( uintptr_t ) this + 0x1E8 );
	}

	struct character_report
	{
		bool f;
		bool s;
		bool t;
		vec3_t ff;
		vec3_t ss;
		vec3_t tt;
		uintptr_t ptr;
		vec3_t fout;
	};
	character_report& rep( )
	{
		return *reinterpret_cast< character_report* >( ( uintptr_t ) this + 0x88 );
	}
};
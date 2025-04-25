#pragma once
#include "common.hpp"
#include "il2cpp_class.hpp"

class il2cpp_image
{
public:
	int get_class_count( );
	il2cpp_class* get_class( int idx );
	il2cpp_class* get_class( std::string nmspc, std::string name );
};


#pragma once
#include "common.hpp"
#include "il2cpp_image.hpp"

class il2cpp_assembly
{
public:
	static il2cpp_assembly* open( std::string ass );
	il2cpp_image* image( );
};


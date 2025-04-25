#pragma once
#include "common.hpp"
#include "il2cpp_field.hpp"
#include "il2cpp_method.hpp"
class il2cpp_class
{
private:
	il2cpp_field* get_fields_internal( void** iter );
	il2cpp_method* get_methods_internal( void** iter );

public:
	std::string get_name( );
	std::string get_namespace( );
	std::vector<il2cpp_field*> get_fields( );
	std::vector<il2cpp_method*> get_methods( );
	il2cpp_field* get_field( std::string name );
	il2cpp_method* get_method( std::string name, int params = 0 );
	il2cpp_method* get_method( std::string name, std::initializer_list<std::string> params = { } );
};


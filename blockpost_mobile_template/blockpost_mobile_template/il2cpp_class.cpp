#include "il2cpp_class.hpp"

il2cpp_field* il2cpp_class::get_fields_internal( void** iter )
{
	return reinterpret_cast< il2cpp_field * ( * )( il2cpp_class*, void** ) >( IAPI( "il2cpp_class_get_fields" ) )( this, iter );
}
il2cpp_method* il2cpp_class::get_methods_internal( void** iter )
{
	return reinterpret_cast< il2cpp_method * ( * )( il2cpp_class*, void** ) >( IAPI( "il2cpp_class_get_methods" ) )( this, iter );
}

std::string il2cpp_class::get_name( )
{
	return std::string( reinterpret_cast< const char* ( * ) ( il2cpp_class* ) >( IAPI( "il2cpp_class_get_name" ) )( this ) );
}
std::string il2cpp_class::get_namespace( )
{
	return std::string( reinterpret_cast< const char* ( * ) ( il2cpp_class* ) >( IAPI( "il2cpp_class_get_namespace" ) )( this ) );
}

std::vector<il2cpp_field*> il2cpp_class::get_fields( )
{
	std::vector< il2cpp_field* > ret;

	void* iter = nullptr;
	while ( auto field = get_fields_internal( &iter ) )
	{
		ret.push_back( field );
	}

	return ret;
}
il2cpp_field* il2cpp_class::get_field( std::string name )
{
	for ( const auto& field : get_fields( ) )
	{
		if ( field->get_name( ) != name )
			continue;

		return field;
	}
	return reinterpret_cast< il2cpp_field * ( * )( il2cpp_class*, const char* ) >( IAPI( "il2cpp_class_get_field_from_name" ) )( this, name.c_str( ) );
}

std::vector<il2cpp_method*> il2cpp_class::get_methods( )
{
	std::vector< il2cpp_method* > ret;

	void* iter = nullptr;
	while ( auto method = get_methods_internal( &iter ) )
	{
		if(std::find( ret.begin( ), ret.end( ), method ) == ret.end( ) )
			ret.push_back( method );
	}

	return ret;
}
il2cpp_method* il2cpp_class::get_method( std::string name, int params )
{
	return reinterpret_cast< il2cpp_method * ( * )( il2cpp_class*, const char*, int ) >( IAPI( "il2cpp_class_get_method_from_name" ) )( this, name.c_str( ), params );
}
il2cpp_method* il2cpp_class::get_method( std::string name, std::initializer_list<std::string> params )
{
	if ( params.size( ) == 0 )
		return get_method( name, 0 );
	
	auto methods = get_methods( );

	auto method_matches = [ &name, &params ]( il2cpp_method* method ) {
		if ( method->get_name( ) != name )
			return false;

		uint32_t param_count = method->get_param_count( );
		if ( param_count != params.size( ) )
			return false;

		auto param_it = params.begin( );
		for ( uint32_t idx = 0; idx < param_count; ++idx, ++param_it )
		{
			if ( method->get_param_name( idx ) != *param_it )
				return false;
		}

		return true;
		};

	auto it = std::ranges::find_if( methods, method_matches );

	if ( it != methods.end( ) )
	{
		return *it;
	}

	return nullptr;
}
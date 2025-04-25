#include "il2cpp_image.hpp"

int il2cpp_image::get_class_count( )
{
	return reinterpret_cast< int ( * )( il2cpp_image* ) >( IAPI( "il2cpp_image_get_class_count" ) )( this );
}

il2cpp_class* il2cpp_image::get_class( int idx )
{
	return reinterpret_cast< il2cpp_class * ( * )( il2cpp_image*, int ) >( IAPI( "il2cpp_image_get_class" ) )( this, idx );
}

il2cpp_class* il2cpp_image::get_class( std::string nmspc, std::string name )
{
	return reinterpret_cast< il2cpp_class * ( * )( il2cpp_image*, const char*, const char* ) >( IAPI( "il2cpp_class_from_name" ) )( this, nmspc.c_str( ), name.c_str( ) );
}
#include "il2cpp_delegate.hpp"

uintptr_t& il2cpp_delegate::get_method_pointer( )
{
	return *reinterpret_cast< uintptr_t* >( ( uintptr_t ) this + 0x10 );
}
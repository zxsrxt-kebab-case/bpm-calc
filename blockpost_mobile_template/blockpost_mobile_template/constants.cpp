#include "constants.hpp"

namespace constants
{
	namespace plh
	{
	#ifdef LAUNCHER
		const char* player = "DCEBMNIKPOA";
	#endif
	#ifdef STEAM
		const char* player = "NDJNKIDKLLG";
	#endif
	}
	namespace controll
	{
	#ifdef LAUNCHER
		const char* in_zoom = "CBBBPNHDCIG";
		const char* ex_player = "LOKAGDCGAFM";
	#endif
	#ifdef STEAM
		const char* in_zoom = "DCEBMNIKPOA";
		const char* ex_player = "LOKAGDCGAFM";
	#endif
	}
	namespace shooter
	{
	#ifdef LAUNCHER
		const char* cs = "JIMJMHAJMNN";
	#endif
	#ifdef STEAM
		const char* cs = "JIMJMHAJMNN";
	#endif
	}
	namespace client
	{
	#ifdef LAUNCHER
		const ptrdiff_t send_pos = 0xFCD430;
	#endif
	#ifdef STEAM
		const ptrdiff_t send_pos = 0x1044050;
	#endif
	}
}
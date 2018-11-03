#pragma once

#ifdef ST_PLATFORM_WINDOWS
	#ifdef ST_BUILD_DLL
		#define STRATA_API __declspec(dllexport)
	#else 
		#define STRATA_API __declspec(dllimport)
	#endif
#else
	#define STRATA_API
#endif
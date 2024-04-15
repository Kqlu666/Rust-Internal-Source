#pragma once
#include "../vcruntime/constant/xor.hpp"
#include "../util/util.hpp"

#ifdef DEBUG
#define TRACE( fmt, ... ) \
  printf( _( "%s:%d() >>\t%s\n" ), _( __FUNCTION__ ), __LINE__, util::format( fmt, __VA_ARGS__ ) )
#else
#define TRACE( fmt, ... )
#endif

#define PAUSE( ) getchar( );

namespace logger {
	bool initialize( );
} // namespace logger

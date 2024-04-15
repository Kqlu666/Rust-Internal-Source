#include "logger.hpp"
#include "../vcruntime/safe/safe.hpp"
#include "../util/util.hpp"

namespace logger {
    bool initialize( ) {
        proc_call( AllocConsole )( );
        freopen( _( "CONOUT$" ), _( "w" ), stdout );
        freopen( _( "CONIN$" ), _( "r" ), stdin );
        return true;
    }
} // namespace logger

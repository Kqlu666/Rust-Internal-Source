#include "safe.hpp"

namespace mem {

    void copy( void* buffer, void* address, size_t size ) {
        memcpy( buffer, address, size );
    }
} // namespace mem

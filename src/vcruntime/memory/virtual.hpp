#pragma once
#include <unordered_map>

#include "../safe/safe.hpp"

namespace memory {
    class c_virtual_hook {
        uint8_t* _original = nullptr;
        uint8_t* _procedure = nullptr;
        uint8_t* _replace = nullptr;

        bool _initialized = false;

    public:
        c_virtual_hook( ) = default;

        template < class T, typename _ty2 >
        __forceinline c_virtual_hook( T procedure, _ty2 replace ) {
            _procedure = mem::read< uint8_t* >( procedure );

            if ( !_procedure )
                return;

            _replace = reinterpret_cast< uint8_t* >( replace );

            if ( !_replace )
                return;

            _original = reinterpret_cast< uint8_t* >( procedure );

            if ( !_original )
                return;

            mem::write< _ty2 >( procedure, replace );

            _initialized = true;
        }

        template < class T >
        __forceinline auto call( T fn ) {
            return T( get( ) );
        }

        __forceinline auto unhook( ) {
            mem::write< uint8_t* >( _replace, _procedure );
            _initialized = false;
            return true;
        }

        auto get( ) { return _procedure; }
        auto& intialized( ) { return _initialized; }
    };
} // namespace memory

struct hooks_t {

    template < class T >
    memory::c_virtual_hook& get( T hk_addr ) {
        return *_hooks.at( reinterpret_cast< uintptr_t >( hk_addr ) );
    }

    template < class T >
    T call( T hk_addr ) {
        return _hooks.at( reinterpret_cast< uintptr_t >( hk_addr ) )->call( hk_addr );
    }

    template < class T, typename _ty2 >
    memory::c_virtual_hook& add( T procedure, _ty2 replace ) {

        for ( auto& hk : reserve ) {
            if ( hk.intialized( ) )
                continue;

            hk = { procedure, replace };
            _hooks.insert( { reinterpret_cast< uintptr_t >( replace ), &hk } );
            break;
        }

        return *_hooks.at( reinterpret_cast< uintptr_t >( replace ) );
    }

    void destroy_all( ) {
        for ( auto& hook : _hooks )
            hook.second->unhook( );
    }

private:
    std::array< memory::c_virtual_hook, 30 > reserve = { };
    std::unordered_map< uintptr_t, memory::c_virtual_hook* > _hooks;
};


static hooks_t g_hooks;

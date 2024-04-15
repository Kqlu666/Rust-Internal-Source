#pragma once
#include "../pe/image.hpp"
#include "../../util/hde/hde64.hpp"
#include "../constant/xor.hpp"

#include "../memory/operation.hpp"
#include "../memory/scan.hpp"

extern "C" void* system_call_stub( );

class c_safe {

    uintptr_t m_sys_gadget_addr = 0;
    uint32_t m_bad_index = std::numeric_limits< uint32_t >::max( );

public:
    __forceinline bool initialize( ) {
        auto nt_allocate_virtual_memory = pe::get_procedure< uintptr_t >( HASH( "NtAllocateVirtualMemory" ) );
        if ( !memory::is_valid( nt_allocate_virtual_memory ) )
            return false;

        m_sys_gadget_addr = nt_allocate_virtual_memory + 0x12;

        return true;
    }

    template < typename result_t, class T, typename... args_t >
    __forceinline result_t call( const T& procedure, args_t... args_pack ) {
        const auto fn = _call< result_t( * )( args_t... ) >( reinterpret_cast< uintptr_t >( procedure ) );
        return fn( args_pack... );
    }

    template < typename _fn, class T >
    __forceinline _fn call_fn( const T& procedure ) {
        const auto fn = _call< _fn >( uintptr_t( procedure ) );
        return fn;
    }

    template < typename result_t = NTSTATUS, typename proc = uintptr_t, typename... args_t >
    __forceinline result_t syscall( const proc& procedure, args_t... args_pack ) {
        const auto fn = _syscall< result_t( * )( args_t... ) >( reinterpret_cast< uintptr_t >( procedure ) );
        return fn( args_pack... );
    }

    template < typename _fn, typename proc = uintptr_t >
    __forceinline _fn syscall( const proc& procedure ) {
        const auto fn = _syscall< _fn >( uintptr_t( procedure ) );
        return fn;
    }

    template < class T >
    __forceinline T _syscall( hash_t procedure ) {

        auto index = get_system_index( procedure );
        if ( index == m_bad_index )
            return nullptr;

        __writegsdword( 0x000, index );
        __writegsqword( 0x150, m_sys_gadget_addr );

        return _call< T >( reinterpret_cast< uintptr_t >( &system_call_stub ) );
    }

    template < class T >
    __forceinline T _syscall( uint32_t index ) {

        if ( index == m_bad_index )
            return nullptr;

        __writegsdword( 0x000, index );
        __writegsqword( 0x150, m_sys_gadget_addr );

        return _call< T >( reinterpret_cast< uintptr_t >( &system_call_stub ) );
    }

    const uint32_t get_system_index( hash_t procedure );

    template < class T >
    __forceinline uint32_t get_vidx( T procedure ) {
        static uint32_t virtual_index = m_bad_index;

        if ( virtual_index == m_bad_index ) {
            union {
                T unknown;
                uint8_t* pointer;
            };

            unknown = procedure;

            if ( memory::is_valid( pointer ) ) {
                constexpr auto opcode_count = 0x0005;
                constexpr auto opcode_subtract = 0x0002;

                const auto opcode = *reinterpret_cast< uint16_t* >( &pointer[ opcode_count - opcode_subtract ] );

                if ( opcode == 0x60FF ) {
                    virtual_index = *reinterpret_cast< uint8_t* >( &pointer[ opcode_count ] );
                } else if ( opcode == 0xA0FF ) {
                    virtual_index = *reinterpret_cast< uint32_t* >( &pointer[ opcode_count ] );
                } else if ( opcode == 0x20FF ) {
                    virtual_index = reinterpret_cast< uint32_t >( nullptr );
                }

                if ( virtual_index == m_bad_index )
                    return m_bad_index;

                virtual_index /= sizeof( std::uint64_t );
            }
        }

        return virtual_index;
    }
    template < class T >
    __forceinline void* get_vproc( void* object, T procedure ) {
        static bool created = false;

        static uint8_t* opcode_stub = nullptr;

        if ( !created )
        {
            opcode_stub = ( uint8_t* )VirtualAlloc( nullptr, 32, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE );
            created = true;
        }

        for ( auto i = 0; i < 32; ++i )
            opcode_stub[ i ] = 0x00;

        if ( opcode_stub[ 0 ] == 0x00 ) {
            auto virtual_index = get_vidx( procedure );
            auto virtual_table = *reinterpret_cast< LPVOID** >( object );

            auto opcode_size = 0;

            for ( auto i = 0; i < 1; i++ ) {
                const auto address = reinterpret_cast< uintptr_t >( virtual_table[ virtual_index ] );
                const auto pointer = reinterpret_cast< LPCVOID >( address + opcode_size );

                memcpy( &opcode_stub[ opcode_size ], pointer, sizeof_opcode( pointer ) );
                opcode_size += sizeof_opcode( virtual_table[ virtual_index + opcode_size ] );
            }

            *( uint16_t* )( &opcode_stub[ opcode_size ] ) = 0x25FF;
            *( uint32_t* )( &opcode_stub[ opcode_size + 2 ] ) = 0x0000;
            *( uint64_t* )( &opcode_stub[ opcode_size + 6 ] )
                = reinterpret_cast< uint64_t >( virtual_table[ virtual_index ] ) + opcode_size;

            // TRACE( "opcode_size is '%d'", FN, opcode_size );
        }

        return reinterpret_cast< void* >( &opcode_stub[ 0 ] );
    }

private:
    template < class T >
    __forceinline T _call( uintptr_t procedure ) {
        return T( procedure );
    }

    const uint32_t get_system_index( uintptr_t image, hash_t procedure );
};

inline c_safe safe;

#define get_proc( proc ) pe::get_procedure( HASH( #proc ) )

#define proc_call( proc )             safe.call_fn< decltype( &proc ) >( pe::get_procedure( HASH( #proc ) ) )
#define _proc_call( type, proc, ... ) safe.call< type >( pe::get_procedure( HASH( #proc ) ), __VA_ARGS__ )

#define vcall( Type, Object, Name, ... ) \
  Object->Name( __VA_ARGS__ )
  //safe.call< Type >(                     \
  //    safe.get_vproc( Object, &std::remove_pointer< decltype( Object ) >::type::Name ), Object, __VA_ARGS__ )
     

#define il2cpp_call( proc ) safe.call_fn< il2cpp_##proc >( proc )

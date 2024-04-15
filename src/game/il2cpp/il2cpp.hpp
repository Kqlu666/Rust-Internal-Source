#pragma once
#include <vector>
#include <codecvt>
#include <algorithm>

#include "../../renderer/renderer.hpp"
#include "../../vcruntime/safe/safe.hpp"
#include "../../util/logger.hpp"

#define create_type( name, args ) \
  using il2cpp_##name = args;       \
  inline il2cpp_##name name;

#define assign_type( name ) name = pe::get_procedure< il2cpp_##name >( game_assembly, HASH( "il2cpp_" #name ) );

#define il2cpp_method( fn_name, ns, klass_name, name, arg_count, type ) \
  static const auto fn_name = mem::read< type >( il2cpp::get_method< type >( il2cpp::get_class( ns, klass_name ), name, arg_count ) )

#define il2cpp_icall( fn_name, name, type ) \
  static const auto fn_name = reinterpret_cast< type >( il2cpp::resolve_icall( name ) )

#define il2cpp_field_get( type, fl_name, ns, klass_name, name )                           \
  inline type fl_name( ) {                                                              \
    static const auto off = il2cpp::get_field( il2cpp::get_class( ns, klass_name ), name ); \
    if ( !off )                                                                         \
      return { };                                                                       \
    return mem::read< type >( this + off );                                             \
  }

#define il2cpp_field_set( type, fl_name, ns, klass_name, name )                           \
  inline auto fl_name( type value ) {                                                   \
    static const auto off = il2cpp::get_field( il2cpp::get_class( ns, klass_name ), name ); \
    if ( !off )                                                                         \
      return;                                                                           \
    mem::write< type >( this + off, value );                                            \
  }

#define il2cpp_field_set( type, fl_name, ns, klass_name, name )                           \
  inline auto fl_name( type value ) {                                                   \
    static const auto off = il2cpp::get_field( il2cpp::get_class( ns, klass_name ), name ); \
    if ( !off )                                                                         \
      return;                                                                           \
    mem::write< type >( this + off, value );                                            \
  }

#define il2cpp_static_field_get( type, fl_name, ns, klass_name, name )                           \
  static inline type fl_name( ) {                                                   \
    static const auto field = il2cpp::get_field( il2cpp::get_class( ns, klass_name ), name, false ); \
    if ( !memory::is_valid( field ) )                                                                         \
      return { };                                                                       \
    return il2cpp::get_static_field< type >( field );                                          \
  }

#define il2cpp_static_field_set( type, fl_name, ns, klass_name, name )                           \
  static auto fl_name( type value ) {                                                   \
    static const auto field = il2cpp::get_field( il2cpp::get_class( ns, klass_name ), name, false ); \
    if ( !memory::is_valid( field ) )                                                                         \
      return;                                                                        \
    il2cpp::set_static_field< type >( field, value );                                            \
  }

namespace il2cpp {
    static uint8_t* unity_player = nullptr;
    static uint8_t* game_assembly = nullptr;

    create_type( array_new, uintptr_t( * )( uintptr_t, int ) );
    create_type( string_new, uintptr_t( * )( const char* ) );
    create_type( object_new, uintptr_t( * )( uintptr_t ) );
    create_type( domain_get, uintptr_t( * )( ) );
    create_type( resolve_icall, uintptr_t( * )( const char* ) );
    create_type( thread_attach, void( * )( ) );
    create_type( class_get_type, uintptr_t( * )( uintptr_t ) );
    create_type( type_get_object, uintptr_t( * )( uintptr_t ) );
    create_type( class_from_name, uintptr_t( * )( uintptr_t, const char*, const char* ) );
    create_type( class_get_fields, uintptr_t( * )( uintptr_t, uintptr_t* ) );
    create_type( field_get_offset, uintptr_t( * )( uintptr_t ) );
    create_type( class_get_methods, uintptr_t( * )( uintptr_t, uintptr_t* ) );
    create_type( string_new_wrapper, uintptr_t( * )( const char* ) );
    create_type( assembly_get_image, uintptr_t( * )( uintptr_t ) );
    create_type( domain_get_assemblies, uintptr_t* ( * )( uintptr_t domain, uintptr_t* size ) );
    create_type( method_get_param_name, const char* ( * )( uintptr_t, uint32_t ) );
    create_type( method_get_param_count, int ( * )( uintptr_t ) );
    create_type( method_get_return_type, const char* ( * )( uintptr_t ) );
    create_type( field_static_get_value, uintptr_t( * )( uintptr_t, uintptr_t* ) );
    create_type( field_static_set_value, uintptr_t( * )( uintptr_t, uintptr_t* ) );
    create_type( class_get_method_from_name, uintptr_t( * )( uintptr_t, const char*, int ) );
    create_type( class_get_static_field_data, uintptr_t( * )( uintptr_t ) );

    namespace impl {
        uintptr_t _get_class( const char* ns, const char* klass_name );
        uintptr_t _get_method( uintptr_t klass, const char* name, int argument_number = -1 );
        uintptr_t _get_field( uintptr_t klass, const char* name, bool offset );
    } // namespace impl

    bool initialize( );

    uintptr_t new_string( const char* string );
    uintptr_t new_array( uintptr_t array, int length );
    uintptr_t type_object( const char* ns, const char* name );

    uintptr_t get_virtual_method( const char* ns, const char* klass_name, const char* method_name, int argument_number = -1 );

    template < class T = uintptr_t >
    inline T get_class( const char* ns, const char* name ) {
        return T( impl::_get_class( ns, name ) );
    }

    template < class T = uintptr_t >
    inline T new_object( const char* ns, const char* klass_name ) {
        const auto& klass = get_class( ns, klass_name );
        if ( !memory::is_valid( klass ) )
            return { };

        return T( il2cpp_call( object_new )( klass ) );
    }

    template < class T = uintptr_t >
    inline T get_method( uintptr_t klass, const char* name, int argument_number = -1 ) {
        if ( !memory::is_valid( klass ) )
            return { };

        return T( impl::_get_method( klass, name, argument_number ) );
    }

    template < class T = uintptr_t >
    inline T get_field( uintptr_t klass, const char* name, bool offset = true ) {
        if ( !memory::is_valid( klass ) )
            return { };

        const auto& field = impl::_get_field( klass, name, offset );
        if ( !field ) {
            TRACE( "%s not found!", name );
            return { };
        }

        TRACE( "%s @ %#llx was found!", name, field );
        return T( field );
    }

    template < class T = uintptr_t >
    inline T get_static_field( uintptr_t field ) {
        if ( !memory::is_valid( field ) )
            return { };

        uintptr_t ret = 0;
        il2cpp_call( field_static_get_value )( field, &ret );

        return T( ret );
    }

    template < class T = uintptr_t >
    inline void set_static_field( uintptr_t field, T value ) {
        if ( !memory::is_valid( field ) )
            return;

        il2cpp_call( field_static_set_value )( field, ( uintptr_t* )&value );
    }
} // namespace il2cpp

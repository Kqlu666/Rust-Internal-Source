#include "il2cpp.hpp"

bool il2cpp::initialize( ) {
    unity_player = pe::get_image( HASH( "unityplayer" ) );
    game_assembly = pe::get_image( HASH( "gameassembly" ) );

    if ( !unity_player || !game_assembly )
        return false;

    assign_type( array_new );
    assign_type( string_new );
    assign_type( object_new );
    assign_type( domain_get );
    assign_type( resolve_icall );
    assign_type( thread_attach );
    assign_type( class_get_type );
    assign_type( type_get_object );
    assign_type( class_from_name );
    assign_type( class_get_fields );
    assign_type( field_get_offset );
    assign_type( class_get_methods );
    assign_type( string_new_wrapper );
    assign_type( assembly_get_image );
    assign_type( domain_get_assemblies );
    assign_type( method_get_param_name );
    assign_type( method_get_param_count );
    assign_type( method_get_return_type );
    assign_type( field_static_get_value );
    assign_type( field_static_set_value );
    assign_type( class_get_method_from_name );
    assign_type( class_get_static_field_data );

    il2cpp_call( thread_attach )( );
    return true;
}

uintptr_t il2cpp::impl::_get_class( const char* ns, const char* klass_name ) {
    const auto& domain = il2cpp_call( domain_get )( );

    std::size_t count = 0;
    const auto& assemblies = il2cpp_call( domain_get_assemblies )( domain, &count );

    for ( size_t idx = 0; idx < count; idx++ ) {
        const auto& assembly = assemblies[ idx ];
        if ( !memory::is_valid( assembly ) )
            continue;

        const auto& img = il2cpp_call( assembly_get_image )( assembly );
        if ( !memory::is_valid( img ) )
            continue;

        const auto& klass = il2cpp_call( class_from_name )( img, ns, klass_name );
        if ( !memory::is_valid( klass ) )
            continue;

        TRACE( "\"%s.%s\" @ %#llx was found!", ns, klass_name, klass );
        return klass;
    }

    TRACE( "%s not found!", klass_name );
    PAUSE( );
    return 0;
}

uintptr_t il2cpp::impl::_get_method( uintptr_t klass, const char* name, int argument_number ) {
    const auto& method = il2cpp_call( class_get_method_from_name )( klass, name, argument_number );
    if ( !memory::is_valid( method ) ) {
        TRACE( "%s not found!", name );
        PAUSE( );
        return { };
    }

    TRACE( "%s @ %#llx was found!", name, method );
    return method;
}

uintptr_t il2cpp::impl::_get_field( uintptr_t klass, const char* name, bool offset ) {
    uintptr_t iterator = { };

    while ( auto field = il2cpp_call( class_get_fields )( klass, &iterator ) ) {
        std::string current_name = mem::read< const char* >( field );
        if ( current_name.empty( ) )
            continue;

        if ( current_name.find( name ) == std::string::npos )
            continue;

        if ( !offset )
            return field;

        return il2cpp_call( field_get_offset )( field );
    }

    return { };
}

uintptr_t il2cpp::get_virtual_method( const char* ns, const char* klass_name, const char* method_name, int argument_number ) {
    const auto& klass = get_class( ns, klass_name );
    if ( !memory::is_valid( klass ) )
        return { };

    const auto& method = mem::read< uintptr_t >( get_method( klass, method_name, argument_number ) );
    if ( !memory::is_valid( method ) )
        return { };

    for ( auto idx = klass; idx <= klass + 0x10000; idx += 0x1 ) {
        const auto& addr = mem::read< uintptr_t >( idx );
        if ( addr == method ) {
            TRACE( "%s @ %#llx virtual was found!", method_name, idx );
            return idx;
        }
    }

    TRACE( "%s virtual not found!", method_name );
    PAUSE( );
    return { };
}

uintptr_t il2cpp::new_string( const char* string ) {
    return il2cpp_call( string_new )( string );
}

uintptr_t il2cpp::new_array( uintptr_t array, int length ) {
    return il2cpp_call( array_new )( array, length );
}

uintptr_t il2cpp::type_object( const char* ns, const char* name ) {
    const auto& klass = get_class( ns, name );
    if ( !memory::is_valid( klass ) )
        return { };

    const auto& class_type = il2cpp_call( class_get_type )( klass );
    if ( !memory::is_valid( class_type ) )
        return { };

    return il2cpp_call( type_get_object )( class_type );
}
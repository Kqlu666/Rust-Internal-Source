#pragma once

namespace sdk {
	class c_base_player;

	class c_base_entity : public c_base_networkable {
	public:
		il2cpp_field_get( unity::bounds_t, bounds, _( "" ), _( "BaseEntity" ), _( "bounds" ) );
		il2cpp_field_get( c_model*, entity_model, _( "" ), _( "BaseEntity" ), _( "model" ) );

		il2cpp_field_set( vec3_t, set_hit_direction, _( "" ), _( "TreeEntity" ), _( "hitDirection" ) );

		template<typename T = void*>
		static inline T find_closest( std::string name, float max_distance ) {
			float best_distance = max_distance;
			T best_object = NULL;

			const auto& client_entities = sdk::c_base_networkable::client_entities( );
			if ( !memory::is_valid( client_entities ) )
				return T( );

			const auto& entity_list = client_entities->entity_list( );
			if ( !memory::is_valid( client_entities ) )
				return T( );

			const auto& count = entity_list->count( );
			if ( count <= 0 )
				return T( );

			for ( auto i = 0; i < count; ++i ) {
				const auto& base_networkable = entity_list->get< sdk::c_base_networkable* >( i );
				if ( !memory::is_valid( base_networkable ) )
					continue;

				if ( !base_networkable->is_class( name.c_str( ) ) )
					continue;

				const auto& transform = base_networkable->transform( );
				if ( !memory::is_valid( transform ) )
					continue;

				const auto& distance = sdk::local_position.distance_to( transform->position( ) );
				if ( distance < best_distance ) {
					best_distance = distance;
					best_object = T( base_networkable );
				}
			}

			return best_object;
		}

		inline auto server_rpc( const char* name ) {
			il2cpp_method( ptr, _( "" ), _( "BaseEntity" ), _( "ServerRPC" ), 1, void ( * )( c_base_entity*, unity::c_string ) );
			safe.call< void >( ptr, this, il2cpp::new_string( name ) );
		}

		inline auto server_rpc( const char* rpc, const char* string, bool value ) {
			il2cpp_method( ptr, _( "" ), _( "BaseEntity" ), _( "ServerRPC" ), 4, void ( * )( c_base_entity*, unity::c_string, unity::c_string, bool ) );
			safe.call< void >( ptr, this, il2cpp::new_string( rpc ), il2cpp::new_string( string ), value );
		}

		inline auto parent_velocity( ) {
			il2cpp_method( ptr, _( "" ), _( "BaseEntity" ), _( "GetParentVelocity" ), -1, vec3_t( * )( c_base_entity* ) );
			return safe.call< vec3_t >( ptr, this );
		}

		inline void send_signal_broadcast( sdk::e_signal signal, const char* rpc = "" ) {
			il2cpp_method( ptr, _( "" ), _( "BaseEntity" ), _( "SendSignalBroadcast" ), 2, void ( * )( c_base_entity*, sdk::e_signal, unity::c_string ) );
			safe.call< void >( ptr, this, signal, il2cpp::new_string( rpc ) );
		}

		inline auto is_visible_and_see( vec3_t position ) {
			il2cpp_method( ptr, _( "" ), _( "BaseEntity" ), _( "IsVisibleAndCanSee" ), -1, bool( * )( vec3_t ) );
			return safe.call< bool >( ptr, this, position );
		}

		inline float max_velocity( ) {
			il2cpp_method( ptr, _( "" ), _( "BaseEntity" ), _( "MaxVelocity" ), -1, void ( * )( c_base_entity* ) );
			return safe.call< float >( ptr, this );
		}

		inline float bounds_padding( ) {
			il2cpp_method( ptr, _( "" ), _( "BaseEntity" ), _( "BoundsPadding" ), -1, void ( * )( c_base_entity* ) );
			return safe.call< float >( ptr, this );
		}
	};
}
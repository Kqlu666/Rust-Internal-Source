#pragma once

namespace sdk {

	struct trace_info_t {

		bool valid; // 0x0
		float distance; // 0x4
		c_base_entity* entity; // 0x8   //BasePlayer
		vec3_t point; // 0x10
		vec3_t normal; // 0x1C
		unity::c_transform* bone; // 0x28
		uintptr_t material; // 0x30
		unsigned int partID; // 0x38
		unity::c_component* collider; // 0x40
	};

	class c_asset_name_cache {
	public:
		static auto name( uintptr_t material ) {
			il2cpp_method( ptr, _( "" ), _( "AssetNameCache" ), _( "GetName" ), 1, unity::c_string * ( * )( uintptr_t ) );
			return safe.call< unity::c_string* >( ptr, material );
		}
	};

	class c_held_entity;

	class c_projectile : public unity::c_component {
	public:
		il2cpp_field_get( float, drag, _( "" ), _( "Projectile" ), _( "drag" ) );
		il2cpp_field_get( bool, invisible, _( "" ), _( "Projectile" ), _( "invisible" ) );
		il2cpp_field_get( float, tumble_speed, _( "" ), _( "Projectile" ), _( "tumbleSpeed" ) );
		il2cpp_field_get( bool, create_decals, _( "" ), _( "Projectile" ), _( "createDecals" ) );
		il2cpp_field_get( c_base_player*, owner, _( "" ), _( "Projectile" ), _( "owner" ) );
		il2cpp_field_get( float, integrity, _( "" ), _( "Projectile" ), _( "integrity" ) );
		il2cpp_field_get( c_hit_test*, hit_test, _( "" ), _( "Projectile" ), _( "hitTest" ) );
		il2cpp_field_get( float, max_distance, _( "" ), _( "Projectile" ), _( "maxDistance" ) );
		il2cpp_field_get( float, launch_time, _( "" ), _( "Projectile" ), _( "launchTime" ) );
		il2cpp_field_get( float, traveled_time, _( "" ), _( "Projectile" ), _( "traveledTime" ) );
		il2cpp_field_get( float, close_flyby_distance, _( "" ), _( "Projectile" ), _( "closeFlybyDistance" ) );
		il2cpp_field_get( int, projectile_id, _( "" ), _( "Projectile" ), _( "projectileID" ) );
		il2cpp_field_get( float, gravity_modifier, _( "" ), _( "Projectile" ), _( "gravityModifier" ) );
		il2cpp_field_get( float, initial_distance, _( "" ), _( "Projectile" ), _( "initialDistance" ) );
		il2cpp_field_get( float, penetration_power, _( "" ), _( "Projectile" ), _( "penetrationPower" ) );
		il2cpp_field_get( float, traveled_distance, _( "" ), _( "Projectile" ), _( "traveledDistance" ) );
		il2cpp_field_get( vec3_t, initial_velocity, _( "" ), _( "Projectile" ), _( "initialVelocity" ) );
		il2cpp_field_get( vec3_t, current_position, _( "" ), _( "Projectile" ), _( "current_position" ) );
		il2cpp_field_get( vec3_t, current_velocity, _( "" ), _( "Projectile" ), _( "currentVelocity" ) );
		il2cpp_field_get( vec3_t, previous_position, _( "" ), _( "Projectile" ), _( "previousPosition" ) );
		il2cpp_field_get( uintptr_t, damage_properties, _( "" ), _( "Projectile" ), _( "damageProperties" ) );
		il2cpp_field_get( c_held_entity*, source_weapon_prefab, _( "" ), _( "Projectile" ), _( "sourceWeaponPrefab" ) );
		il2cpp_field_get( c_projectile*, source_projectile_prefab, _( "" ), _( "Projectile" ), _( "sourceProjectilePrefab" ) );

		il2cpp_field_set( float, set_integrity, _( "" ), _( "Projectile" ), _( "integrity" ) );
		il2cpp_field_set( float, set_thickness, _( "" ), _( "Projectile" ), _( "thickness" ) );
		il2cpp_field_set( c_hit_test*, set_hit_test, _( "" ), _( "Projectile" ), _( "hitTest" ) );
		il2cpp_field_set( vec3_t, set_sent_position, _( "" ), _( "Projectile" ), _( "sentPosition" ) );
		il2cpp_field_set( float, set_traveled_time, _( "" ), _( "Projectile" ), _( "traveledTime" ) );
		il2cpp_field_set( vec3_t, set_previous_position, _( "" ), _( "Projectile" ), _( "previousPosition" ) );
		il2cpp_field_set( vec3_t, set_current_position, _( "" ), _( "Projectile" ), _( "current_position" ) );
		il2cpp_field_set( vec3_t, set_initial_velocity, _( "" ), _( "Projectile" ), _( "initialVelocity" ) );
		il2cpp_field_set( vec3_t, set_current_velocity, _( "" ), _( "Projectile" ), _( "currentVelocity" ) );
		il2cpp_field_set( float, set_initial_distance, _( "" ), _( "Projectile" ), _( "initialDistance" ) );
		il2cpp_field_set( float, set_traveled_distance, _( "" ), _( "Projectile" ), _( "traveledDistance" ) );

		inline auto do_movement( float delta_time ) {
			il2cpp_method( ptr, _( "" ), _( "Projectile" ), _( "DoMovement" ), 1, void( * )( c_projectile*, float ) );
			safe.call< void >( ptr, this, delta_time );
		}

		inline auto do_velocity_update( float delta_time ) {
			il2cpp_method( ptr, _( "" ), _( "Projectile" ), _( "DoVelocityUpdate" ), 1, void( * )( c_projectile*, float ) );
			safe.call< void >( ptr, this, delta_time );
		}

		inline auto do_flyby_sound( ) {
			il2cpp_method( ptr, _( "" ), _( "Projectile" ), _( "DoFlybySound" ), -1, void( * )( c_projectile* ) );
			safe.call< void >( ptr, this );
		}

		inline float effect_scale( ) {
			if ( invisible( ) ) {
				return 0.f;
			}

			if ( traveled_distance( ) < 8.f ) {
				return 0.f;
			}

			return vec3_t( ).inverse_lerp( 12.f, 20.f, current_velocity( ).magnitude( ) );
		}

		inline auto set_effect_scale( float scale ) {
			il2cpp_method( ptr, _( "" ), _( "Projectile" ), _( "SetEffectScale" ), 1, void( * )( c_projectile*, float ) );
			safe.call< void >( ptr, this, scale );
		}

		inline auto do_hit( c_hit_test* test, vec3_t point, vec3_t normal ) {
			il2cpp_method( ptr, _( "" ), _( "Projectile" ), _( "DoHit" ), 3, bool( * )( c_projectile*, c_hit_test*, vec3_t, vec3_t ) );
			return safe.call< bool >( ptr, this, test, point, normal );
		}

		inline auto retire( ) {
			il2cpp_method( ptr, _( "" ), _( "Projectile" ), _( "Retire" ), -1, void( * )( c_projectile* ) );
			safe.call< void >( ptr, this );
		}

		inline void update_hit_test( trace_info_t info ) {
			const auto& ht_test = hit_test( );
			if ( !memory::is_valid( ht_test ) )
				return;

			ht_test->set_did_hit( true );
			ht_test->set_hit_entity( info.entity );
			ht_test->set_hit_distance( info.distance );

			if ( info.material != 0 )
				ht_test->set_hit_material( c_asset_name_cache::name( info.material ) );

			ht_test->set_hit_part( info.partID );
			ht_test->set_hit_transform( info.bone );
			ht_test->set_hit_point( info.point );
			ht_test->set_hit_normal( info.normal );
			ht_test->set_collider( info.collider );

			auto go = ( info.collider ) ? info.collider->game_object( ) : info.bone->game_object( );
			ht_test->set_game_object( go );

			if ( info.bone != 0 ) {
				const auto& hit_transform = ht_test->hit_transform( );

				vec3_t hit_point = hit_transform->inverse_transform_point( info.point );
				ht_test->set_hit_point( hit_point );

				vec3_t normal_point = hit_transform->inverse_transform_direction( info.normal );
				ht_test->set_hit_normal( normal_point );
			}
		}

		inline auto is_alive( ) {
			return ( integrity( ) > 0.001f && traveled_distance( ) < max_distance( ) );
		}
	};
}
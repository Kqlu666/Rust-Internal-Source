#pragma once

namespace sdk {
	class c_hit_test {
	public:
		il2cpp_field_get( c_base_entity*, hit_entity, _( "" ), _( "HitTest" ), _( "HitEntity" ) );
		il2cpp_field_get( vec3_t, hit_point, _( "" ), _( "HitTest" ), _( "HitPoint" ) );
		il2cpp_field_get( vec3_t, hit_normal, _( "" ), _( "HitTest" ), _( "HitNormal" ) );
		il2cpp_field_get( unity::c_transform*, hit_transform, _( "" ), _( "HitTest" ), _( "HitTransform" ) );

		il2cpp_field_set( int, set_type, _( "" ), _( "HitTest" ), _( "type" ) );
		il2cpp_field_set( float, set_radius, _( "" ), _( "HitTest" ), _( "Radius" ) );
		il2cpp_field_set( bool, set_did_hit, _( "" ), _( "HitTest" ), _( "DidHit" ) );
		il2cpp_field_set( bool, set_best_hit, _( "" ), _( "HitTest" ), _( "BestHit" ) );
		il2cpp_field_set( float, set_forgiveness, _( "" ), _( "HitTest" ), _( "Forgiveness" ) );
		il2cpp_field_set( vec3_t, set_hit_point, _( "" ), _( "HitTest" ), _( "HitPoint" ) );
		il2cpp_field_set( vec3_t, set_hit_normal, _( "" ), _( "HitTest" ), _( "HitNormal" ) );
		il2cpp_field_set( unity::c_component*, set_collider, _( "" ), _( "HitTest" ), _( "collider" ) );
		il2cpp_field_set( unity::c_game_object*, set_game_object, _( "" ), _( "HitTest" ), _( "gameObject" ) );
		il2cpp_field_set( unity::ray_t, set_attack_ray, _( "" ), _( "HitTest" ), _( "AttackRay" ) );
		il2cpp_field_set( float, set_hit_distance, _( "" ), _( "HitTest" ), _( "HitDistance" ) );
		il2cpp_field_set( float, set_max_distance, _( "" ), _( "HitTest" ), _( "MaxDistance" ) );
		il2cpp_field_set( unity::c_string*, set_hit_material, _( "" ), _( "HitTest" ), _( "HitMaterial" ) );
		il2cpp_field_set( unsigned int, set_hit_part, _( "" ), _( "HitTest" ), _( "HitPart" ) );
		il2cpp_field_set( c_base_entity*, set_hit_entity, _( "" ), _( "HitTest" ), _( "HitEntity" ) );
		il2cpp_field_set( c_base_entity*, set_ignore_entity, _( "" ), _( "HitTest" ), _( "ignoreEntity" ) );
		il2cpp_field_set( unity::c_transform*, set_hit_transform, _( "" ), _( "HitTest" ), _( "HitTransform" ) );
		il2cpp_field_set( uintptr_t, set_damage_properties, _( "" ), _( "HitTest" ), _( "damageProperties" ) );

		inline auto hit_point_world( ) {
			il2cpp_method( ptr, _( "" ), _( "HitTest" ), _( "HitPointWorld" ), -1, vec3_t( * )( c_hit_test* ) );
			return safe.call< vec3_t >( ptr, this );
		}

		inline auto hit_normal_world( ) {
			il2cpp_method( ptr, _( "" ), _( "HitTest" ), _( "HitNormalWorld" ), -1, vec3_t( * )( c_hit_test* ) );
			return safe.call< vec3_t >( ptr, this );
		}

		inline auto build_attack_message( c_base_player* player ) {
			il2cpp_method( ptr, _( "" ), _( "HitTest" ), _( "BuildAttackMessage" ), 1, c_attack * ( * )( c_hit_test*, c_base_player* ) );
			return safe.call< c_attack* >( ptr, this, player );
		}
	};
}
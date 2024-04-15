namespace sdk {
	class c_hit_info {
	public:
		il2cpp_field_get( c_base_player*, initiator, _( "" ), _( "HitInfo" ), _( "Initiator" ) );
		il2cpp_field_get( bool, did_hit, _( "" ), _( "HitInfo" ), _( "DidHit" ) );
		il2cpp_field_get( vec3_t, point_start, _( "" ), _( "HitInfo" ), _( "PointStart" ) );
		il2cpp_field_get( vec3_t, hit_position_world, _( "" ), _( "HitInfo" ), _( "HitPositionWorld" ) );
		il2cpp_field_get( float, projectile_distance, _( "" ), _( "HitInfo" ), _( "ProjectileDistance" ) );
		
		il2cpp_field_set( c_base_player*, set_initiator, _( "" ), _( "HitInfo" ), _( "Initiator" ) );
		il2cpp_field_set( uint32_t, set_hit_bone, _( "" ), _( "HitInfo" ), _( "HitBone" ) );
		il2cpp_field_set( int, set_projectile_id, _( "" ), _( "HitInfo" ), _( "ProjectileID" ) );
		il2cpp_field_set( c_held_entity*, set_weapon_prefab, _( "" ), _( "HitInfo" ), _( "WeaponPrefab" ) );
		il2cpp_field_set( bool, set_is_predicting, _( "" ), _( "HitInfo" ), _( "IsPredicting" ) );
		il2cpp_field_set( bool, set_do_decals, _( "" ), _( "HitInfo" ), _( "DoDecals" ) );
		il2cpp_field_set( float, set_projectile_distance, _( "" ), _( "HitInfo" ), _( "ProjectileDistance" ) );
		il2cpp_field_set( vec3_t, set_projectile_velocity, _( "" ), _( "HitInfo" ), _( "ProjectileVelocity" ) );
		il2cpp_field_set( c_projectile*, set_projectile_prefab, _( "" ), _( "HitInfo" ), _( "ProjectilePrefab" ) );

		inline auto position_on_ray( vec3_t pos ) {
			il2cpp_method( ptr, _( "" ), _( "HitInfo" ), _( "PositionOnRay" ), 1, void( * )( c_hit_info*, vec3_t ) );
			return safe.call< vec3_t >( ptr, this, pos );
		}

		inline auto load_from_attack( c_attack* attack, bool state ) {
			il2cpp_method( ptr, _( "" ), _( "HitInfo" ), _( "LoadFromAttack" ), 2, void( * )( c_hit_info*, c_attack*, bool ) );
			safe.call< void >( ptr, this, attack, state );
		}
	};
}
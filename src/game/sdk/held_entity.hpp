#pragma once

namespace sdk {
	class c_item_definition;

	class c_aim_cone_util {
	public:
		static auto modified_aim_cone_direction( float aim_cone, vec3_t input_vec, bool anywhere_inside = true ) {
			il2cpp_method( ptr, _( "" ), _( "AimConeUtil" ), _( "GetModifiedAimConeDirection" ), 3, vec3_t( * )( float, vec3_t, bool ) );
			return safe.call< vec3_t >( ptr, aim_cone, input_vec, anywhere_inside );
		}
	};
 
	class c_recoil_properties {
	public:
		il2cpp_field_get( c_recoil_properties*, new_recoil_override, _( "" ), _( "RecoilProperties" ), _( "newRecoilOverride" ) );

		il2cpp_field_set( float, set_recoil_yaw_min, _( "" ), _( "RecoilProperties" ), _( "recoilYawMin" ) );
		il2cpp_field_set( float, set_recoil_yaw_max, _( "" ), _( "RecoilProperties" ), _( "recoilYawMax" ) );
		il2cpp_field_set( float, set_recoil_pitch_min, _( "" ), _( "RecoilProperties" ), _( "recoilPitchMin" ) );
		il2cpp_field_set( float, set_recoil_pitch_max, _( "" ), _( "RecoilProperties" ), _( "recoilPitchMax" ) );
	};

	class c_held_entity : public c_base_entity {
	public:
		il2cpp_field_get( bool, is_deployed, _( "" ), _( "HeldEntity" ), _( "isDeployed" ) );
		il2cpp_field_get( float, next_reload_time, _( "" ), _( "BaseProjectile" ), _( "nextReloadTime" ) );
		il2cpp_field_get( float, last_shot_time, _( "" ), _( "BaseProjectile" ), _( "lastShotTime" ) );
		il2cpp_field_get( bool, needs_cycle, _( "" ), _( "BaseProjectile" ), _( "needsCycle" ) );
		il2cpp_field_get( bool, manual_cycle, _( "" ), _( "BaseProjectile" ), _( "manualCycle" ) );
		il2cpp_field_get( bool, automatic, _( "" ), _( "BaseProjectile" ), _( "automatic" ) );

		il2cpp_field_get( float, repeat_delay, _( "" ), _( "AttackEntity" ), _( "repeatDelay" ) );
		il2cpp_field_get( float, deploy_delay, _( "" ), _( "AttackEntity" ), _( "deployDelay" ) );
		il2cpp_field_get( float, next_attack_time, _( "" ), _( "AttackEntity" ), _( "nextAttackTime" ) );
		il2cpp_field_get( float, time_since_deploy, _( "" ), _( "AttackEntity" ), _( "timeSinceDeploy" ) );

		il2cpp_field_set( float, set_deploy_delay, _( "" ), _( "AttackEntity" ), _( "deployDelay" ) );
		il2cpp_field_set( float, set_block_sprint, _( "" ), _( "BaseMelee" ), _( "blockSprintOnAttack" ) );

		il2cpp_field_get( c_view_model*, view_model, _( "" ), _( "HeldEntity" ), _( "viewModel" ) );

		il2cpp_field_get( float, max_distance, _( "" ), _( "BaseMelee" ), _( "maxDistance" ) );
		il2cpp_field_set( float, set_max_distance, _( "" ), _( "BaseMelee" ), _( "maxDistance" ) );
		il2cpp_field_set( float, set_attack_radius, _( "" ), _( "BaseMelee" ), _( "attackRadius" ) );

		il2cpp_field_set( bool, set_was_aiming, _( "" ), _( "BowWeapon" ), _( "wasAiming" ) );
		il2cpp_field_set( bool, set_attack_ready, _( "" ), _( "BowWeapon" ), _( "attackReady" ) );

		il2cpp_field_set( float, set_string_bonus_damage, _( "" ), _( "CompoundBowWeapon" ), _( "stringBonusDamage" ) );
		il2cpp_field_set( float, set_current_hold_progress, _( "" ), _( "CompoundBowWeapon" ), _( "currentHoldProgress" ) );
		il2cpp_field_set( float, set_movement_penalty_ramp_up_time, _( "" ), _( "CompoundBowWeapon" ), _( "movementPenaltyRampUpTime" ) );

		il2cpp_field_set( float, set_repeat_delay, _( "" ), _( "AttackEntity" ), _( "repeatDelay" ) );

		il2cpp_field_set( float, set_success_fraction, _( "" ), _( "FlintStrikeWeapon" ), _( "successFraction" ) );
		il2cpp_field_set( bool, set_did_spark_this_frame, _( "" ), _( "FlintStrikeWeapon" ), _( "didSparkThisFrame" ) );

		il2cpp_field_get( float , reload_time, _( "" ), _( "BaseProjectile" ), _( "reloadTime" ) );
		il2cpp_field_get( c_recoil_properties*, recoil, _( "" ), _( "BaseProjectile" ), _( "recoil" ) );
		il2cpp_field_get( c_magazine*, primary_magazine, _( "" ), _( "BaseProjectile" ), _( "primaryMagazine" ) );
		il2cpp_field_get( uintptr_t, damage_properties, _( "" ), _( "BaseMelee" ), _( "damageProperties" ) );

		il2cpp_field_set( float, set_aim_cone, _( "" ), _( "BaseProjectile" ), _( "aimCone" ) );
		il2cpp_field_set( float, set_is_burst_weapon, _( "" ), _( "BaseProjectile" ), _( "isBurstWeapon" ) );
		il2cpp_field_set( float, set_aim_sway, _( "" ), _( "BaseProjectile" ), _( "aimSway" ) );
		il2cpp_field_set( float, set_hip_aim_cone, _( "" ), _( "BaseProjectile" ), _( "hipAimCone" ) );
		il2cpp_field_set( bool, set_automatic, _( "" ), _( "BaseProjectile" ), _( "automatic" ) );
		il2cpp_field_set( bool, set_trigger_ready, _( "" ), _( "BaseProjectile" ), _( "triggerReady" ) );
		il2cpp_field_set( bool, set_needs_cycle, _( "" ), _( "BaseProjectile" ), _( "needsCycle" ) );
		il2cpp_field_set( float, set_aim_sway_speed, _( "" ), _( "BaseProjectile" ), _( "aimSwaySpeed" ) );
		il2cpp_field_set( float, set_distance_scale, _( "" ), _( "BaseProjectile" ), _( "distanceScale" ) );
		il2cpp_field_set( float, set_stance_penalty, _( "" ), _( "BaseProjectile" ), _( "stancePenalty" ) );
		il2cpp_field_set( float, set_aimcone_penalty, _( "" ), _( "BaseProjectile" ), _( "aimconePenalty" ) );
		il2cpp_field_set( float, set_hip_aim_cone_scale, _( "" ), _( "BaseProjectile" ), _( "hipAimConeScale" ) );
		il2cpp_field_set( float, set_hip_aim_cone_offset, _( "" ), _( "BaseProjectile" ), _( "hipAimConeOffset" ) );
		il2cpp_field_set( float, set_aim_cone_penalty_max, _( "" ), _( "BaseProjectile" ), _( "aimConePenaltyMax" ) );
		il2cpp_field_set( float, set_stance_penalty_scale, _( "" ), _( "BaseProjectile" ), _( "stancePenaltyScale" ) );
		il2cpp_field_set( float, set_sight_aim_cone_scale, _( "" ), _( "BaseProjectile" ), _( "sightAimConeScale" ) );
		il2cpp_field_set( float, set_sight_aim_cone_offset, _( "" ), _( "BaseProjectile" ), _( "sightAimConeOffset" ) );
		il2cpp_field_set( float, set_no_aiming_while_cycling, _( "" ), _( "BaseProjectile" ), _( "noAimingWhileCycling" ) );
		il2cpp_field_set( float, set_aimcone_penalty_per_shot, _( "" ), _( "BaseProjectile" ), _( "aimconePenaltyPerShot" ) );
		il2cpp_field_set( float, set_projectile_velocity_scale, _( "" ), _( "BaseProjectile" ), _( "projectileVelocityScale" ) );
		il2cpp_field_set( float, set_aimcone_penalty_recover_time, _( "" ), _( "BaseProjectile" ), _( "aimconePenaltyRecoverTime" ) );
		il2cpp_field_set( float, set_aimcone_penalty_recover_delay, _( "" ), _( "BaseProjectile" ), _( "aimconePenaltyRecoverDelay" ) );

		il2cpp_field_get( unity::c_list< c_projectile* >*, created_projectiles, _( "" ), _( "BaseProjectile" ), _( "createdProjectiles" ) );

		inline auto do_throw( ) {
			il2cpp_method( ptr, _( "" ), _( "BaseMelee" ), _( "DoThrow" ), -1, void( * )( c_held_entity* ) );
			safe.call< void >( ptr, this );
		}

		inline auto process_attack( c_hit_test* hit ) {
			il2cpp_method( ptr, _( "" ), _( "BaseMelee" ), _( "ProcessAttack" ), 1, void( * )( c_held_entity*, c_hit_test* ) );
			safe.call< void >( ptr, this, hit );
		}

		inline auto start_attack_cooldown( float cooldown ) {
			il2cpp_method( ptr, _( "" ), _( "AttackEntity" ), _( "StartAttackCooldown" ), 1, void( * )( c_held_entity*, float ) );
			safe.call< void >( ptr, this, cooldown );
		}

		inline auto owner_item_definition( ) {
			il2cpp_method( ptr, _( "" ), _( "HeldEntity" ), _( "GetOwnerItemDefinition" ), -1, void( * )( c_held_entity* ) );
			return safe.call< c_item_definition* >( ptr, this );
		}

		inline auto has_attack_cooldown( ) {
			il2cpp_method( ptr, _( "" ), _( "AttackEntity" ), _( "HasAttackCooldown" ), -1, bool( * )( c_held_entity* ) );
			return safe.call< bool >( ptr, this );
		}

		inline auto has_reload_cooldown( ) {
			return unity::c_time::time( ) < next_reload_time( );
		}

		inline auto is_fully_deployed( ) {
			il2cpp_method( ptr, _( "" ), _( "AttackEntity" ), _( "IsFullyDeployed" ), -1, bool( * )( c_held_entity* ) );
			return safe.call< bool >( ptr, this );
		}

		inline auto projectile_velocity_scale( bool max = false ) {
			il2cpp_method( ptr, _( "" ), _( "BaseProjectile" ), _( "GetProjectileVelocityScale" ), 1, float( * )( c_held_entity* ) );
			return safe.call< float >( ptr, this, max );
		}

		inline auto shot_fired( ) {
			il2cpp_method( ptr, _( "" ), _( "BaseProjectile" ), _( "ShotFired" ), -1, void( * )( c_held_entity* ) );
			safe.call< void >( ptr, this );
		}

		inline auto did_attack_clientside( ) {
			il2cpp_method( ptr, _( "" ), _( "BaseProjectile" ), _( "DidAttackClientside" ), -1, void( * )( c_held_entity* ) );
			safe.call< void >( ptr, this );
		}

		inline auto begin_cycle( ) {
			il2cpp_method( ptr, _( "" ), _( "BaseProjectile" ), _( "BeginCycle" ), -1, void( * )( c_held_entity* ) );
			safe.call< void >( ptr, this );
		}

		inline auto launch_projectile( ) {
			il2cpp_method( ptr, _( "" ), _( "BaseProjectile" ), _( "LaunchProjectile" ), -1, void( * )( c_held_entity* ) );
			safe.call< void >( ptr, this );
		}

		inline auto update_ammo_display( ) {
			il2cpp_method( ptr, _( "" ), _( "BaseProjectile" ), _( "UpdateAmmoDisplay" ), -1, void( * )( c_held_entity* ) );
			safe.call< void >( ptr, this );
		}

		inline void do_attack_recreation( ) {
			if ( !is_class( _( "BaseProjectile" ) ) )
				return;

			if ( has_attack_cooldown( ) )
				return;

			if ( time_since_deploy( ) < deploy_delay( ) )
				return;

			set_trigger_ready( true );

			if ( manual_cycle( ) && needs_cycle( ) ) {
				begin_cycle( );
				return;
			}

			if ( has_attack_cooldown( ) ) {
				return;
			}

			if ( !automatic( ) ) {
				set_trigger_ready( false );
			}

			if ( primary_magazine( )->contents( ) <= 0 ) {
				return;
			}

			send_signal_broadcast( e_signal::attack );

			primary_magazine( )->set_contents( primary_magazine( )->contents( ) - 1 );

			launch_projectile( );
			update_ammo_display( );
			shot_fired( );
			did_attack_clientside( );
			if ( !manual_cycle( ) ) {
				begin_cycle( );
				return;
			}

			set_needs_cycle( true );
		}
	};
}
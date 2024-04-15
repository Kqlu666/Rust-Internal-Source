#pragma once

namespace sdk {

	class c_item : public unity::c_behaviour {
	public:
		il2cpp_field_get( int, amount, _( "" ), _( "Item" ), _( "amount" ) );
		il2cpp_field_get( unsigned int, uid, _( "" ), _( "Item" ), _( "uid" ) );
		il2cpp_field_get( float, remove_time, _( "" ), _( "Item" ), _( "removeTime" ) );
		il2cpp_field_get( c_item_definition*, info, _( "" ), _( "Item" ), _( "info" ) );
		il2cpp_field_get( c_held_entity*, held_entity, _( "" ), _( "Item" ), _( "heldEntity" ) );

		inline auto icon_sprite( ) {
			il2cpp_method( ptr, _( "" ), _( "Item" ), _( "get_iconSprite" ), -1, unity::c_sprite * ( * )( c_item* ) );
			return safe.call< unity::c_sprite* >( ptr, this );
		}

		inline bool is_weapon( ) {
			const bool b_is_weapon = !is_class( _( "BaseMelee" ) ) &&
				!is_class( _( "ThrownWeapon" ) ) &&
				!is_class( _( "TorchWeapon" ) );
			if ( !b_is_weapon )
				return false;

			const auto& held = held_entity( );
			if ( !memory::is_valid( held ) )
				return false;

			if ( b_is_weapon && (
				held->is_class( _( "BaseProjectile" ) ) ||
				held->is_class( _( "BowWeapon" ) ) ||
				held->is_class( _( "CompoundBowWeapon" ) ) ||
				held->is_class( _( "CrossbowWeapon" ) ) ||
				held->is_class( _( "FlintStrikeWeapon" ) ) ) )
				return true;

			return false;
		}

		inline bool is_heal( ) {
			const auto& held = held_entity( );
			if ( !memory::is_valid( held ) )
				return false;

			return held->is_class( _( "MedicalTool" ) );
		}

		inline bool is_melee( ) {
			const auto& held = held_entity( );
			if ( !memory::is_valid( held ) )
				return false;

			if (
				held->is_class( _( "ThrownWeapon" ) ) ||
				held->is_class( _( "BaseMelee" ) ) ||
				held->is_class( _( "Jackhammer" ) ) ||
				held->is_class( _( "Paddle" ) )
			   )
				return true;

			return false;
		}

		inline auto despawn_multiplier( ) {
			il2cpp_method( ptr, _( "" ), _( "Item" ), _( "get_despawnMultiplier" ), -1, int ( * )( c_item* ) );
			return safe.call< int >( ptr, this );
		}

		inline float despawn_duration( ) {
			const auto& inf = info( );
			if ( !memory::is_valid( inf ) )
				return 300.f;

			if ( inf->quick_despawn( ) )
				return 30.f;

			return 300.f * despawn_multiplier( );
		}

		inline auto set_block_sprint( bool state ) {
			const auto& entity = held_entity( );
			if ( !memory::is_valid( entity ) )
				return;

			entity->set_block_sprint( state );
		}

		inline auto set_attack_max_distance( float value ) {
			const auto& entity = held_entity( );
			if ( !memory::is_valid( entity ) )
				return;

			entity->set_max_distance( value );
			entity->set_attack_radius( value );
		}

		inline auto set_attack_ready( bool state ) {
			const auto& item_name = info( )->shortname( )->string( );
			if ( item_name.find( _( "bow.compound" ) ) != std::string::npos
				 || item_name.find( _( "cross" ) ) != std::string::npos )
				return;

			const auto& entity = held_entity( );
			if ( !memory::is_valid( entity ) )
				return;

			entity->set_attack_ready( state );
		}

		inline auto set_was_aiming( bool state ) {
			const auto& item_name = info( )->shortname( )->string( );
			if ( item_name.find( _( "bow.compound" ) ) != std::string::npos
				 || item_name.find( _( "cross" ) ) != std::string::npos )
				return;

			const auto& entity = held_entity( );
			if ( !memory::is_valid( entity ) )
				return;

			entity->set_was_aiming( state );
		}

		inline auto set_always_eoka( float value ) {
			const auto& item_name = info( )->shortname( )->string( );
			if ( item_name.find( _( "eoka" ) ) == std::string::npos )
				return;

			const auto& entity = held_entity( );
			if ( !memory::is_valid( entity ) )
				return;

			entity->set_success_fraction( value );
		}

		inline auto set_did_spark_this_frame( bool state ) {
			const auto& item_name = info( )->shortname( )->string( );
			if ( item_name.find( _( "eoka" ) ) == std::string::npos )
				return;

			const auto& entity = held_entity( );
			if ( !memory::is_valid( entity ) )
				return;

			entity->set_did_spark_this_frame( state );
		}

		inline auto set_repeat_delay( float value ) {
			const auto& entity = held_entity( );
			if ( !memory::is_valid( entity ) )
				return;

			entity->set_repeat_delay( value );
		}
		
		inline auto set_current_hold_progress( float value ) {
			const auto& item_name = info( )->shortname( )->string( );
			if ( item_name.find( _( "bow.compound" ) ) == std::string::npos )
				return;

			const auto& entity = held_entity( );
			if ( !memory::is_valid( entity ) )
				return;

			entity->set_current_hold_progress( value );
		}

		inline auto set_string_bonus_damage( float value ) {
			const auto& item_name = info( )->shortname( )->string( );
			if ( item_name.find( _( "bow.compound" ) ) == std::string::npos )
				return;

			const auto& entity = held_entity( );
			if ( !memory::is_valid( entity ) )
				return;

			entity->set_string_bonus_damage( value );
		}

		inline auto set_movement_penalty_ramp_up_time( float value ) {
			const auto& item_name = info( )->shortname( )->string( );
			if ( item_name.find( _( "bow.compound" ) ) == std::string::npos )
				return;

			const auto& entity = held_entity( );
			if ( !memory::is_valid( entity ) )
				return;

			entity->set_movement_penalty_ramp_up_time( value );
		}

		inline auto server_rpc( const char* rpc ) {
			const auto& entity = held_entity( );
			if ( !memory::is_valid( entity ) )
				return;

			entity->server_rpc( rpc );
		}

		inline auto send_signal_broadcast( e_signal signal ) {
			const auto& entity = held_entity( );
			if ( !memory::is_valid( entity ) )
				return;

			entity->send_signal_broadcast( signal );
		}

		inline auto send_broadcast_projectile( e_signal signal, const char* rpc ) {
			const auto& entity = held_entity( );
			if ( !memory::is_valid( entity ) )
				return;

			entity->send_signal_broadcast( signal, rpc );
		}

		inline auto set_is_burst_weapon( bool state ) {
			const auto& entity = held_entity( );
			if ( !memory::is_valid( entity ) )
				return;

			entity->set_is_burst_weapon( state );
		}

		inline auto set_automatic( bool state ) {
			const auto& entity = held_entity( );
			if ( !memory::is_valid( entity ) )
				return;

			entity->set_automatic( state );
		}

		inline float reload_time( ) {
			const auto& entity = held_entity( );
			if ( !memory::is_valid( entity ) )
				return 0.f;

			return entity->reload_time( );
		}

		inline bool recoil_min_max( vec2_t& pitch_bounds, vec2_t& yaw_bounds ) {
			auto weapon_id = info( )->itemid( );
			switch ( weapon_id ) {
			case -139037392:
				pitch_bounds.x = -2.5f;
				pitch_bounds.y = -3.5f;
				yaw_bounds.x = 1.5f;
				yaw_bounds.y = 2.5f;
				return true;
			case 1545779598:
				pitch_bounds.x = -2.5f;
				pitch_bounds.y = -3.5f;
				yaw_bounds.x = 1.5f;
				yaw_bounds.y = 2.5f;
				return true;
			case -1335497659:
				pitch_bounds.x = -2.5f;
				pitch_bounds.y = -3.5f;
				yaw_bounds.x = 1.5f;
				yaw_bounds.y = 2.5f;
				return true;
			case 1588298435:
				pitch_bounds.x = -2.f;
				pitch_bounds.y = -3.f;
				yaw_bounds.x = -4.f;
				yaw_bounds.y = 4.f;
				return true;
			case 884424049:
				pitch_bounds.x = -3.f;
				pitch_bounds.y = -6.f;
				yaw_bounds.x = -3.f;
				yaw_bounds.y = 3.f;
				return true;
			case 1965232394:
				pitch_bounds.x = -3.f;
				pitch_bounds.y = -6.f;
				yaw_bounds.x = -3.f;
				yaw_bounds.y = 3.f;
				return true;
			case 649912614:
				pitch_bounds.x = -3.f;
				pitch_bounds.y = -6.f;
				yaw_bounds.x = -1.f;
				yaw_bounds.y = 1.f;
				return true;
			case 442886268:
				pitch_bounds.x = -15.f;
				pitch_bounds.y = -20.f;
				yaw_bounds.x = 5.f;
				yaw_bounds.y = 10.f;
				return true;
			case  818877484:
				pitch_bounds.x = -2.f;
				pitch_bounds.y = -2.5f;
				yaw_bounds.x = -1.f;
				yaw_bounds.y = 1.f;
				return true;
			case -904863145:
				pitch_bounds.x = -2.f;
				pitch_bounds.y = -3.f;
				yaw_bounds.x = -0.5f;
				yaw_bounds.y = 0.5f;
				return true;
				//Snowball Gun
			case 1103488722:
				pitch_bounds.x = -2.f;
				pitch_bounds.y = -3.f;
				yaw_bounds.x = -1.f;
				yaw_bounds.y = 1.f;
				return true;
			case -41440462:
				pitch_bounds.x = -10.f;
				pitch_bounds.y = -14.f;
				yaw_bounds.x = 4.f;
				yaw_bounds.y = 8.f;
				break;
			case 1318558775:
				pitch_bounds.x = -1.f;
				pitch_bounds.y = -3.f;
				yaw_bounds.x = -1.f;
				yaw_bounds.y = 1.f;
				return true;
			case -1123473824:
				pitch_bounds.x = -15.f;
				pitch_bounds.y = -20.f;
				yaw_bounds.x = 5.f;
				yaw_bounds.y = 10.f;
				return true;
			case 1953903201:
				pitch_bounds.x = -3.f;
				pitch_bounds.y = -6.f;
				yaw_bounds.x = -1.f;
				yaw_bounds.y = 1.f;
				return true;
			case 1914691295:
				pitch_bounds.x = -2.f;
				pitch_bounds.y = -2.5f;
				yaw_bounds.x = -1.f;
				yaw_bounds.y = 1.f;
				return true;
			case 795371088:
				pitch_bounds.x = -10.f;
				pitch_bounds.y = -14.f;
				yaw_bounds.x = 4.f;
				yaw_bounds.y = 8.f;
				return true;
			case 1373971859:
				pitch_bounds.x = -15.f;
				pitch_bounds.y = -16.f;
				yaw_bounds.x = -2.f;
				yaw_bounds.y = 2.f;
				return true;
			case -73195037:
				pitch_bounds.x = -3.f;
				pitch_bounds.y = -6.f;
				yaw_bounds.x = -3.f;
				yaw_bounds.y = 3.f;
				break;
			case -1812555177:
				pitch_bounds.x = -2.f;
				pitch_bounds.y = -3.f;
				yaw_bounds.x = -1.f;
				yaw_bounds.y = 1.f;
				return true;
			case -2069578888:
				pitch_bounds.x = -3.f;
				pitch_bounds.y = -4.f;
				yaw_bounds.x = 1.25f;
				yaw_bounds.y = 2.25f;
				return true;
			case 28201841:
				pitch_bounds.x = -3.f;
				pitch_bounds.y = -4.f;
				yaw_bounds.x = 1.5f;
				yaw_bounds.y = 2.5f;
				return true;
			case 678698219:
				pitch_bounds.x = -7.f;
				pitch_bounds.y = -10.f;
				yaw_bounds.x = 2.f;
				yaw_bounds.y = 4.f;
				return true;
			case -852563019:
				pitch_bounds.x = -7.f;
				pitch_bounds.y = -8.f;
				yaw_bounds.x = -1.f;
				yaw_bounds.y = 1.f;
				return true;
			case 1796682209:
				pitch_bounds.x = -1.5f;
				pitch_bounds.y = -2.f;
				yaw_bounds.x = -1.f;
				yaw_bounds.y = 1.f;
				return true;
			case -765183617:
				pitch_bounds.x = -10.f;
				pitch_bounds.y = -15.f;
				yaw_bounds.x = 8.f;
				yaw_bounds.y = 15.f;
				return true;
			case -1214542497:
				pitch_bounds.x = -3.f;
				pitch_bounds.y = -4.f;
				yaw_bounds.x = -1.25f;
				yaw_bounds.y = -2.5f;
				return true;
			case -218009552:
				pitch_bounds.x = -8.f;
				pitch_bounds.y = -8.f;
				yaw_bounds.x = 8.f;
				yaw_bounds.y = 8.f;
				return true;
			case 1443579727:
				pitch_bounds.x = -3.f;
				pitch_bounds.y = -6.f;
				yaw_bounds.x = -3.f;
				yaw_bounds.y = 3.f;
				break;
			case -778367295:
				pitch_bounds.x = -1.f;
				pitch_bounds.y = -1.5f;
				yaw_bounds.x = -2.f;
				yaw_bounds.y = 2.f;
				return true;
			case -1517740219:
				pitch_bounds.x = -3.f;
				pitch_bounds.y = -6.f;
				yaw_bounds.x = -3.f;
				yaw_bounds.y = 3.f;
				return true;
			case -1758372725:
				pitch_bounds.x = -1.5f;
				pitch_bounds.y = -2.f;
				yaw_bounds.x = -1.f;
				yaw_bounds.y = 1.f;
				return true;
			case -1367281941:
				pitch_bounds.x = -10.f;
				pitch_bounds.y = -14.f;
				yaw_bounds.x = 4.f;
				yaw_bounds.y = 8.f;
				return true;
			}

			return false;
		}

		inline auto set_recoil_min_max( float value ) {
			vec2_t pitch_bounds = vec2_t( ), yaw_bounds = vec2_t( );

			const auto& entity = held_entity( );
			if ( !memory::is_valid( entity ) )
				return;

			const auto& recoil = entity->recoil( );
			if ( !memory::is_valid( recoil ) )
				return;

			if ( recoil_min_max( pitch_bounds, yaw_bounds ) ) {
				recoil->set_recoil_yaw_min( yaw_bounds.x * value );
				recoil->set_recoil_yaw_max( yaw_bounds.y * value );
				recoil->set_recoil_pitch_min( pitch_bounds.x * value );
				recoil->set_recoil_pitch_max( pitch_bounds.y * value );

				const auto& new_recoil = recoil->new_recoil_override( );
				if ( memory::is_valid( new_recoil ) ) {
					new_recoil->set_recoil_yaw_min( yaw_bounds.x * value );
					new_recoil->set_recoil_yaw_max( yaw_bounds.y * value );
					new_recoil->set_recoil_pitch_min( pitch_bounds.x * value );
					new_recoil->set_recoil_pitch_max( pitch_bounds.y * value );
				}
			}
		}

		inline auto set_aim_sway( float value ) {
			if ( !is_weapon( ) )
				return;

			const auto& entity = held_entity( );
			if ( !memory::is_valid( entity ) )
				return;

			entity->set_aim_sway( value );
		}

		inline auto set_aim_sway_speed( float value ) {
			if ( !is_weapon( ) )
				return;

			const auto& entity = held_entity( );
			if ( !memory::is_valid( entity ) )
				return;

			entity->set_aim_sway_speed( value );
		}

		inline auto set_aim_cone( float value ) {
			if ( !is_weapon( ) )
				return;

			const auto& entity = held_entity( );
			if ( !memory::is_valid( entity ) )
				return;

			entity->set_aim_cone( value );
		}

		inline auto set_aim_penalty_recover_delay( float value ) {
			if ( !is_weapon( ) )
				return;

			const auto& entity = held_entity( );
			if ( !memory::is_valid( entity ) )
				return;

			entity->set_aimcone_penalty_recover_delay( value );
		}

		inline auto set_aim_penalty_recover_time( float value ) {
			if ( !is_weapon( ) )
				return;

			const auto& entity = held_entity( );
			if ( !memory::is_valid( entity ) )
				return;

			entity->set_aimcone_penalty_recover_time( value );
		}

		inline auto set_aim_sight_aim_cone_scale( float value ) {
			if ( !is_weapon( ) )
				return;

			const auto& entity = held_entity( );
			if ( !memory::is_valid( entity ) )
				return;

			entity->set_hip_aim_cone_scale( value );
		}

		inline auto set_aim_sight_aim_cone_offset( float value ) {
			if ( !is_weapon( ) )
				return;

			const auto& entity = held_entity( );
			if ( !memory::is_valid( entity ) )
				return;

			entity->set_sight_aim_cone_offset( value );
		}

		inline auto set_stance_penalty( float value ) {
			if ( !is_weapon( ) )
				return;

			const auto& entity = held_entity( );
			if ( !memory::is_valid( entity ) )
				return;

			entity->set_stance_penalty( value );
		}

		inline auto set_aim_cone_penalty_max( float value ) {
			if ( !is_weapon( ) )
				return;

			const auto& entity = held_entity( );
			if ( !memory::is_valid( entity ) )
				return;

			entity->set_aim_cone_penalty_max( value );
		}

		inline auto set_aim_cone_penalty_per_shot( float value ) {
			if ( !is_weapon( ) )
				return;

			const auto& entity = held_entity( );
			if ( !memory::is_valid( entity ) )
				return;

			entity->set_aimcone_penalty_per_shot( value );
		}

		inline auto set_stance_penalty_scale( float value ) {
			if ( !is_weapon( ) )
				return;

			const auto& entity = held_entity( );
			if ( !memory::is_valid( entity ) )
				return;

			entity->set_stance_penalty_scale( value );
		}

		inline auto set_hip_aim_cone( float value ) {
			if ( !is_weapon( ) )
				return;

			const auto& entity = held_entity( );
			if ( !memory::is_valid( entity ) )
				return;

			entity->set_hip_aim_cone( value );
		}

		inline auto set_aim_cone_penalty( float value ) {
			if ( !is_weapon( ) )
				return;

			const auto& entity = held_entity( );
			if ( !memory::is_valid( entity ) )
				return;

			entity->set_aimcone_penalty( value );
		}

		inline auto set_hip_aim_cone_scale( float value ) {
			if ( !is_weapon( ) )
				return;

			const auto& entity = held_entity( );
			if ( !memory::is_valid( entity ) )
				return;

			entity->set_hip_aim_cone_scale( value );
		}

		inline auto set_hip_aim_cone_offset( float value ) {
			if ( !is_weapon( ) )
				return;

			const auto& entity = held_entity( );
			if ( !memory::is_valid( entity ) )
				return;

			entity->set_hip_aim_cone_offset( value );
		}

		inline auto set_projectile_velocity_scale( float value ) {
			const auto& entity = held_entity( );
			if ( !memory::is_valid( entity ) )
				return;

			entity->set_projectile_velocity_scale( value );
		}

		inline auto set_distance_scale( float value ) {
			const auto& entity = held_entity( );
			if ( !memory::is_valid( entity ) )
				return;

			entity->set_distance_scale( value );
		}
	};
}
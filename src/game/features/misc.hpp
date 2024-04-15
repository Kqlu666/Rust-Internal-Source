#pragma once
#include <algorithm>

#include "../../vcruntime/core.hpp"

namespace features {
	int current_hour;
	int current_minute;

	float flyhack_pause_time;
	float flyhack_vertical;
	float flyhack_horizontal;
	float flyhack_max_vertical;
	float flyhack_max_horizontal;
	float flyhack_distance_vertical = 0.f;
	float flyhack_distance_horizontal = 0.f;

	float speedhack_pause_time;
	float speedhack;
	float speedhack_max;
	float speedhack_distance = 0.f;

	bool just_shot = false;
	bool did_reload = false;
	float reload_time = 0.f;
	float time_since_last_shot = 0.f;
	float fixed_time_last_shot = 0.f;

	c_time_average_value_data counter = { 0, 0, 0 };
	c_time_average_value_data bullet_queue_counter = { 0, 0, 0 };

	class c_misc : public singleton< c_misc > {
	public:
		inline void do_melee_attack( sdk::c_base_player* player, sdk::c_held_entity* base_projectile ) {
			if ( base_projectile->next_attack_time( ) >= unity::c_time::time( ) )
				return;

			if ( base_projectile->time_since_deploy( ) < base_projectile->deploy_delay( ) )
				return;

			const auto& camera_transform = sdk::camera->transform( );
			if ( !memory::is_valid( camera_transform ) )
				return;

			const auto& model = player->model( );
			if ( !memory::is_valid( model ) )
				return;

			const auto& transform = model->is_npc( ) ? player->transform( ) : player->bone_transform( sdk::e_bone::head );
			if ( !memory::is_valid( transform ) )
				return;

			const auto& hit_test = il2cpp::new_object< sdk::c_hit_test* >( _( "" ), _( "HitTest" ) );
			if ( !memory::is_valid( hit_test ) )
				return;

			const auto& position = transform->position( );

			const auto& transform_pos = transform->inverse_transform_point( position );

			hit_test->set_max_distance( config::misc_weapon_extended_melee ? base_projectile->max_distance( ) + config::misc_weapon_extended_melee_radius : base_projectile->max_distance( ) );
			hit_test->set_hit_transform( transform );

			const auto& camera_position = camera_transform->position( );
			unity::ray_t ray = { camera_position, ( position - camera_position ).normalized( ) };
			hit_test->set_attack_ray( ray );

			hit_test->set_did_hit( true );
			hit_test->set_hit_entity( player );

			hit_test->set_hit_point( transform_pos );
			hit_test->set_hit_normal( vec3_t{ } );
			hit_test->set_damage_properties( base_projectile->damage_properties( ) );

			base_projectile->start_attack_cooldown( base_projectile->repeat_delay( ) );
			base_projectile->process_attack( hit_test );
		}

		inline void do_simulate_projectile( vec3_t position, vec3_t velocity, float partial_time, float travel_time, vec3_t gravity, float drag, vec3_t prev_position, vec3_t prev_velocity ) {
			float num = 0.03125f;
			prev_position = position;
			prev_velocity = velocity;
			if ( partial_time > 0 ) {
				float num2 = num - partial_time;
				if ( travel_time < num2 ) {
					prev_position = position;
					prev_velocity = velocity;
					position += velocity * travel_time;
					partial_time += travel_time;
					return;
				}

				prev_position = position;
				prev_velocity = velocity;
				position += velocity * num2;
				velocity += gravity * num;
				velocity -= velocity * ( drag * num );
				travel_time -= num2;
			}

			int num3 = int( travel_time / num );
			for ( int i = 0; i < num3; i++ ) {
				prev_position = position;
				prev_velocity = velocity;
				position += velocity * num;
				velocity += gravity * num;
				velocity -= velocity * ( drag * num );
			}

			partial_time = travel_time - num * ( float )num3;
			if ( partial_time > 0 ) {
				prev_position = position;
				prev_velocity = velocity;
				position += velocity * partial_time;
			}
		}

		class c_line {
		public:

			vec3_t start;
			vec3_t end;

			c_line( ) { }

			c_line( vec3_t s, vec3_t e ) {
				start = s; end = e;
			}

			float clamp22( float value, float min, float max ) {
				if ( value < min ) {
					value = min;
				} else if ( value > max ) {
					value = max;
				}
				return value;
			}

			float dot( const vec3_t& start, const vec3_t& end ) {
				return start.x * end.x + start.y * end.y + start.z * end.z;
			}

			vec3_t closest_point( vec3_t pos ) {
				vec3_t a = end - start;
				float magnitude = a.length( );
				if ( magnitude == 0.f ) return start;
				vec3_t vector = a / magnitude;
				return start + vector * clamp22( dot( pos - start, vector ), 0.f, magnitude );
			}

			float distance( vec3_t pos ) {
				return ( pos - closest_point( pos ) ).magnitude2d( );
			}
		};

		inline sdk::c_hit_info* do_create_hit_info( sdk::c_projectile* projectile, sdk::c_player_projectile_attack* player_projectile_attack, sdk::c_hit_test* hit_test, vec3_t hit_point_world ) {
			if ( !memory::is_valid( player_projectile_attack ) )
				return {};

			if ( !memory::is_valid( projectile ) )
				return {};

			if ( !memory::is_valid( hit_test ) )
				return {};

			const auto& target = aimbot.get_target( );
			if ( !memory::is_valid( target.player ) )
				return {};
			
			const auto& player_attack = il2cpp::new_object< sdk::c_player_attack* >( _( "ProtoBuf" ), _( "PlayerAttack" ) );
			if ( memory::is_valid( player_attack ) ) {
				player_projectile_attack->set_player_attack( player_attack );
				player_projectile_attack->set_hit_velocity( projectile->current_velocity( ) );
				player_projectile_attack->set_hit_distance( projectile->traveled_distance( ) );
			}

			player_projectile_attack->player_attack( )->set_attack( hit_test->build_attack_message( sdk::local_player ) );
			player_projectile_attack->player_attack( )->set_projectile_id( projectile->projectile_id( ) );
			
			const auto& hit_info = il2cpp::new_object< sdk::c_hit_info* >( _( "" ), _( "HitInfo" ) );
			if ( memory::is_valid( hit_info ) ) {
				hit_info->load_from_attack( player_projectile_attack->player_attack( )->attack( ), false );
				hit_info->set_initiator( projectile->owner( ) );
				hit_info->set_projectile_id( projectile->projectile_id( ) );
				hit_info->set_projectile_distance( projectile->traveled_distance( ) );
				hit_info->set_projectile_velocity( projectile->current_velocity( ) );
				hit_info->set_projectile_prefab( projectile->source_projectile_prefab( ) );
				hit_info->set_is_predicting( true );
				hit_info->set_weapon_prefab( projectile->source_weapon_prefab( ) );
				hit_info->set_do_decals( projectile->create_decals( ) );

				return hit_info;
			}
		}

		inline bool do_bullet_tp( sdk::c_projectile* instance, vec3_t next_current_position, vec3_t current_position, vec3_t current_velocity, float delta_time ) {
			if ( instance->projectile_id( ) == 0 )
				return false;

			if ( instance->integrity( ) <= 0.f )
				return false;

			if ( bullet_queue_counter.calculate( ) > 100 )
				return false;

			const auto& target = aimbot.get_target( );
			if ( !memory::is_valid( target.player ) )
				return false;

			auto line = next_current_position - current_position;
			auto line_length = line.magnitude2d( );
			line.unity_normalize( );

			auto center_position = target.player->pivot_point( ) + vec3_t( 0.f, 0.9f, 0.f );
			auto v = center_position - current_position;
			auto d = v.dot( line );

			if ( d < 0.0f ) {
				d = 0.0f;
			} else if ( d > line_length ) {
				d = line_length;
			}

			auto orig_closest_point = current_position + line * d;
			auto closest_point_online = orig_closest_point;

			auto closest_point = target.player->closest_point( closest_point_online );
			auto orig_distance = target.player->transform( )->position( ).distance_to( closest_point_online );
			auto distance = orig_distance;

			vec3_t hit_point_world = closest_point;

			if ( target.player->mounted( ) ) {
				if ( distance > 7.5f )
					return false;
			} else {
				if ( distance > 3.f )
					return false;
			}

			if ( distance > 1.2f ) {
				auto end_position_trajectory_usage = distance - 1.2f + 0.01f;
				auto amount_needed = end_position_trajectory_usage / distance;
				auto direction = hit_point_world - closest_point_online;
				auto new_position = closest_point_online + ( direction * amount_needed );

				if ( closest_point_online.distance_to( new_position ) > 1.f || !sdk::c_game_physics::is_visible( closest_point_online, new_position, 0.f ) )
					return false;

				closest_point_online = new_position;
				distance = target.player->transform( )->position( ).distance_to( closest_point_online );
			}

			if ( distance > 1.f ) {
				auto player_distance_usage = std::min( 0.19f, distance - 1.f );
				auto amount_needed = 1.f - player_distance_usage / distance;
				auto direction = hit_point_world - closest_point_online;

				auto new_position = closest_point_online + ( direction * amount_needed );
				hit_point_world = new_position;
			}

			int num15 = 2162688;
			num15 |= 8388608;
			num15 |= 134217728;

			if ( !sdk::c_game_physics::is_visible( closest_point_online, hit_point_world, 0.f ) || !sdk::c_game_physics::is_visible( current_position, orig_closest_point, 0.f ) ||
				 !sdk::c_game_physics::is_visible( orig_closest_point, closest_point_online, 0.f ) || !sdk::c_game_physics::is_visible( center_position, hit_point_world, 0.f ) )
				return false;

			instance->set_previous_position( instance->current_position( ) );
			instance->set_current_position( orig_closest_point );
			instance->set_traveled_distance( instance->traveled_distance( ) + instance->previous_position( ).distance_to( instance->current_position( ) ) );
			instance->set_traveled_time( instance->traveled_time( ) + instance->previous_position( ).distance_to( instance->current_position( ) ) / current_velocity.length( ) );

			const auto& player_projectile_update = il2cpp::new_object< sdk::c_player_projectile_update* >( _( "ProtoBuf" ), _( "player_projectile_update" ) );
			if ( memory::is_valid( player_projectile_update ) ) {
				player_projectile_update->set_projectile_id( instance->projectile_id( ) );
				player_projectile_update->set_travel_time( instance->traveled_time( ) );
				player_projectile_update->set_cur_velocity( instance->current_velocity( ) );
				player_projectile_update->set_cur_position( instance->current_position( ) );

				vec3_t position2 = instance->current_position( );
				vec3_t current_position = player_projectile_update->cur_position( );
				vec3_t b2 = ( current_position - position2 ).unity_normalize( ) * 0.01f;

				int num14 = 2162688;
				num14 |= 8388608;
				num14 |= 134217728;

				if ( !sdk::c_game_physics::line_of_sight( position2 - b2, current_position + b2, num14, nullptr ) )
					return false;

				instance->owner( )->send_projectile_update( player_projectile_update );
				counter.increment( );
			}

			instance->set_previous_position( instance->current_position( ) );
			instance->set_current_position( closest_point_online );
			instance->set_traveled_distance( instance->traveled_distance( ) + instance->previous_position( ).distance_to( instance->current_position( ) ) );
			instance->set_traveled_time( instance->traveled_time( ) + instance->previous_position( ).distance_to( instance->current_position( ) ) / current_velocity.length( ) );

			const auto& player_projectile_update_second = il2cpp::new_object< sdk::c_player_projectile_update* >( _( "ProtoBuf" ), _( "PlayerProjectileUpdate" ) );
			if ( memory::is_valid( player_projectile_update_second ) ) {
				player_projectile_update_second->set_projectile_id( instance->projectile_id( ) );
				player_projectile_update_second->set_travel_time( instance->traveled_time( ) );
				player_projectile_update_second->set_cur_velocity( instance->current_velocity( ) );
				player_projectile_update_second->set_cur_position( instance->current_position( ) );

				vec3_t position2 = instance->current_position( );
				vec3_t current_position = player_projectile_update_second->cur_position( );
				vec3_t b2 = ( current_position - position2 ).unity_normalize( ) * 0.01f;

				int num14 = 2162688;
				num14 |= 8388608;
				num14 |= 134217728;

				if ( !sdk::c_game_physics::line_of_sight( position2 - b2, current_position + b2, num14, nullptr ) )
					return false;

				instance->owner( )->send_projectile_update( player_projectile_update_second );
				counter.increment( );
			}

			auto hit_test = instance->hit_test( );
			if ( !memory::is_valid( hit_test ) )
				hit_test = il2cpp::new_object< sdk::c_hit_test* >( _( "" ), _( "HitTest" ) );

			if ( hit_point_world == instance->current_position( ) )
				hit_point_world.y += 0.01f;

			unity::ray_t ray = { instance->current_position( ), ( hit_point_world - instance->current_position( ) ).unity_normalize( ) };

			hit_test->set_max_distance( 1000.0f );
			hit_test->set_attack_ray( ray );
			hit_test->set_did_hit( true );
			hit_test->set_hit_transform( target.player->bone_transform( target.bone ) );
			hit_test->set_hit_entity( target.player );
			hit_test->set_hit_point( hit_test->hit_transform( )->inverse_transform_point( hit_point_world ) );
			hit_test->set_hit_normal( hit_test->hit_transform( )->inverse_transform_direction( hit_point_world ) );
			hit_test->set_damage_properties( instance->damage_properties( ) );
			hit_test->set_hit_material( ( unity::c_string* )il2cpp::new_string( _( "Flesh" ) ) );

			const auto& player_projectile_attack = il2cpp::new_object< sdk::c_player_projectile_attack* >( _( "ProtoBuf" ), _( "PlayerProjectileAttack" ) );
			if ( memory::is_valid( player_projectile_attack ) ) {
				const auto hit_info = do_create_hit_info( instance, player_projectile_attack, hit_test, hit_point_world );
				if ( memory::is_valid( hit_info ) ) {
					vec3_t vector2 = hit_info->position_on_ray( hit_point_world );

					vec3_t position2 = instance->current_position( );
					const auto point_start = hit_info->point_start( );
					vec3_t b2 = ( point_start - position2 ).unity_normalize( ) * 0.01f;
					vec3_t b3 = ( vector2 - point_start ).unity_normalize( ) * 0.01f;
					vec3_t vector = hit_info->hit_position_world( );

					bool flag10 = sdk::c_game_physics::line_of_sight( position2 - b2, point_start + b2, num15, nullptr ) &&
						sdk::c_game_physics::line_of_sight( point_start - b3, vector2, num15, nullptr ) && sdk::c_game_physics::line_of_sight( vector2, vector, num15, nullptr );;
					if ( !flag10 )
						return false;

					vec3_t hit_position_world = hit_info->hit_position_world( );

					vec3_t position3 = hit_point_world;
					vec3_t vector4 = instance->owner( )->world_space_bounds( ).position;

					bool flag11 = sdk::c_game_physics::is_visible( hit_position_world, position3 ) && sdk::c_game_physics::is_visible( position3, hit_position_world );
					if ( !flag11 )
						flag11 = ( sdk::c_game_physics::is_visible( hit_position_world, vector4 ) && sdk::c_game_physics::is_visible( vector4, hit_position_world ) );

					if ( !flag11 )
						return false;

					float time_since_last_tick = ( unity::c_time::realtime_since_startup( ) - sdk::local_player->last_sent_tick_time( ) );
					float last_tick_time = std::max( 0.f, std::min( time_since_last_tick, 1.f ) );

					float num = 1.5f;
					float eye_clientframes = 2.0f;
					float eye_serverframes = 2.0f;
					float num2 = eye_clientframes / 60.f;
					float num3 = eye_serverframes * std::max( std::max( unity::c_time::delta_time( ), unity::c_time::smooth_delta_time( ) ), unity::c_time::fixed_delta_time( ) );
					float num4 = ( last_tick_time + num2 + num3 ) * num;

					float num5 = sdk::local_player->max_speed( ) + sdk::local_player->parent_velocity( ).magnitude( );
					float num6 = sdk::local_player->bounds_padding( ) + num4 * num5;

					if ( instance->source_weapon_prefab( )->is_class( _( "BowWeapon" ) ) ) {
						if ( memory::is_valid( hit_test->hit_entity( ) ) ) {
							const auto& player = hit_test->hit_entity( );

							float magnitude = player->parent_velocity( ).magnitude2d( );
							float num19 = player->bounds_padding( ) + num4 * magnitude + 0.1f;
							float num20 = player->transform( )->position( ).distance_to( hit_info->hit_position_world( ) );
							if ( num20 > num19 )
								return false;
						}

						float num16 = hit_test->hit_entity( )->max_velocity( ) + hit_test->hit_entity( )->parent_velocity( ).magnitude( );
						float num17 = hit_test->hit_entity( )->bounds_padding( ) + num4 * num16;
						float num18 = hit_test->hit_entity( )->transform( )->position( ).distance_to( hit_info->hit_position_world( ) );
						if ( num18 > num17 )
							return false;
					}


					float travel = std::max( instance->traveled_time( ) - instance->close_flyby_distance( ), 0.f );
					vec3_t gr = unity::c_physics::gravity( );

					vec3_t a{ };
					vec3_t b{ };
					do_simulate_projectile( instance->current_position( ), instance->current_velocity( ), instance->tumble_speed( ), travel, gr * instance->gravity_modifier( ), instance->drag( ), a, b );

					float num25 = 0.f;
					if ( hit_test->hit_entity( ) != nullptr ) {
						float num26 = hit_test->hit_entity( )->parent_velocity( ).magnitude2d( );
						if ( hit_test->hit_entity( )->is_class( _( "TugBoat" ) ) )
							num26 += hit_test->hit_entity( )->max_velocity( );

						num25 = num6 * num26;
					}

					vec3_t position_offset{ };
					c_line line_1 = c_line( a - b, instance->current_position( ) + b );
					float num27 = std::max( line_1.distance( hit_info->point_start( ) ) - position_offset.magnitude2d( ) - num25, 0.f );
					float num28 = std::max( line_1.distance( hit_info->hit_position_world( ) ) - position_offset.magnitude2d( ) - num25, 0.f );

					if ( num27 > 1.f )
						return false;

					if ( num28 > 1.f )
						return false;

					bool flag6 = hit_test->hit_entity( ) != nullptr;
					float num14 = instance->current_position( ).distance_to( hit_info->hit_position_world( ) );
					float num21 = flag6 ? ( hit_test->hit_entity( )->max_velocity( ) + hit_test->hit_entity( )->parent_velocity( ).magnitude2d( ) ) : 0.f;
					float num22 = flag6 ? ( num6 * num21 ) : 0.f;
					float num24 = hit_info->projectile_distance( ) + 1.f + position_offset.magnitude2d( ) + num22;

					if ( num14 > num24 )
						return false;
				}

			}

			instance->do_hit( hit_test, hit_test->hit_point( ), hit_test->hit_normal( ) );
			return true;
		}

		inline void do_attack( sdk::c_held_entity* a1 ) {
			if ( a1->next_attack_time( ) >= unity::c_time::time( ) )
				return;

			if ( a1->time_since_deploy( ) < a1->deploy_delay( ) )
				return;

			if ( a1->primary_magazine( )->contents( ) <= 0 )
				return;

			if ( a1->has_attack_cooldown( ) )
				return;

			a1->launch_projectile( );
			a1->primary_magazine( )->set_contents( a1->primary_magazine( )->contents( ) - 1 );
			a1->update_ammo_display( );
			a1->shot_fired( );
			a1->did_attack_clientside( );
			a1->start_attack_cooldown( a1->repeat_delay( ) );
		}

		inline bool do_movement( float delta_time, sdk::c_projectile* projectile ) {
			const auto& owner = projectile->owner( );
			if ( !memory::is_valid( owner ) )
				return false;

			if ( owner->user_id( ) != sdk::local_player->user_id( ) )
				return false;

			vec3_t a = projectile->current_velocity( ) * delta_time;
			float magnitude = a.length( );
			float num2 = 1 / magnitude;
			vec3_t vec2 = a * num2;
			bool flag = false;

			vec3_t vec3 = projectile->current_position( ) + vec2 * magnitude;
			float num3 = projectile->traveled_time( ) + delta_time;

			float v38 = delta_time + projectile->traveled_time( );

			sdk::c_hit_test* hit_test = projectile->hit_test( );
			if ( !memory::is_valid( hit_test ) ) {
				hit_test = il2cpp::new_object< sdk::c_hit_test* >( _( "" ), _( "HitTest" ) );
				projectile->set_hit_test( hit_test );
			}

			unity::ray_t ray = { projectile->current_position( ), vec2 };
			hit_test->set_attack_ray( ray );
			hit_test->set_max_distance( magnitude );

			hit_test->set_ignore_entity( owner );
			hit_test->set_radius( 0.f );
			hit_test->set_forgiveness( 0.5f );

			if ( !projectile->owner( ) || owner->user_id( ) == projectile->owner( )->user_id( ) ) {
				hit_test->set_type( 0x2 );
			} else {
				hit_test->set_type( 0x1 );
			}

			if ( projectile->source_weapon_prefab( ) ) {
				hit_test->set_best_hit( true );
				hit_test->set_damage_properties( projectile->damage_properties( ) );
			}

		OFFSET:

			static const auto& game_assembly = pe::get_image( HASH( "gameassembly" ) );

			static const auto& ptr_method_addr = reinterpret_cast< uintptr_t( * )( ) >( game_assembly + 0x125DED0 ); // Method$Facepunch.Pool.GetList\u003CTraceInfo\u003E() MethodAddress

			const auto& st = mem::read< uintptr_t >( game_assembly + 0x346F620 ); // Method$Facepunch.Pool.GetList\u003CTraceInfo\u003E() address
			if ( !memory::is_valid( st ) )
				goto OFFSET;

			const auto& rs = safe.call< unity::c_list< sdk::trace_info_t >* >( ptr_method_addr, st );
			if ( !memory::is_valid( rs ) )
				goto OFFSET;

			int layer_mask = 1270440721;

			//{
			//	layer_mask &= ~256; //Deployed
			//	layer_mask &= ~1; //Default
			//	layer_mask &= ~1073741824; //tree
			//	layer_mask &= ~16; //Water
			//	layer_mask &= ~134217728; //Vehicle_Large
			//	layer_mask &= ~32768; //Vehicle_World
			//	layer_mask &= ~8192; //Vehicle_Detailed
			//	layer_mask &= ~1073741824; //tree
			//}

			sdk::c_game_trace::trace_all( hit_test, rs, layer_mask );

			static bool traceall = false;

			hit_test = projectile->hit_test( );
			hit_test->set_max_distance( 0.f );

			const auto& target = aimbot.get_target( );
			if ( !memory::is_valid( target.player ) )
				return false;

			float real_travel_time = projectile->traveled_time( );
			vec3_t vel = projectile->current_velocity( );

			if ( do_bullet_tp( projectile, vec3, projectile->current_position( ), projectile->current_velocity( ), delta_time ) ) {
				counter.reset( );

				projectile->set_integrity( 0 );
				flag = true;
				return false;
			}

			for ( int i = 0; i < rs->count( ) && projectile->is_alive( ) && !flag; i++ ) {
				const auto& trace = rs->value( i );
				if ( trace.valid ) {
					projectile->update_hit_test( trace );

					vec3_t vec4 = hit_test->hit_point_world( );
					vec3_t normal = hit_test->hit_normal_world( );

					float magnitude2 = ( vec4 - projectile->current_position( ) ).length( );
					float num5 = magnitude2 * num2 * delta_time;

					projectile->set_traveled_distance( projectile->traveled_distance( ) + magnitude2 );
					projectile->set_traveled_time( projectile->traveled_time( ) + num5 );
					projectile->set_current_position( vec4 );

					bool exit = false;
					if ( projectile->do_hit( hit_test, vec4, normal ) )
						flag = true;

					if ( exit )
						return true;
				}
			}

			if ( projectile->integrity( ) > 0.001 && projectile->max_distance( ) > projectile->traveled_distance( ) ) {
				if ( flag && projectile->traveled_time( ) < num3 ) {
					do_movement( num3 - projectile->traveled_time( ), projectile );
					return false;
				}

				if ( !flag && projectile->is_alive( ) ) {
					float magnitude3 = ( vec3 - projectile->current_position( ) ).length( );
					float num6 = magnitude3 * num2 * delta_time;
					projectile->set_traveled_distance( projectile->traveled_distance( ) + magnitude3 );
					projectile->set_traveled_time( projectile->traveled_time( ) + num6 );
					projectile->set_current_position( vec3 );
				}
			}
		}

		inline void do_velocity_update( float delta_time, sdk::c_projectile* projectile ) {
			vec3_t gr = unity::c_physics::gravity( );

			vec3_t tr = ( gr * projectile->gravity_modifier( ) * delta_time );
			projectile->set_current_velocity( projectile->current_velocity( ) + tr );

			vec3_t dr = ( projectile->current_velocity( ) * projectile->drag( ) * delta_time );
			projectile->set_current_velocity( projectile->current_velocity( ) - dr );
		}

		inline bool is_no_clipping( vec3_t old_pos, vec3_t new_pos ) {
			flyhack_pause_time = std::max( 0.f, flyhack_pause_time - unity::c_time::fixed_delta_time( ) );
			bool in_air = false;

			float radius = sdk::local_player->radius( );
			float height = sdk::local_player->height( false );

			vec3_t vector = ( old_pos + new_pos ) * 0.5f;
			vec3_t vector2 = vector + vec3_t( 0.f, radius - 2.f, 0.f );
			vec3_t vector3 = vector + vec3_t( 0.f, height - radius, 0.f );
			float radius2 = radius - 0.05f;

			const auto a = unity::c_physics::check_capsule( vector2, vector3, radius2, 1503731969, unity::e_query_trigger_interaction::ignore );
			in_air = !a && !sdk::c_water_level::test( vector - vec3_t( 0.f, 2.f, 0.f ), true, false, sdk::local_player );

			if ( in_air && 
				 !sdk::local_player->is_sleeping( ) && 
				 !sdk::local_player->has_parent( ) && 
				 !sdk::local_player->on_ladder( ) && 
				 !sdk::local_player->model_state( )->has_flag( sdk::e_model_state_flags::is_mounted ) ) {
				bool flag = false;

				vec3_t vector4 = new_pos - old_pos;
				float num3 = abs( vector4.y );
				float num4 = vector4.magnitude2d( );

				if ( vector4.y >= 0.f ) {
					flyhack_distance_vertical += vector4.y;
					flag = true;
				}

				if ( num3 < num4 ) {
					flyhack_distance_horizontal += num4;
					flag = true;
				}

				if ( flag ) {
					float num5 = std::max( ( flyhack_pause_time > 0.f ) ? 10.f : 1.5f, 0.f );
					float num6 = sdk::local_player->jump_height( ) + num5 + 3.2f;
					if ( flyhack_distance_vertical > num6 )
						return true;

					float num7 = std::max( ( flyhack_pause_time > 0.f ) ? 10.f : 1.5f, 0.f );
					float num8 = 5.f + num7 + 3.2f;
					if ( flyhack_distance_horizontal > num8 )
						return true;
				}
			} else {
				flyhack_distance_horizontal = 0.f;
				flyhack_distance_vertical = 0.f;
			}

			return false;
		}

		inline void on_check_flyhack( ) {
			if ( sdk::local_player->is_dead( ) || sdk::local_player->is_sleeping( ) )
				return;

			const auto& last_sent_tick = sdk::local_player->last_sent_tick( );
			if ( !memory::is_valid( last_sent_tick ) )
				return;

			const auto& transform = sdk::local_player->transform( );
			if ( !memory::is_valid( transform ) )
				return;

			const auto& old_pos = last_sent_tick->position( );
			const auto& model_pos = transform->position( );

			const auto result = is_no_clipping( old_pos, model_pos );

			float num5 = std::max( ( flyhack_pause_time > 0.f ) ? 10.f : 1.5f, 0.f );
			float num6 = sdk::local_player->jump_height( ) + num5 + 3.2f;

			flyhack_vertical = flyhack_distance_vertical;
			flyhack_max_vertical = num6;

			float num7 = std::max( ( flyhack_pause_time > 0.f ) ? 10.f : 1.5f, 0.f );
			float num8 = 5.f + num7;

			flyhack_horizontal = flyhack_distance_horizontal;
			flyhack_max_horizontal = num8 + 3.2f;

			if ( result && config::exploits_anti_fly_hack_kick ) {
				const auto& player_movement = sdk::local_player->walk_movement( );
				if ( memory::is_valid( player_movement ) ) {

					const auto& position = last_sent_tick->position( );
					player_movement->set_target_movement( vec3_t( ) );
					player_movement->teleport_to( vec3_t( position.x, position.y - 0.001f, position.z ) );

					const auto& rigid_body = player_movement->body( );
					if ( memory::is_valid( rigid_body ) )
						rigid_body->set_velocity( vec3_t( ) );
				}
			}
		}

		inline bool is_speeding( vec3_t old_pos, vec3_t new_pos ) {
			bool flag;
			speedhack_pause_time = std::max( 0.f, speedhack_pause_time - unity::c_time::fixed_delta_time( ) );

			float num = 1.f;
			float num2 = 0.f;
			float num3 = 0.f;

			bool flag3 = sdk::local_player->is_running( );
			bool flag4 = sdk::local_player->is_ducked( );
			bool flag5 = sdk::local_player->is_swimming( );
			bool flag6 = sdk::local_player->is_crawling( );
			num = ( flag3 ? 1.f : 0.f );
			num2 = ( ( flag4 || flag5 ) ? 1.f : 0.f );
			num3 = ( flag6 ? 1.f : 0.f );

			float speed = sdk::local_player->speed( num, num2, num3 );
			TRACE( "speed: %.f", speed );

			vec3_t vector3 = new_pos - old_pos;

			float num4 = vector3.magnitude2d( );
			float num5 = unity::c_time::fixed_delta_time( ) * speed;
			if ( num4 > num5 ) {
				vec3_t vector4 = ( sdk::c_terrain_meta::height_map( ) ? sdk::c_terrain_meta::height_map( )->normal( old_pos ) : vec3_t( 0.f, 1.f, 0.f ) );
				float num6 = std::max( 0.f, vector4.xz3d( ).dot( vector3.xz3d( ) ) ) * 10.f * unity::c_time::fixed_delta_time( );
				num4 = std::max( 0.f, num4 - num6 );
			}

			float num7 = std::max( ( speedhack_pause_time > 0.f ) ? 10.f : 2.f, 0.1f );
			float num8 = num7 + std::max( 2.f, 0.1f );
			speedhack_distance = vec3_t::clamp( speedhack_distance, -num8, num8 );
			speedhack_distance = vec3_t::clamp( speedhack_distance - num5, -num8, num8 );

			if ( speedhack_distance > num7 ) {
				flag = true;
			} else {
				speedhack_distance = vec3_t::clamp( speedhack_distance + num4, -num8, num8 );
				if ( speedhack_distance > num7 ) {
					flag = true;
				} else {
					flag = false;
				}
			}

			return flag;
		}

		inline void on_check_speedhack( ) {
			if ( sdk::local_player->is_dead( ) || sdk::local_player->is_sleeping( ) )
				return;

			const auto& last_sent_tick = sdk::local_player->last_sent_tick( );
			if ( !memory::is_valid( last_sent_tick ) )
				return;

			const auto& transform = sdk::local_player->transform( );
			if ( !memory::is_valid( transform ) )
				return;

			const auto& old_pos = last_sent_tick->position( );
			const auto& new_pos = transform->position( );

			const auto result = is_speeding( old_pos, new_pos );

			float num7 = std::max( ( speedhack_pause_time > 0.f ) ? 10.f : 2.f, 0.1f );
			speedhack = speedhack_distance;
			speedhack_max = num7;

	/*		if ( result ) {
				const auto& player_movement = sdk::local_player->walk_movement( );
				if ( memory::is_valid( player_movement ) ) {

					const auto& position = last_sent_tick->position( );
					player_movement->set_target_movement( vec3_t( ) );
					player_movement->teleport_to( vec3_t( position.x, position.y - 0.001f, position.z ) );

					const auto& rigid_body = player_movement->body( );
					if ( memory::is_valid( rigid_body ) )
						rigid_body->set_velocity( vec3_t( ) );
				}
			}*/
		}

		inline void on_manipulation( ) {
			const auto& item = sdk::local_player->item( );
			if ( !memory::is_valid( item ) )
				return;

			const auto& held_entity = item->held_entity( );
			if ( !memory::is_valid( held_entity ) )
				return;

			vec3_t eye_pos = sdk::local_player->eyes( )->position( );

			float time_since_last_tick = ( unity::c_time::realtime_since_startup( ) - sdk::local_player->last_sent_tick_time( ) );
			float last_tick = std::max( 0.f, std::min( time_since_last_tick, 1.f ) );

			float num = 1.5f;
			float eye_clientframes = 2.0f;
			float eye_serverframes = 2.0f;
			float num2 = eye_clientframes / 60.f;
			float num3 = eye_serverframes * std::max( std::max( unity::c_time::delta_time( ), unity::c_time::smooth_delta_time( ) ), unity::c_time::fixed_delta_time( ) );
			float num4 = ( last_tick + num2 + num3 ) * num;

			float num5 = sdk::local_player->max_speed( ) + sdk::local_player->parent_velocity( ).magnitude( );
			float num6 = sdk::local_player->bounds_padding( ) + num4 * num5;

			max_spoofed_eye_distance = num6;

			if ( !held_entity->is_class( _( "BowWeapon" ) ) ) {
				auto desynctime = unity::c_time::realtime_since_startup( ) - sdk::local_player->last_sent_tick_time( );
				auto desyncpercentage = ( ( ( desynctime / 0.85f ) * 100.0f ) + 1.f ) / 100;

				if ( desyncpercentage >= 0.85f )
					instant_hit_ready = true;
				else
					instant_hit_ready = false;
			}

			if ( config::aimbot_manipulation && input_handler.is_hold( config::aimbot_manipulation_key ) ) {
				sdk::local_player->set_client_tick_interval( 0.99f );
			} else {
				sdk::local_player->set_client_tick_interval( 0.05f );
			}

			const auto& target = aimbot.get_target( );
			if ( memory::is_valid( target.player ) ) {

				static uint64_t current_target;
				static bool has_logged_target = false;

				if ( !has_logged_target ) {
					current_target = target.player->user_id( );
					has_logged_target = true;
				}

				if ( target.player->user_id( ) != current_target ) {
					cached_bullet_tp_position = { };
					cached_manip_point = eye_pos;
					manipulation_angle = eye_pos;
					point_visible = false;
					bullet_tp_point_visible = false;
					bullet_tp_angle = { };
					has_logged_target = false;
				}

				target_id = target.player->user_id( );

				if ( config::aimbot_manipulation ) {
					auto distance = sdk::local_player->transform( )->position( ).distance_to( cached_manip_point );

					if ( sdk::c_game_physics::is_visible( eye_pos, cached_manip_point ) &&
						 sdk::c_game_physics::is_visible( cached_manip_point, cached_bullet_tp_position )
						 && distance < 9
						 && !cached_manip_point.empty( )
						 && target.player->user_id( ) == target_id
						 && !target.player == NULL ) {
						point_visible = true;
					} else {
						cached_manip_point = sdk::local_player->eyes( )->position( );
						point_visible = false;
					}
				} else {
					cached_manip_point = eye_pos;
				}

				if ( config::aimbot_manipulation && input_handler.is_hold( config::aimbot_manipulation_key ) ) {
					aimbot.find_manipulation_angles( num6 );
				} else {
					point_visible = false;

					cached_manip_point = eye_pos;
					manipulation_angle = eye_pos;

					can_manipulate = false;
					start_shooting = false;
					manipulation_indicator = false;
				}

				if ( config::aimbot_bullet_tp ) {
					aimbot.find_bullet_tp_angles( num6 );
					target.player->transform( )->set_rotation( quat_t( 0.f, 0.f, 0.f, 1.f ) );
				} else if ( !bullet_tp_angle.empty( ) ) {
					cached_bullet_tp_position = target.pos;
					bullet_tp_angle = vec3_t( 0, 0, 0 );
					bullet_tp_point_visible = false;
				} else {
					cached_bullet_tp_position = target.pos;
				}

				if ( config::aimbot_autoshoot && input_handler.is_hold( config::aimbot_autoshoot_key ) ) {

					if ( target.player->is_visible( ) ) {
						point_visible = true;
						start_shooting = true;
					}
				} else {
					start_shooting = false;
				}

				if ( bullet_tp_angle.empty( ) )
					bullet_tp_angle = target.pos;

				if ( config::aimbot_autoshoot && instant_hit_ready && 
					 start_shooting && point_visible && 
					 !held_entity->is_class( _( "Hammer" ) ) && input_handler.is_hold( config::aimbot_autoshoot_key ) )
					held_entity->do_attack_recreation( );

				if ( held_entity->is_class( _( "BaseProjectile" ) ) ) {
					if ( start_shooting && point_visible ) {
						const auto& primary_magazine = held_entity->primary_magazine( );
						if ( memory::is_valid( primary_magazine ) ) {
							const auto& contents = primary_magazine->contents( );
							if ( contents > 0 ) {
								if ( bullet_tp_angle.empty( ) )
									bullet_tp_angle = target.pos;

								if ( bullet_queue_counter.calculate( ) <= 2 ) {

									for ( int i = 0; i < 5; i++ ) {
										primary_magazine->set_contents( contents - 1 );

										held_entity->launch_projectile( );
										held_entity->update_ammo_display( );
										held_entity->shot_fired( );
										held_entity->did_attack_clientside( );
										break;
									}

									bullet_queue_counter.increment( );
								}
							}
						}
					}
				}
			} else {
				manipulation_indicator = false;
				start_shooting = false;
				cached_bullet_tp_position = { };
				cached_manip_point = eye_pos;
				manipulation_angle = eye_pos;
				point_visible = false;
				bullet_tp_point_visible = false;
				bullet_tp_angle = { };
			}
		}

		inline void on_some_misc( ) {

			if ( config::exploits_admin_flag ) {
				const auto& all = sdk::c_console_system::all( );
				for ( auto i = 0; i < all->count( ); ++i ) {
					const auto& cmd = all->get( i );
					if ( !memory::is_valid( cmd ) )
						continue;

					const auto& name = cmd->name( );
					if ( !memory::is_valid( name ) )
						continue;

					if ( HASH_EQUAL( RHASH( name->string( ).c_str( ) ), _( L"noclip" ) )
						 && cmd->allow_run_from_server( ) )
						cmd->set_allow_run_from_server( false );
					else if ( HASH_EQUAL( RHASH( name->string( ).c_str( ) ), _( L"debugcamera" ) )
							  && cmd->allow_run_from_server( ) )
						cmd->set_allow_run_from_server( false );
					else if ( HASH_EQUAL( RHASH( name->string( ).c_str( ) ), _( L"camspeed" ) )
							  && cmd->allow_run_from_server( ) )
						cmd->set_allow_run_from_server( false );
					else if ( HASH_EQUAL( RHASH( name->string( ).c_str( ) ), _( L"camlerp" ) )
							  && cmd->allow_run_from_server( ) )
						cmd->set_allow_run_from_server( false );
				}

				sdk::local_player->add_flag( sdk::e_flag::is_admin );
			} else {
				sdk::local_player->remove_flag( sdk::e_flag::is_admin );
			}

			if ( config::misc_automatic_auto_trade ) {
				static const auto& type_object = il2cpp::type_object( _( "" ), _( "LootPanelVendingMachine" ) );
				static const auto& loot_panel_vending_machine = unity::c_game_object::find_object_of_type< sdk::c_loot_panel_vending_machine* >( type_object );

				static sdk::c_invisible_vending_machine* vending_machine = nullptr;

				if ( loot_panel_vending_machine->enabled( ) ) {
					if ( !vending_machine )
						vending_machine = loot_panel_vending_machine->vending_machine( );

					if ( memory::is_valid( vending_machine ) ) {
						const auto& sell_orders = vending_machine->sell_order_container( )->sell_orders( );
						if ( memory::is_valid( sell_orders ) ) {
							//for ( auto&& index : options::misc.auto_trade_values ) {
							//	const auto& sell_order = sell_orders->value( index );
							//	if ( !memory::is_valid( sell_order ) )
							//		continue;

							//	const auto& in_stock = sell_order->in_stock( );
							//	if ( in_stock <= 0 )
							//		continue;

							//	loot_panel_vending_machine->buy_button_clicked( index, in_stock );
							//}
						}
					}
				} else {
					if ( vending_machine )
						vending_machine = nullptr;
				}
			}
		}

		inline void on_misc( ) {
			if ( config::misc_debug_camera ) {
				if ( input_handler.is_down( config::misc_debug_camera_key ) ) {
					static const auto& game_assembly = pe::get_image( HASH( "gameassembly" ) );

					const auto& camera_man = mem::read_chain< unity::c_behaviour* >( game_assembly, { 0x3406B00, 0xB8, 0x0 } ); // SingletonComponent_CameraMan__c*
					if ( !memory::is_valid( camera_man ) ) {
						sdk::c_game_manager::instance( )->create_prefab( _( "assets/bundled/prefabs/system/debug/debug_camera.prefab" ) );
					} else {
						camera_man->set_enabled( !camera_man->enabled( ) );
					}

					sdk::local_player->on_view_mode_changed( );
				}

				sdk::convar::c_client::set_cam_lerp( 1.f );
				sdk::convar::c_client::set_cam_speed( 1.f );
			}

			const auto& movement = sdk::local_player->walk_movement( );
			const auto& model_state = sdk::local_player->model_state( );
			if ( memory::is_valid( movement ) && memory::is_valid( model_state ) ) {
				if ( config::misc_suicide && input_handler.is_down( config::misc_suicide_key ) ) {
					sdk::local_player->on_land( -108.0001f );
				} else if ( config::misc_disable_land_damage ) {
					movement->set_was_falling( true );
					movement->set_previous_velocity( vec3_t( ) );
				}

				if ( config::misc_modify_can_attack ) {
					model_state->add_flag( sdk::e_model_state_flags::on_ground );
					movement->set_grounded( true );
				}

				if ( config::misc_infinite_jump ) {
					movement->set_jump_time( 0.f );
					movement->set_ground_time( movement->ground_time( ) + 0.5f );
					movement->set_land_time( 0.f );
				}

				movement->set_gravity_multiplier( config::misc_high_jump ? config::misc_high_jump_height / 100.f : 2.35f );
			}

			if ( config::visuals_time_change_time )
				sdk::convar::c_admin::set_admintime( config::visuals_time_change_time_value );
			else
				sdk::convar::c_admin::set_admintime( -1.f );

			const auto& tod_sky = sdk::c_tod_sky::instance( );
			if ( memory::is_valid( tod_sky ) ) {

				if ( config::visuals_weather_no_sun ) {
					tod_sky->sun( )->set_mesh_brightness( 0.f );
					tod_sky->sun( )->set_mesh_contrast( 0.f );
					tod_sky->sun( )->set_mesh_size( 0.f );
				}

				if ( config::visuals_sky_bright_cave ) {
					tod_sky->day( )->set_ambient_multiplier( 1.f );
					tod_sky->day( )->set_reflection_multiplier( 1.f );
				}

				if ( config::visuals_sky_bright_night ) {
					tod_sky->night( )->set_ambient_multiplier( 10.f );
					tod_sky->ambient( )->set_saturation( 0.f );
					tod_sky->night( )->set_light_intensity( 0.f );
				}
			}

			if ( config::visuals_weather_no_clouds )
				sdk::convar::c_weather::set_cloud_opacity( 0.f );
			else
				sdk::convar::c_weather::set_cloud_opacity( -1.f );

			if ( config::visuals_weather_no_fog )
				sdk::convar::c_weather::set_fog( 0.f );
			else
				sdk::convar::c_weather::set_fog( -1.f );

			if ( config::visuals_weather_no_rain )
				sdk::convar::c_weather::set_rain( 0.f );
			else
				sdk::convar::c_weather::set_rain( -1.f );

			if ( config::visuals_weather_rainbows )
				sdk::convar::c_weather::set_rainbow( 1.f );
			else
				sdk::convar::c_weather::set_rainbow( -1.f );

			if ( config::visuals_weather_no_thunder )
				sdk::convar::c_weather::set_thunder( 0.f );
			else
				sdk::convar::c_weather::set_thunder( -1.f );

			if ( config::visuals_weather_atmosphere )
				sdk::convar::c_weather::set_atmosphere_contrast( 5.f );
			else
				sdk::convar::c_weather::set_atmosphere_contrast( -1.f );

			if ( config::visuals_weather_no_wind )
				sdk::convar::c_weather::set_wind( 0.f );
			else
				sdk::convar::c_weather::set_wind( -1.f );
		}

		inline void on_exploits( ) {

			static float clothing_move_speed_reduction = 0.f;
			static bool has_got = false;

			if ( config::exploits_speedhack && input_handler.is_hold( config::exploits_speedhack_key ) ) {
				if ( !has_got ) {
					clothing_move_speed_reduction = sdk::local_player->clothing_move_speed_reduction( );
					has_got = true;
				}

				if ( has_got )
					sdk::local_player->set_clothing_move_speed_reduction( -config::exploits_speedhack_value );
			} else {
				if ( has_got )
					sdk::local_player->set_clothing_move_speed_reduction( clothing_move_speed_reduction );
			}

			if ( config::misc_fov_zoom_fov && input_handler.is_hold( config::misc_fov_zoom_fov_key ) )
				sdk::convar::c_graphics::set_fov( 90.f - config::misc_fov_zoom_fov_amount );
			else if ( config::misc_fov_fov_changer )
				sdk::convar::c_graphics::set_fov( config::misc_fov_fov_changer_amount );
			else
				sdk::convar::c_graphics::set_fov( 90.f );

			const auto& mountable = sdk::local_player->mounted( );
			if ( memory::is_valid( mountable ) ) {
				if ( config::exploits_can_attack_in_vehicles )
					mountable->set_can_wield_items( true );
			}

			unity::c_physics::ignore_layer_collision( unity::e_layer_collision::player_movement, unity::e_layer_collision::ai, config::exploits_walk_through_players );
			unity::c_physics::ignore_layer_collision( unity::e_layer_collision::player_movement, unity::e_layer_collision::tree, config::exploits_walk_through_trees );
			unity::c_physics::ignore_layer_collision( unity::e_layer_collision::player_movement, unity::e_layer_collision::water, !config::exploits_walk_on_water );
		}

		inline void on_misc_weapon( ) {
			const auto& item = sdk::local_player->item( );
			if ( !memory::is_valid( item ) )
				return;

			const auto& held_entity = item->held_entity( );
			if ( !memory::is_valid( held_entity ) )
				return;

			if ( config::misc_weapon_chams && sdk::bundles::initialized ) {
				const auto& active_model = sdk::c_base_view_model::active_model( );
				if ( memory::is_valid( active_model ) ) {
					active_model->set_use_view_model_camera( false );

					const auto& renderers = active_model->components_in_children< unity::c_renderer* >( il2cpp::type_object( _( "UnityEngine" ), _( "Renderer" ) ) );
					if ( memory::is_valid( renderers ) ) {
						for ( auto i = 0; i < renderers->count( ); ++i ) {
							const auto& renderer = renderers->get( i );
							if ( !memory::is_valid( renderer ) )
								continue;

							const auto& material = renderer->material( );
							if ( !memory::is_valid( material ) )
								continue;

							if ( material->name( )->contains( _( "sparks2" ) )
								 || material->name( )->contains( _( "puff-3" ) )
								 || material->name( )->contains( _( "c4_smoke_01" ) )
								 || material->name( )->contains( _( "HeavyRefract" ) )
								 || material->name( )->contains( _( "pfx_smoke_whispy_1_white_viewmodel" ) )
								 || material->name( )->contains( _( "Ak47uIce Specular" ) )
								 || material->name( )->contains( _( "muzzle_embers" ) )
								 || material->name( )->contains( _( "c4charge" ) )
								 || material->name( )->contains( _( "pfx_smoke_rocket" ) )
								 || material->name( )->contains( _( "pfx_smoke_rocket_thicksoftblend" ) )
								 || material->name( )->contains( _( "holosight.georeticle (Instance)" ) )
								 || material->name( )->contains( _( "vfx_embers (Instance)" ) )
								 || material->name( )->contains( _( "pfx_smoke_whispy_1_white  (Instance)" ) )
								 || material->name( )->contains( _( "flame-mlrs-cone (Instance)" ) )
								 || material->name( )->contains( _( "vfx_heatshimmer (Instance)" ) )
								 || material->name( )->contains( _( "lasersight (Instance)" ) )

								 || material->name( )->contains( _( "muzzle_fumes1" ) )
								 || material->name( )->contains( _( "muzzle_fumes2" ) )
								 || material->name( )->contains( _( "muzzle_fumes3" ) )
								 || material->name( )->contains( _( "wispy-2" ) )
								 || material->name( )->contains( _( "quickblast-1" ) )
								 || material->name( )->contains( _( "muzzle_flash-front-3x3" ) )
								 || material->name( )->contains( _( "muzzle_flash-cross" ) )
								 || material->name( )->contains( _( "muzzle_flash-side-1x4" ) ) )
								continue;

							switch ( config::misc_weapon_chams_value ) {
							case 0:
								if ( material->shader( ) != sdk::bundles::glitter_shader ) {
									sdk::bundles::glitter_material->set_shader( sdk::bundles::glitter_shader );
									renderer->set_material( sdk::bundles::glitter_material );

									sdk::bundles::glitter_material->set_color( _( "_FresnelColor" ), unity::c_color( 0, 255, 255, 255 ) );
									sdk::bundles::glitter_material->set_color( _( "_GlitterColor" ), unity::c_color( 0, 255, 255, 255 ) );
									sdk::bundles::glitter_material->set_color( _( "_ExteriorDesignColor" ), unity::c_color( 0, 255, 255, 255 ) );
								}
								break;
							case 1:
								if ( material->shader( ) != sdk::bundles::damascus_shader ) {
									sdk::bundles::damascus_material->set_shader( sdk::bundles::damascus_shader );
									renderer->set_material( sdk::bundles::damascus_material );

									sdk::bundles::damascus_material->set_color( _( "_FresnelColor" ), unity::c_color( 33, 0, 191, 255 ) );
									sdk::bundles::damascus_material->set_color( _( "_InteriorColor" ), unity::c_color( 0, 255, 255, 255 ) );
									sdk::bundles::damascus_material->set_color( _( "_ExteriorTopColor" ), unity::c_color( 0, 255, 255, 255 ) );
									sdk::bundles::damascus_material->set_color( _( "_ExteriorBottomColor" ), unity::c_color( 255, 0, 0, 255 ) );

									sdk::bundles::damascus_material->set_float( _( "_ExteriorGradientPos" ), 0.f );
									sdk::bundles::damascus_material->set_float( _( "_ExterioirGredientSpread" ), 0.5f );
								}
								break;
							case 2:
								if ( material->shader( ) != sdk::bundles::lightning_shader ) {
									sdk::bundles::lightning_material->set_shader( sdk::bundles::lightning_shader );
									renderer->set_material( sdk::bundles::lightning_material );
								}
								break;
							case 3:
								if ( material->shader( ) != sdk::bundles::wire_frame_shader ) {
									sdk::bundles::wire_frame_material->set_shader( sdk::bundles::wire_frame_shader );
									renderer->set_material( sdk::bundles::wire_frame_material );
								}
								break;
							case 4:
								if ( material->shader( ) != sdk::bundles::fire_a_shader ) {
									sdk::bundles::fire_a_material->set_shader( sdk::bundles::fire_a_shader );
									renderer->set_material( sdk::bundles::fire_a_material );

									sdk::bundles::fire_a_material->set_color( _( "_FlameColor" ), unity::c_color( 255, 206, 0, 255 ) );
									sdk::bundles::fire_a_material->set_color( _( "_Flamecolor2" ), unity::c_color( 255, 0, 0, 255 ) );
								}
								break;
							case 5:
								if ( material->shader( ) != sdk::bundles::fire_b_shader ) {
									sdk::bundles::fire_b_material->set_shader( sdk::bundles::fire_b_shader );
									renderer->set_material( sdk::bundles::fire_b_material );

									sdk::bundles::fire_a_material->set_color( _( "_FlameColor" ), unity::c_color( 0, 255, 255, 255 ) );
									sdk::bundles::fire_a_material->set_color( _( "_Flamecolor2" ), unity::c_color( 0, 0, 255, 255 ) );
								}
								break;
							case 6:
								if ( material->shader( ) != sdk::bundles::geometric_shader ) {
									sdk::bundles::geometric_material->set_shader( sdk::bundles::geometric_shader );
									renderer->set_material( sdk::bundles::geometric_material );
								}
								break;
							}
						}
					}
				}
			}

			if ( item->is_melee( ) ) {
				if ( config::aimbot_silent_melee ) {
					static unity::c_list_dictionary* player_list = sdk::c_base_player::visible_player_list( );

					for ( auto i = 0; i < player_list->count( ); ++i ) {
						const auto& player = player_list->get< sdk::c_base_player* >( i );
						if ( !memory::is_valid( player ) )
							continue;

						if ( player->is_local_player( ) || player->lifestate( ) || player->is_sleeping( ) )
							continue;

						if ( config::aimbot_ignore_team && player->is_teammate( ) )
							continue;

						const auto& model = player->model( );
						if ( !memory::is_valid( model ) )
							continue;

						const auto& max_dist = config::misc_weapon_extended_melee ? 2.f + config::misc_weapon_extended_melee_radius : 2.f;
						if ( sdk::local_position.distance_to( model->position( ) ) <= max_dist )
							do_melee_attack( player, held_entity );
					}
				}

				if ( config::misc_weapon_disable_slow_down_melee )
					item->set_block_sprint( false );

				if ( config::misc_weapon_extended_melee )
					item->set_attack_max_distance( 2.f + config::misc_weapon_extended_melee_radius );

			} else if ( item->is_weapon( ) ) {
				if ( config::misc_weapon_automatic )
					item->set_automatic( true );

				if ( config::misc_weapon_fast_bow ) {
					item->set_attack_ready( true );
					item->set_was_aiming( true );
				}

				if ( config::misc_weapon_fast_eoka ) {
					float eoka_chance = config::misc_weapon_fast_eoka_chance / 100.f;
					item->set_always_eoka( eoka_chance );

					if ( eoka_chance > 0.9f && input_handler.is_hold( VK_LBUTTON ) )
						item->set_did_spark_this_frame( true );
				}

				if ( config::misc_weapon_rapid_fire )
					item->set_repeat_delay( config::misc_weapon_rapid_fire_speed / 100.f );

				if ( config::misc_weapon_instant_charge_compound ) {
					item->set_current_hold_progress( 1.5f );
					item->set_string_bonus_damage( 1000.f );
					item->set_movement_penalty_ramp_up_time( 1000.f );
				}

				item->set_recoil_min_max( config::aimbot_no_recoil ?
										  config::aimbot_no_recoil_value / 100.f : 1.f );

				if ( config::aimbot_no_sway ) {
					item->set_aim_sway( 0.f );
					item->set_aim_sway_speed( 0.f );
				}

				if ( config::aimbot_no_spread ) {
					float spread = config::aimbot_no_spread_value / 100.f;

					item->set_aim_cone( spread );
					item->set_aim_cone_penalty_per_shot( spread );
					item->set_aim_cone_penalty( spread );
					item->set_hip_aim_cone( spread );
					item->set_stance_penalty( spread );
					item->set_aim_cone_penalty_max( spread );
					item->set_aim_penalty_recover_time( spread );
					item->set_aim_penalty_recover_delay( spread );
					item->set_aim_sight_aim_cone_scale( spread );
					item->set_aim_sight_aim_cone_offset( spread );
					item->set_hip_aim_cone_scale( spread );
					item->set_hip_aim_cone_offset( spread );
				}

				if ( config::misc_weapon_spam && input_handler.is_hold( config::misc_weapon_spam_key ) ) {
					static float send_time = unity::c_time::realtime_since_startup( );

					const auto& delay = config::misc_weapon_spam_delay / 100;
					const auto& current_time = unity::c_time::realtime_since_startup( );
					if ( current_time - send_time > delay ) {
						item->send_signal_broadcast( sdk::e_signal::attack );
						send_time = current_time;
					}
				}
			}

			reload_time = held_entity->reload_time( );

			if ( config::misc_automatic_auto_reload && held_entity->is_class( _( "BaseProjectile" ) ) && !held_entity->has_reload_cooldown( ) ) {
				if ( !did_reload )
					time_since_last_shot = ( unity::c_time::fixed_time( ) - fixed_time_last_shot );

				if ( just_shot && ( time_since_last_shot > 0.2f ) ) {
					held_entity->server_rpc( _( "StartReload" ) );
					held_entity->send_signal_broadcast( sdk::e_signal::reload );

					just_shot = false;
				}

				if ( time_since_last_shot > ( held_entity->reload_time( ) - ( held_entity->reload_time( ) / 10 ) ) && !did_reload ) {
					held_entity->server_rpc( _( "Reload" ) );
					did_reload = true;
					time_since_last_shot = 0.f;
				}
			} else {
				did_reload = false;
				just_shot = false;
				fixed_time_last_shot = 0.f;
				time_since_last_shot = 0.f;
			}

			if ( config::misc_automatic_auto_heal && item->is_heal( ) ) {
				float next_action_time = 0, period = 1.4721;

				const auto& time = unity::c_time::time( );
				if ( held_entity->time_since_deploy( ) > held_entity->deploy_delay( ) && held_entity->next_attack_time( ) <= time ) {
					if ( time > next_action_time ) {
						next_action_time = time + period;
						if ( sdk::local_player->health( ) < 99 )
							held_entity->server_rpc( _( "UseSelf" ) );
					}
				}
			}
		}

		inline void loop( ) {
			if ( !sdk::local_player )
				return;

			static bool visuals_players_chams_enabled = false;
			if ( config::visuals_players_chams ) {
				visuals_players_chams_enabled = true;
			} else if ( visuals_players_chams_enabled ) {
				sdk::c_player_model::rebuild_all( );
				visuals_players_chams_enabled = false;
			}

			on_manipulation( );
			on_check_flyhack( );
			//on_check_speedhack( );

			on_misc( );
			on_exploits( );
			on_misc_weapon( );
		}
	};

	auto& misc = c_misc::instance( );
}
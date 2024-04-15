#pragma once
#include "../../vcruntime/core.hpp"

namespace features {
	float initial_distance_debug = 0.f;

	bool can_manipulate = false;
	bool start_shooting = false;
	bool instant_hit_ready = false;
	bool manipulation_indicator = false;
	float max_spoofed_eye_distance = 0.f;

	vec3_t temp_cached_point;
	vec3_t cached_manip_point;
	vec3_t manipulation_angle;
	bool position_visible = false;

	vec3_t bullet_tp_angle;
	vec3_t bullet_tp_seperator;
	vec3_t cached_bullet_tp_position;
	bool bullet_tp_point_visible = false;

	uint64_t target_id;
	bool point_visible = false;

	class c_aim_target {
	public:
		vec3_t pos;
		vec3_t velocity;

		sdk::e_bone bone;

		sdk::c_base_player* player = nullptr;

		float fov = config::aimbot_fov_value;
		float distance = 100.f;

		bool is_heli = false;
		bool visible = false;
		bool teammate = false;

		bool operator<( const c_aim_target& b ) {
			if ( fov == config::aimbot_fov_value ) {
				return distance < b.distance;
			} else {
				return fov < b.fov;
			}
		}
	};

	class c_line {
	public:
		vec3_t start;
		vec3_t end;

		c_line( ) { }
		c_line( vec3_t s, vec3_t e ) {
			start = s; end = e;
		}

		vec3_t closest_point( vec3_t pos ) {
			vec3_t a = end - start;
			float magnitude = a.length( );
			if ( magnitude == 0.f ) return start;
			vec3_t vector = a / magnitude;
			return start + vector * vec3_t::clamp( ( pos - start ).dot( vector ), 0.f, magnitude );
		}
	};

	class c_time_average_value_data {
	public:
		int counter_prev;
		int counter_next;

		float refresh_time;

		int calculate( ) {
			float realtime_since_startup = unity::c_time::realtime_since_startup( );

			float num = realtime_since_startup - refresh_time;
			if ( num >= 1.0 ) {
				counter_prev = ( int )( counter_next / num + 0.5 );
				counter_next = 0;
				refresh_time = realtime_since_startup;
				num = 0;
			}

			return ( int )( counter_prev * ( 1.0 - num ) ) + counter_next;
		}

		void increment( ) {
			calculate( );
			counter_next += 1;
		}

		void reset( ) {
			counter_prev = 0;
			counter_next = 0;
		}
	};

	struct trace_result_t {
		bool did_hit;
		bool silent_cat;
		sdk::c_base_player* hit_entity;
		vec3_t hit_position;
		vec3_t hit_velocity;
		vec3_t player_position;
		vec3_t out_velocity;
		float hit_time;
		float desired_time;
		float hit_dist;
		bool can_hit;

		trace_result_t( ) {
			did_hit = can_hit = silent_cat = false;
			hit_dist = FLT_MAX;
		}
	};

	struct prediction_data_t {
		bool possible;
		vec3_t position;
		vec3_t out_velocity;
		vec3_t hit_velocity;
		float hit_time;
		float hit_mismatch;
		int difficulty;

		float drag;
		vec3_t gravity;
		vec3_t angle;

		prediction_data_t( ) {
			possible = false;
		}
	};

	class c_aimbot : public singleton< c_aimbot > {
	public:
		inline sdk::e_bone bone( ) {
			switch ( config::aimbot_bone ) {
			case 0:
				return sdk::e_bone::head;
			case 1:
				return sdk::e_bone::neck;
			case 2:
				return sdk::e_bone::spine1;
			case 3:
				switch ( rand( ) % 9 ) {
				case 0:
					return sdk::e_bone::head;
				case 1:
					return sdk::e_bone::pelvis;
				case 2:
					return sdk::e_bone::r_hip;
				case 3:
					return sdk::e_bone::r_foot;
				case 4:
					return sdk::e_bone::spine1;
				case 5:
					return sdk::e_bone::l_hand;
				case 6:
					return sdk::e_bone::r_upperarm;
				case 7:
					return sdk::e_bone::l_knee;
				case 8:
					return sdk::e_bone::spine4;
				}
				break;
			}
		}

		inline c_aim_target get_target( vec3_t source = { }, float max_distance = 500.f ) {
			if ( source.empty( ) )
				source = sdk::local_player->eyes( )->position( );

			c_aim_target best_target = { };

			for ( auto&& player : sdk::info.players ) {

				if ( player->is_local_player( ) || player->lifestate( ) || player->is_sleeping( ) )
					continue;

				c_aim_target target;
				target.player = player;

				const auto& model = player->model( );
				if ( memory::is_valid( model ) ) {
					target.velocity = model->new_velocity( );

					if ( !config::visuals_players_npc && model->is_npc( ) )
						continue;
				}

				target.teammate = player->is_teammate( );
				if ( config::aimbot_ignore_team && target.teammate )
					continue;

				target.visible = player->is_visible( );
				if ( config::aimbot_visible_check && !target.visible )
					continue;

				target.bone = bone( );
				target.pos = target.player->bone_position( target.bone );
				target.fov = sdk::camera->calculate_fov( target.pos );

				target.distance = source.distance_to( target.pos );
				if ( target.distance > max_distance )
					continue;

				if ( target < best_target )
					best_target = target;
			}

			return best_target;
		}

		inline bool drag_gravity( float* drag, vec3_t* gravity, float* bullet_speed, float* penetration_power ) {
			const auto& item = sdk::local_player->item( );
			if ( !memory::is_valid( item ) )
				return false;

			const auto& held_entity = item->held_entity( );
			if ( !memory::is_valid( held_entity ) )
				return false;

			const auto& projectile_type = il2cpp::type_object( _( "" ), _( "Projectile" ) );
			const auto& item_mod_projectile_type = il2cpp::type_object( _( "" ), _( "ItemModProjectile" ) );

			sdk::c_item_mod_projectile* item_mod_projectile = nullptr;
			if ( item->is_weapon( ) ) {
				const auto& ammo_type = held_entity->primary_magazine( )->ammo_type( );
				if ( !memory::is_valid( ammo_type ) )
					return false;

				item_mod_projectile = ammo_type->component< sdk::c_item_mod_projectile* >( item_mod_projectile_type );
			} else if ( !item->is_weapon( ) ) {
				const auto& info = item->info( );
				if ( !memory::is_valid( info ) )
					return false;

				item_mod_projectile = info->component< sdk::c_item_mod_projectile* >( item_mod_projectile_type );
			}

			if ( !memory::is_valid( item_mod_projectile ) )
				return false;

			if ( bullet_speed ) {
				const auto& modifier = item->is_weapon( ) ? held_entity->projectile_velocity_scale( ) : 1.f;
				const auto& velocity = item_mod_projectile->projectile_velocity( );

				const auto& spread = item_mod_projectile->projectile_velocity_spread( );
				*bullet_speed = ( velocity + spread ) * modifier * ( config::aimbot_instant_bullet ? 1.495f : 1.f );
			}

			const auto& projectile_object_ref = item_mod_projectile->projectile_object( );
			if ( !memory::is_valid( projectile_object_ref ) )
				return false;

			const auto& guid = projectile_object_ref->guid( );
			if ( !memory::is_valid( guid ) )
				return false;

			const auto& projectile_object = sdk::c_game_manifest::guid_to_object( guid );
			if ( !memory::is_valid( projectile_object ) )
				return false;

			const auto& projectile = projectile_object->component< sdk::c_projectile* >( projectile_type );
			if ( !memory::is_valid( projectile ) )
				return false;

			if ( drag )
				*drag = projectile->drag( );

			if ( gravity )
				*gravity = vec3_t( 0, -9.81f * projectile->gravity_modifier( ), 0 );

			if ( penetration_power )
				*penetration_power = projectile->penetration_power( );

			return true;
		}

		trace_result_t trace_projectile( vec3_t position, vec3_t velocity, float drag, vec3_t gravity, vec3_t target_point, bool draw = false, float max_travel_time = 0.f ) {
			constexpr float num = 0.03125f;

			vec3_t prev_position = position;
			float prev_dist = FLT_MAX;

			c_line result_line = c_line( position, position );
			float travel_time = 0.f;

			trace_result_t result;
			result.out_velocity = velocity;

			vec3_t hit_velocity = velocity;

			if ( max_travel_time <= 0.f )
				max_travel_time = 8.f;

			for ( ; travel_time < max_travel_time; ) {
				prev_position = position;
				position += velocity * num;

				auto line = c_line( prev_position, position );
				vec3_t nearest = line.closest_point( target_point );

				float dst = nearest.distance_to( target_point );
				if ( dst > prev_dist )
					break;

				prev_dist = dst;
				result_line = line;

				velocity += gravity * num;
				velocity -= velocity * drag * num;
				travel_time += num;
				hit_velocity = velocity;
			}

			vec3_t hit_pos = result_line.closest_point( target_point );

			result.hit_dist = hit_pos.distance_to( target_point );
			result.hit_position = hit_pos;
			result.hit_velocity = hit_velocity;
			result.hit_time = travel_time - num;
			result.did_hit = true;
			return result;
		}

		prediction_data_t prediction( vec3_t start_position, vec3_t target, vec3_t velocity, float prediction_accuracy = 0.2f, float bullet_speed_override = 0.f, float max_travel_time = 0.f, float drag_override = -1.f, float gravity_override = -1.f ) {
			prediction_data_t result;
			result.out_velocity = ( target - start_position );
			result.position = target;
			result.possible = false;

			float drag;
			float bullet_speed;

			vec3_t gravity;
			if ( !drag_gravity( &drag, &gravity, &bullet_speed, NULL ) && ( drag_override < 0.f || gravity_override < 0.f || bullet_speed_override <= 0.f ) )
				return result;

			if ( drag_override >= 0.f )
				drag = drag_override;

			if ( gravity_override >= 0.f )
				gravity = vec3_t( 0, -9.81f * gravity_override, 0 );

			if ( bullet_speed_override > 0.f )
				bullet_speed = bullet_speed_override;

			if ( max_travel_time <= 0.f )
				max_travel_time = 8.f;

			result.gravity = gravity;
			result.drag = drag;
			result.out_velocity = ( target - start_position ).normalized( ) * bullet_speed;

			trace_result_t best;
			trace_result_t previous;
			vec3_t best_angle;
			int iter = 0;
			bool movement_predicted = false;

		PIDOR:
			if ( !movement_predicted ) {
				constexpr float num = 0.03125f;

				const auto& travel_time = ( start_position - target ).length( ) / bullet_speed;
				float x = std::max( 0.f, travel_time + num );
				target += velocity.xz( ) * x;

				movement_predicted = true;
			}

			const auto& dir = ( target - start_position ).normalized( );
			const auto& dir_rotation = unity::c_quaternion::look_rotation( dir );
			const auto& basic_euler = unity::c_quaternion::euler_angles( dir_rotation );

			int scan_limit = 75;

			double left = -90.0;
			double right = 90.0;
			while ( iter < scan_limit ) {
				auto get_distance = [ & ]( double angle, vec3_t basic_euler, float bullet_speed, vec3_t start_position, float drag, vec3_t gravity, vec3_t target, float max_travel_time ) {
					const auto& euler = vec3_t( angle, basic_euler.y, 0 );
					const auto& forward = unity::c_quaternion::euler( euler ) * vec3_t( 0, 0, 1 );
					const auto& start_velocity = forward * bullet_speed;

					trace_result_t res = trace_projectile( start_position, start_velocity, drag, gravity, target, false, max_travel_time );
					return res;
				};

				const auto& m1 = left + ( right - left ) / 3;
				const auto& m2 = right - ( right - left ) / 3;

				trace_result_t r1 = get_distance( m1, basic_euler, bullet_speed, start_position, drag, gravity, target, max_travel_time );
				trace_result_t r2 = get_distance( m2, basic_euler, bullet_speed, start_position, drag, gravity, target, max_travel_time );

				if ( r1.hit_dist < r2.hit_dist ) {
					right = m2;
				} else {
					left = m1;
				}

				if ( !best.did_hit || r1.hit_dist < best.hit_dist ) {
					best = r1;
					best_angle = vec3_t( m1, basic_euler.y, 0 );
				}

				if ( r2.hit_dist < best.hit_dist ) {
					best = r2;
					best_angle = vec3_t( m2, basic_euler.y, 0 );
				}

				if ( best.did_hit && best.hit_dist <= prediction_accuracy )
					break;

				iter += 2;
			}

			if ( !movement_predicted ) {
				if ( velocity.length( ) ) {
					target += velocity * best.hit_time * 0.75f;

					previous.did_hit = best.did_hit = false;
					movement_predicted = true;
					goto PIDOR;
				}
			}

			if ( best.did_hit ) {
				result.hit_mismatch = best.hit_dist;
				result.hit_time = best.hit_time;
				result.out_velocity = best.out_velocity;
				result.position = best.hit_position;
				result.hit_velocity = best.hit_velocity;
				result.angle = best_angle;
				result.difficulty = iter;
			}

			result.possible = ( result.hit_mismatch <= prediction_accuracy );
			return result;
		}

		inline vec3_t aim_direction_to_target( vec3_t target_position, vec3_t target_velocity, sdk::c_item_mod_projectile* mod, vec3_t eyes_position ) {
			const auto& item = sdk::local_player->item( );
			if ( !memory::is_valid( item ) )
				return { };

			const auto& held_entity = item->held_entity( );
			if ( !memory::is_valid( held_entity ) )
				return { };

			if ( held_entity->is_class( _( "Planner" ) ) || held_entity->is_class( _( "MedicalTool" ) ) )
				return { };

			const auto& projectile_velocity = mod->projectile_velocity( );
			const auto& projectile_velocity_scale = held_entity->projectile_velocity_scale( false ) * 1.f;

			const auto& projectile_prefab = sdk::c_game_manifest::guid_to_object( mod->projectile_object( )->guid( ) )->component< sdk::c_projectile* >( il2cpp::type_object( _( "" ), _( "Projectile" ) ) );
			if ( !memory::is_valid( projectile_prefab ) )
				return { };

			const auto& projectile_drag = projectile_prefab->drag( );
			const auto& projectile_gravity_modifier = projectile_prefab->gravity_modifier( );

			const auto& gravity = vec3_t( 0.f, -9.81f, 0.f );

			const auto& final_velocity = projectile_velocity * projectile_velocity_scale;
			const auto& modifier_gravity = gravity * projectile_gravity_modifier;

			vec3_t temporary_target_position = target_position;

			for ( size_t j = 0; j < 50; j++ ) {
				vec3_t direction = ( temporary_target_position - eyes_position ).normalized( );

				vec3_t velocity = direction * final_velocity;

				vec3_t current_position = eyes_position;
				vec3_t previous_position = current_position;

				vec3_t missed_offset = { 0, 0, 0 };
				vec3_t closest_point = { FLT_MAX, FLT_MAX, FLT_MAX };

				vec3_t simulated_target_position = target_position;
				float simulated_target_time = 0.f;

				vec3_t initial_position = current_position;

				float inital_distance = projectile_prefab->initial_distance( );

				if ( config::aimbot_instant_bullet ) {
					auto desync_time_clamped = std::min( unity::c_time::realtime_since_startup( ) - sdk::local_player->last_sent_tick_time( ), 1.f );

					auto desync_time_clamped_server = std::max( desync_time_clamped - 0.03125f * 5.f, 0.2f );
					float num11 = desync_time_clamped_server;
					float distance_to_target = current_position.distance_to( temporary_target_position );
					float magnitude2 = velocity.magnitude( );
					inital_distance = projectile_prefab->initial_distance( ) + num11 * magnitude2;
					initial_distance_debug = inital_distance;
				}

				size_t total_steps = static_cast< size_t >( 8.0f / 0.03125f );

				for ( size_t step = 0; step < total_steps; step++ ) {
					previous_position = current_position;
					current_position += ( velocity * 0.03125f );

					velocity += ( modifier_gravity * 0.03125f );
					velocity -= ( velocity * projectile_drag * 0.03125f );

					auto line = current_position - previous_position;
					if ( initial_position.distance_to( current_position ) > inital_distance || initial_distance_debug == 0.f ) {
						simulated_target_time += 0.03125f;
						simulated_target_position = ( target_position + ( target_velocity * ( simulated_target_time ) ) );
					} else {
						simulated_target_position = target_position;
					}

					auto line_length = line.magnitude( );
					line.unity_normalize( );

					auto v = simulated_target_position - previous_position;
					auto d = v.dot( line );
					if ( d < 0.0f ) {
						d = 0.0f;
					} else if ( d > line_length ) {
						d = line_length;
					}

					vec3_t current_point = previous_position + line * d;

					float distance_missed = current_point.distance_to( simulated_target_position );
					if ( distance_missed < 0.1f ) {
						closest_point = current_point;
						return temporary_target_position;
					} else if ( distance_missed < closest_point.distance_to( simulated_target_position ) ) {
						missed_offset = simulated_target_position - current_point;
						closest_point = current_point;
					}
				}

				temporary_target_position += missed_offset;
			}

			return { };
		}

		inline float no_clip_radius( float margin ) {
			return 0.5f - margin;
		}

		inline bool test_no_clipping_new( vec3_t old_pos, vec3_t new_pos, float radius, float backtracking, bool sphere_cast, unity::c_collider*& collider, bool vehicle_layer, sdk::c_base_entity* ignore_entity ) {
			int num = 1537286401;
			if ( !vehicle_layer )
				num &= -8193;

			vec3_t normalized = ( new_pos - old_pos ).unity_normalize( );
			vec3_t vector = old_pos - normalized * backtracking;
			float magnitude = ( new_pos - vector ).magnitude( );

			unity::ray_t ray( vector, normalized );
			unity::raycast_hit_t hit_info;

			if ( ignore_entity == nullptr ) {
				if ( unity::c_physics::raycast( ray, hit_info, magnitude + radius, num, unity::e_query_trigger_interaction::ignore ) ) {
					collider = ( unity::c_collider* )hit_info.m_collider;
					return true;
				}
			} else {
				if ( sdk::c_game_physics::trace( ray, 0.f, hit_info, magnitude + radius, num, unity::e_query_trigger_interaction::ignore, ignore_entity ) ) {
					collider = ( unity::c_collider* )hit_info.m_collider;
					return true;
				}
			}

			if ( sphere_cast ) {
				if ( sdk::c_game_physics::trace( ray, radius, hit_info, magnitude, num, unity::e_query_trigger_interaction::ignore, ignore_entity ) ) {
					collider = ( unity::c_collider* )hit_info.m_collider;
					return true;
				}
			}

			return false;
		}

		inline bool validate_eye_pos( sdk::c_base_player* player, vec3_t eye_pos, bool check_line_of_sight = false ) {
			float num = 1.f + 0.5f;
			float eye_clientframes = 2.f;
			float eye_serverframes = 2.f;
			float num2 = eye_clientframes / 60.f;
			float num3 = eye_serverframes * std::max( std::max( unity::c_time::delta_time( ), unity::c_time::smooth_delta_time( ) ), unity::c_time::fixed_delta_time( ) );
			float desync_time = ( unity::c_time::realtime_since_startup( ) - sdk::local_player->last_sent_tick_time( ) );
			float num4 = ( desync_time + num2 + num3 ) * num;

			float num8 = abs( player->mount_velocity( ).y + player->parent_velocity( ).y );
			float num9 = player->bounds_padding( ) + num4 * num8 + player->jump_height( );
			float num10 = abs( player->eyes( )->position( ).y - eye_pos.y );

			float num5 = player->max_speed( ) + player->parent_velocity( ).magnitude( );
			float num6 = player->bounds_padding( ) + num4 * num5;
			float num7 = player->eyes( )->position( ).distance_to( eye_pos );

			if ( !check_line_of_sight && num7 > num6 )
				return false;

			if ( !check_line_of_sight && num10 > num9 )
				return false;

			int num11 = 2162688;
			num11 |= 8388608;

			vec3_t position2 = player->eyes( )->position( );
			float num12 = position2.distance_to( eye_pos );

			float eye_noclip_cutoff = 0.06f;
			int noclip_protection = 3;

			unity::c_collider* collider{ };
			if ( num12 > eye_noclip_cutoff && test_no_clipping_new( position2, eye_pos, no_clip_radius( 0.21f ), 0.01f, noclip_protection >= 2, collider, false, nullptr ) ) {
				return false;
			} else if ( num12 > 0.01f && test_no_clipping_new( position2, eye_pos, 0.01f, 0.01f, noclip_protection >= 2, collider, false, nullptr ) ) {
				return false;
			}

			return true;
		}

		inline void dynamic_manip_angles( std::vector<vec3_t>& re, float max = 1.5f, float maxy = 1.5f, int num_points = 100 ) {
			float radius = max;

			for ( int i = 0; i < num_points; ++i ) {
				float theta = static_cast< float >( rand( ) ) / static_cast< float >( RAND_MAX ) * 2 * M_PI;
				float phi = static_cast< float >( rand( ) ) / static_cast< float >( RAND_MAX ) * M_PI;

				float x = radius * sinf( phi ) * cosf( theta );
				float y = maxy * sinf( phi ) * sinf( theta );
				float z = radius * cosf( phi );

				re.emplace_back( x, y, z );
				re.emplace_back( -x, -y, -z );
				re.emplace_back( x, 0.f, z );

				re.emplace_back( 0.f, y, 0.f );
				re.emplace_back( 0.f, -y, 0.f );

				re.emplace_back( -x, 0.f, 0.f );
				re.emplace_back( x, 0.f, 0.f );

				re.emplace_back( 0.f, 0.f, z );
				re.emplace_back( 0.f, 0.f, -z );
			}
		}

		inline std::vector<vec3_t> generate_points_in_non_uniform_sphere( float radius_x, float radius_y, float radius_z, int base_num_points ) {
			std::vector<vec3_t> points;

			float volume = ( 4.0f / 3.0f ) * M_PI * radius_x * radius_y * radius_z;

			int num_points = static_cast< int >( base_num_points * ( volume / ( 4.0f / 3.0f * M_PI * radius_x * radius_y * radius_z ) ) );

			for ( int i = 0; i < num_points; i++ ) {
				float theta = static_cast< float >( rand( ) ) / static_cast< float >( RAND_MAX ) * 2 * M_PI;
				float phi = static_cast< float >( rand( ) ) / static_cast< float >( RAND_MAX ) * M_PI;

				float x = radius_x * sinf( phi ) * cosf( theta );
				float y = radius_y * sinf( phi ) * sinf( theta );
				float z = radius_z * cosf( phi );

				points.push_back( vec3_t( x, y, z ) );
				points.push_back( vec3_t( -x, -y, -z ) );
				points.push_back( vec3_t( x, 0.f, z ) );

				points.push_back( vec3_t( 0.f, y, 0.f ) );
				points.push_back( vec3_t( 0.f, -y, 0.f ) );

				points.push_back( vec3_t( -x, 0.f, 0.f ) );
				points.push_back( vec3_t( x, 0.f, 0.f ) );

				points.push_back( vec3_t( 0.f, 0.f, z ) );
				points.push_back( vec3_t( 0.f, 0.f, -z ) );
			}

			return points;
		}

		inline bool can_manipulate( vec3_t pos, float mm_eye = 7.f ) {
			const auto& target = get_target( );
			if ( !memory::is_valid( target.player ) )
				return false;

			const auto& eyes = sdk::local_player->eyes( );
			if ( !memory::is_valid( eyes ) )
				return false;

			const auto& eye_position = eyes->position( );

			if ( config::aimbot_bullet_tp ) {
				if ( sdk::c_game_physics::is_visible( eye_position, pos ) ) {
					cached_manip_point = eye_position;
					manipulation_angle = eye_position;
					position_visible = true;
					return true;
				} else
					position_visible = false;

				auto do_check = [ & ]( vec3_t a ) {
					vec3_t p;
					vec3_t manip_angle;
					if ( point_visible ) {
						p = a;
						manip_angle = a;
					} else {
						p = eye_position + a;
						manip_angle = a;
					}

					if ( !sdk::c_game_physics::is_visible( p, eye_position ) )
						return false;

					if ( !validate_eye_pos( sdk::local_player, p ) )
						return false;

					manipulation_angle = a;
					return true;
				};

				if ( config::aimbot_manipulation_mode == 0 ) {
					if ( !bullet_tp_point_visible && !point_visible && !position_visible || cached_manip_point.empty( ) ) {

						for ( float y = 1.5f; y > -1.5f; y -= 0.3f ) {
							int points = 5.f;
							float step = ( 1.57079632679489661923 ) / points;
							float x, z, current = 0;
							for ( size_t i = 0; i < points; i++ ) {
								x = sinf( current ) * mm_eye;
								z = cosf( current ) * mm_eye;

								vec3_t p1 = vec3_t( x, y, z );
								vec3_t p2 = vec3_t( -x, y, z );
								vec3_t p3 = vec3_t( x, y, -z );
								vec3_t p4 = vec3_t( -x, y, -z );

								vec3_t p = eye_position + p1;

								if ( do_check( p1 ) ) {
									cached_manip_point = p1 + eye_position;
									point_visible = true;
									return true;
								} else if ( do_check( p2 ) ) {
									cached_manip_point = p2 + eye_position;
									point_visible = true;
									return true;
								} else if ( do_check( p3 ) ) {
									cached_manip_point = p3 + eye_position;
									point_visible = true;
									return true;
								} else if ( do_check( p4 ) ) {
									cached_manip_point = p4 + eye_position;
									point_visible = true;
									return true;
								} else {
									manipulation_angle = eye_position;
								}

								current += step;
							}
						}
					} else if ( point_visible && !cached_manip_point.empty( ) || cached_manip_point == eye_position ) {
						if ( do_check( cached_manip_point ) ) {
							return true;
						} else {
							manipulation_angle = eye_position;
						}
					}
				} else if ( config::aimbot_manipulation_mode == 1 ) {
					if ( !bullet_tp_point_visible && !point_visible && !position_visible || cached_manip_point.empty( ) ) {
						std::vector<vec3_t> generated_points;
						dynamic_manip_angles( generated_points, mm_eye, 1.5, 5.f );

						for ( const vec3_t& point : generated_points ) {
							if ( do_check( point ) ) {
								cached_manip_point = point + eye_position;
								point_visible = true;
								return true;
							} else {
								manipulation_angle = eye_position;
							}
						}
					} else if ( point_visible && !cached_manip_point.empty( ) && !position_visible ) {
						if ( do_check( cached_manip_point ) ) {
							return true;
						} else {
							manipulation_angle = eye_position;
						}
					} else if ( position_visible ) {
						return true;
					}
				} else if ( config::aimbot_manipulation_mode == 2 ) {
					if ( !bullet_tp_point_visible && !point_visible && !position_visible || cached_manip_point.empty( ) ) {
						std::vector<vec3_t> generatedPoints = generate_points_in_non_uniform_sphere( mm_eye, 1.5, mm_eye, 5.f );

						for ( const vec3_t& point : generatedPoints ) {
							if ( do_check( point ) ) {
								cached_manip_point = point + eye_position;
								point_visible = true;
								return true;
							} else {
								manipulation_angle = eye_position;
							}
						}
					} else if ( point_visible && !cached_manip_point.empty( ) && !position_visible ) {
						if ( do_check( cached_manip_point ) ) {
							return true;
						} else {
							manipulation_angle = eye_position;
						}
					} else if ( position_visible ) {
						return true;
					}
				}

				return false;
			} else {
				if ( sdk::c_game_physics::is_visible( eye_position, pos ) ) {
					manipulation_angle = eye_position;
					cached_manip_point = eye_position;
					return true;
				}

				auto do_check = [ & ]( vec3_t a ) {
					vec3_t p;
					vec3_t manip_angle;
					if ( point_visible ) {
						p = a;
						manip_angle = a - eye_position;
					} else {
						p = eye_position + a;
						manip_angle = a;
					}

					if ( !sdk::c_game_physics::is_visible( p, eye_position ) )
						return false;

					if ( !validate_eye_pos( sdk::local_player, p ) )
						return false;

					float desync_time = ( unity::c_time::realtime_since_startup( ) - sdk::local_player->last_sent_tick_time( ) ) - 0.03125 * 3;

					float mm_max_eye = ( 0.1f + ( ( desync_time + 2.f / 60.f + 0.125f ) * 1.5f ) * sdk::local_player->max_velocity( ) ) - 0.05f;
					float max_v_multicplier = 4.5f;
					float v_mm_max_eye = 1.5f;

					if ( !sdk::c_game_physics::is_visible( p, pos ) ) {
						bool t = false;
						auto body_right = sdk::local_player->eyes( )->body_right( );
						auto body_forward = sdk::local_player->eyes( )->body_forward( );
						auto head_forward = sdk::local_player->eyes( )->head_forward( );

						std::array < vec3_t, 41 > positions = {
							pos + vec3_t( 0.f, ( v_mm_max_eye / 5 ), 0.f ),
							pos + vec3_t( 0.f, v_mm_max_eye / 2, 0.f ),
							pos + vec3_t( 0.f, v_mm_max_eye, 0.f ),
							pos + vec3_t( 0.f, -( 1.4f / 2 ), 0.f ),
							pos + vec3_t( 0.f, -1.4f, 0.f ),
							pos + vec3_t( body_right.x * ( mm_max_eye / 2 ), 0.f, body_right.z * ( mm_max_eye / 2 ) ),
							pos + vec3_t( body_right.x * mm_max_eye, 0.f, body_right.z * mm_max_eye ),
							pos + vec3_t( body_right.x * ( mm_max_eye / 2 ), ( 1.4f / 2 ), body_right.z * ( mm_max_eye / 2 ) ),
							pos + vec3_t( body_right.x * mm_max_eye, 1.4f, body_right.z * mm_max_eye ),
							pos + vec3_t( body_right.x * ( mm_max_eye / 2 ), -( 1.4f / 2 ), body_right.z * ( 4.f / 2 ) ),
							pos + vec3_t( body_right.x * mm_max_eye, -1.4f, body_right.z * mm_max_eye ),
							pos + vec3_t( -( body_right.x * ( mm_max_eye / 2 ) ), 0.f, -( body_right.z * ( mm_max_eye / 2 ) ) ),
							pos + vec3_t( -( body_right.x * mm_max_eye ), 0.f, -( body_right.z * mm_max_eye ) ),
							pos + vec3_t( -( body_right.x * ( mm_max_eye / 2 ) ), ( 1.4f / 2 ), -( body_right.z * ( mm_max_eye / 2 ) ) ),
							pos + vec3_t( -( body_right.x * mm_max_eye ), 1.4f, -( body_right.z * mm_max_eye ) ),
							pos + vec3_t( -( body_right.x * ( mm_max_eye / 2 ) ), -( 1.4f / 2 ), -( body_right.z * ( mm_max_eye / 2 ) ) ),
							pos + vec3_t( -( body_right.x * mm_max_eye ), -1.4f, -( body_right.z * mm_max_eye ) ),
							pos + vec3_t( body_forward.x * ( mm_max_eye / 2 ), 0.f, body_forward.z * ( mm_max_eye / 2 ) ),
							pos + vec3_t( body_forward.x * mm_max_eye, 0.f, body_forward.z * mm_max_eye ),
							pos + vec3_t( -( body_forward.x * ( mm_max_eye / 2 ) ), 0.f, -( body_forward.z * ( mm_max_eye / 2 ) ) ),
							pos + vec3_t( -( body_forward.x * mm_max_eye ), 0.f, -( body_forward.z * mm_max_eye ) ),
							pos + vec3_t( body_forward.x * ( mm_max_eye / 2 ), ( 1.4f / 2 ), body_forward.z * ( mm_max_eye / 2 ) ),
							pos + vec3_t( body_forward.x * mm_max_eye, 1.4f, body_forward.z * mm_max_eye ),
							pos + vec3_t( body_forward.x * ( mm_max_eye / 2 ), -( 1.4f / 2 ), body_forward.z * ( mm_max_eye / 2 ) ),
							pos + vec3_t( body_forward.x * mm_max_eye, -1.4f, body_forward.z * mm_max_eye ),
							pos + vec3_t( -( body_forward.x * ( mm_max_eye / 2 ) ), 0.f, -( body_forward.z * ( mm_max_eye / 2 ) ) ),
							pos + vec3_t( -( body_forward.x * mm_max_eye ), 0.f, -( body_forward.z * mm_max_eye ) ),
							pos + vec3_t( -( body_forward.x * ( mm_max_eye / 2 ) ), ( 1.4f / 2 ), -( body_forward.z * ( mm_max_eye / 2 ) ) ),
							pos + vec3_t( -( body_forward.x * 4.f ), 1.4f, -( body_forward.z * 4.f ) ),
							pos + vec3_t( -( body_forward.x * ( mm_max_eye / 2 ) ), -( 1.4f / 2 ), -( body_forward.z * ( mm_max_eye / 2 ) ) ),
							pos + vec3_t( -( body_forward.x * mm_max_eye ), -1.4f, -( body_forward.z * mm_max_eye ) ),
							pos + vec3_t( body_right.x * mm_max_eye, v_mm_max_eye, body_right.z * ( mm_max_eye ) ) * 0.9f,
							pos + vec3_t( -body_right.x * ( mm_max_eye ), ( v_mm_max_eye ), -body_right.z * ( mm_max_eye ) ) * 0.9f,
							pos + vec3_t( body_right.x * mm_max_eye, -v_mm_max_eye, body_right.z * ( mm_max_eye ) ) * 0.9f,
							pos + vec3_t( -body_right.x * ( mm_max_eye ), ( -v_mm_max_eye ), -body_right.z * ( mm_max_eye ) ) * 0.9f,
							pos + vec3_t( ( body_right.x / 2 ) * mm_max_eye, v_mm_max_eye, ( body_right.z / 2 ) * ( mm_max_eye ) ),
							pos + vec3_t( -( body_right.x / 2 ) * ( mm_max_eye ), ( v_mm_max_eye ), -( body_right.z / 2 ) * ( mm_max_eye ) ),
							pos + vec3_t( ( body_right.x / 2 ) * mm_max_eye, -v_mm_max_eye, ( body_right.z / 2 ) * ( mm_max_eye ) ),
							pos + vec3_t( -( body_right.x / 2 ) * ( mm_max_eye ), ( -v_mm_max_eye ), -( body_right.z / 2 ) * ( mm_max_eye ) ),
							pos + vec3_t( ( body_forward.x / 2 ) * ( mm_max_eye / 2 ), ( v_mm_max_eye / 1 ), ( body_forward.z / 2 ) * ( mm_max_eye / 2 ) ),
							pos + vec3_t( -( ( body_forward.x / 2 ) * ( mm_max_eye / 2 ) ), ( v_mm_max_eye / 1 ), -( ( body_forward.z / 2 ) * mm_max_eye / 2 ) ),
						};

						for ( auto& v : positions ) {

							if ( v.y > 1.5f )
								continue;

							if ( v.y > -1.5f )
								continue;

							if ( v.x > 9.f )
								continue;

							if ( v.x > -9.f )
								continue;

							if ( v.z > 9.f )
								continue;

							if ( v.z > -9.f )
								continue;

							if ( !validate_eye_pos( sdk::local_player, v ) )
								break;

							if ( !validate_eye_pos( sdk::local_player, pos ) )
								break;

							if ( !sdk::c_game_physics::is_visible( p, v ) )
								continue;

							if ( !sdk::c_game_physics::is_visible( pos, v ) )
								continue;

							return true;
						}

						return false;
					}

					manipulation_angle = manip_angle;
					return true;
				};

				if ( config::aimbot_manipulation_mode == 0 ) {
					if ( !point_visible || cached_manip_point.empty( ) ) {
						for ( float y = 1.5f; y > -1.5f; y -= 0.3f ) {
							int points = config::aimbot_manipulation_points;
							float step = ( 1.57079632679489661923 ) / points;
							float x, z, current = 0;
							for ( size_t i = 0; i < points; i++ ) {
								x = sinf( current ) * mm_eye;
								z = cosf( current ) * mm_eye;

								const auto& p1 = vec3_t( x, y, z );
								const auto& p2 = vec3_t( -x, y, z );
								const auto& p3 = vec3_t( x, y, -z );
								const auto& p4 = vec3_t( -x, y, -z );

								const auto& eye_position = sdk::local_player->eyes( )->position( );

								const auto& p = eye_position + p1;

								if ( do_check( p1 ) ) {
									cached_manip_point = p1 + eye_position;
									point_visible = true;
									return true;
								} else if ( do_check( p2 ) ) {
									cached_manip_point = p2 + eye_position;
									point_visible = true;
									return true;
								} else if ( do_check( p3 ) ) {
									cached_manip_point = p3 + eye_position;
									point_visible = true;
									return true;
								} else if ( do_check( p4 ) ) {
									cached_manip_point = p4 + eye_position;
									point_visible = true;
									return true;
								} else {
									manipulation_angle = eye_position;
								}

								current += step;
							}
						}
					} else if ( point_visible && !cached_manip_point.empty( ) || cached_manip_point == eye_position ) {
						if ( do_check( cached_manip_point ) ) {
							return true;
						} else {
							manipulation_angle = eye_position;
						}
					}
				} else if ( config::aimbot_manipulation_mode == 1 ) {
					if ( !point_visible || cached_manip_point.empty( ) ) {
						std::vector<vec3_t> generated_points;
						dynamic_manip_angles( generated_points, mm_eye, 1.5, 5.f );

						for ( const vec3_t& point : generated_points ) {
							if ( do_check( point ) ) {
								cached_manip_point = point + eye_position;
								point_visible = true;
								return true;
							} else {
								manipulation_angle = eye_position;
							}
						}
					} else if ( point_visible && !cached_manip_point.empty( ) || cached_manip_point == eye_position ) {
						if ( do_check( cached_manip_point ) ) {
							return true;
						} else {
							manipulation_angle = eye_position;
						}
					}
				} else if ( config::aimbot_manipulation_mode == 2 ) {
					if ( !point_visible || cached_manip_point.empty( ) ) {
						std::vector<vec3_t> generated_points = generate_points_in_non_uniform_sphere( mm_eye, 1.5, mm_eye, 5.f );

						for ( const vec3_t& point : generated_points ) {
							if ( do_check( point ) ) {
								cached_manip_point = point + eye_position;
								point_visible = true;
								return true;
							} else {
								manipulation_angle = eye_position;
							}
						}
					} else if ( point_visible && !cached_manip_point.empty( ) || cached_manip_point == eye_position ) {
						if ( do_check( cached_manip_point ) ) {
							return true;
						} else {
							manipulation_angle = eye_position;
						}
					}
				}
			}

			return false;
		}

		inline void bullet_tp_angles_mode_intense( std::vector<vec3_t>& re, int num_points = 100 ) {
			float radius = 2.5f;

			for ( int i = 0; i < num_points; ++i ) {
				float theta = 2.0 * M_PI * i / num_points;
				float phi = acosf( 1.0 - 2.0 * ( i + 0.5 ) / num_points );

				float x = radius * sinf( phi ) * cosf( theta );
				float y = radius * sinf( phi ) * sinf( theta );
				float z = radius * cosf( phi );

				re.emplace_back( x, y, z );
				re.emplace_back( -x, -y, -z );
				re.emplace_back( x, 0.f, z );

				re.emplace_back( 0.f, y, 0.f );
				re.emplace_back( 0.f, -y, 0.f );

				re.emplace_back( -x, 0.f, 0.f );
				re.emplace_back( x, 0.f, 0.f );

				re.emplace_back( 0.f, 0.f, z );
				re.emplace_back( 0.f, 0.f, -z );
			}
		}

		inline void find_manipulation_angles( float max_desync_value ) {
			if ( can_manipulate( cached_bullet_tp_position, max_spoofed_eye_distance ) ) {
				start_shooting = true;
				manipulation_indicator = true;
			} else {
				start_shooting = false;
				manipulation_indicator = false;
			}
		}

		inline void find_bullet_tp_angles( float max_desync_value ) {
			const auto& target = get_target( );
			if ( !memory::is_valid( target.player ) ) {
				bullet_tp_angle = vec3_t( );

				cached_bullet_tp_position = target.pos;
				bullet_tp_point_visible = false;
				return;
			}

			vec3_t pos = target.pos;
			if ( sdk::c_game_physics::is_visible( sdk::local_player->eyes( )->position( ), pos ) ) {
				bullet_tp_angle = target.pos;

				cached_bullet_tp_position = target.pos;
				return;
			}

			static bool los_check = false;
			if ( !sdk::c_game_physics::is_visible( cached_manip_point, target.pos ) )
				los_check = false;
			else
				los_check = true;

			static vec3_t point;
			if ( !target.player || cached_bullet_tp_position != point || target.player->user_id( ) != target_id ) {
				bullet_tp_angle = target.pos;
				cached_bullet_tp_position = target.pos;
				bullet_tp_point_visible = false;
			}

			if ( !sdk::c_game_physics::is_visible( cached_manip_point, cached_bullet_tp_position ) || cached_bullet_tp_position.empty( ) || target.player->user_id( ) != target_id )
				bullet_tp_point_visible = false;

			if ( sdk::c_game_physics::is_visible( cached_manip_point, cached_bullet_tp_position ) &&
				 validate_eye_pos( target.player, cached_bullet_tp_position, true ) ) {

				bullet_tp_angle = cached_bullet_tp_position;
				bullet_tp_point_visible = true;
			}

			std::vector< vec3_t > arrz{ };
			if ( !bullet_tp_point_visible && !los_check || cached_bullet_tp_position.empty( ) && memory::is_valid( target.player ) ) {
				if ( config::aimbot_bullet_tp_mode == 0 )
					bullet_tp_angles_mode_intense( arrz, 100 );
				else if ( config::aimbot_bullet_tp_mode == 1 )
					bullet_tp_angles_mode_intense( arrz, 250 );
				else if ( config::aimbot_bullet_tp_mode == 2 )
					arrz = generate_points_in_non_uniform_sphere( 2.3, 2.2, 2.2, 100 );

				for ( const auto& s : arrz ) {
					point = target.pos + s;
					bullet_tp_seperator = target.pos + s;

					if ( sdk::c_game_physics::is_visible( cached_manip_point, point ) &&
						 validate_eye_pos( target.player, point, true ) ) {

						cached_bullet_tp_position = point;
						bullet_tp_point_visible = false;

						bullet_tp_angle = target.pos + s;
						return;
					}
				}
			}
		}
	};

	auto& aimbot = c_aimbot::instance( );
}
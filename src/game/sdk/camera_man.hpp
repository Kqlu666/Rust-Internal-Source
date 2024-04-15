#pragma once

namespace sdk {
	class c_player_cull {
	public:
		static bool enabled( ) {
			il2cpp_method( ptr, _( "ConVar" ), _( "PlayerCull" ), _( "get_enabled" ), -1, void ( * )( ) );
			return safe.call< bool >( ptr );
		}

		static auto set_enabled( bool state ) {
			il2cpp_method( ptr, _( "ConVar" ), _( "PlayerCull" ), _( "set_enabled" ), 1, void ( * )( bool ) );
			safe.call< void >( ptr, state );
		}
	};

	class c_camera_man {
	public:
		inline auto update( ) {
			if ( convar::c_client::cam_lerp( ) >= 1.f ) {
				position = camera->transform( )->position( );
				rotation = camera->transform( )->rotation( );
			}

			zoom = vec3_t::inverse_lerp( 1.f, 120.f, camera->field_of_view( ) );
			do_controls( );
		}

		inline auto on_enable( ) {
			start_culling = c_player_cull::enabled( );
			c_player_cull::set_enabled( false );

			local_player->set_frozen( true );

			position = camera->transform( )->position( );
			rotation = camera->transform( )->rotation( );
			view = camera->transform( )->euler_angles( );
		}

		inline auto on_disable( ) {
			c_player_cull::set_enabled( start_culling );
			local_player->set_frozen( false );
		}

		inline void do_controls( ) {
			auto clamp = [ ]( float value, float min, float max ) {
				if ( value < min ) {
					value = min;
				} else if ( value > max ) {
					value = max;
				}
				return value;
			};

			auto clamp01 = [ ]( float value ) {
				float num;
				if ( value < 0.f ) {
					num = 0.f;
				} else if ( value > 1.f ) {
					num = 1.f;
				} else {
					num = value;
				}
				return num;
			};

			auto lerp = [ & ]( float a, float b, float t ) {
				return a + ( b - a ) * clamp01( t );
			};

			if ( memory::is_valid( local_player ) || local_player->frozen( ) ) {
				if ( sdk::c_buttons::attack2( )->is_down( ) && ( !need_both_mouse_buttons_to_zoom || sdk::c_buttons::attack( )->is_down( ) ) ) {
					zoom = clamp( zoom + unity::c_input::axis_raw( _( "Mouse Y" ) ) * -0.033f, 0.01f, 1.f );
				} else if ( sdk::c_buttons::duck( )->is_down( ) ) {
					view.z = view.z + unity::c_input::axis_raw( _( "Mouse X" ) ) * 1.5f;
				} else {
					view.y = view.y + unity::c_input::axis_raw( _( "Mouse X" ) ) * 3.f * zoom * look_sensitivity;
					view.x = view.x - unity::c_input::axis_raw( _( "Mouse Y" ) ) * 3.f * zoom * look_sensitivity;
				}

				rotation = unity::c_quaternion::euler( view );

				vec3_t vector{ };
				if ( sdk::c_buttons::forward( )->is_down( ) )
					vector += vec3_t( 0.f, 0.f, 1.f );

				if ( sdk::c_buttons::backward( )->is_down( ) )
					vector += vec3_t( 0.f, 0.f, -1.f );

				if ( sdk::c_buttons::left( )->is_down( ) )
					vector += vec3_t( -1.f, 0.f, 0.f );

				if ( sdk::c_buttons::right( )->is_down( ) )
					vector += vec3_t( 1.f, 0.f, 0.f );

				vector.unity_normalize( );

				if ( sdk::c_buttons::sprint( )->is_down( ) )
					vector *= 5.f;

				if ( sdk::c_buttons::jump( )->is_down( ) )
					vector *= 0.2f;

				velocity += rotation * vector * 0.05f * move_speed * convar::c_client::cam_speed( );
				velocity = vec3_t::lerp( velocity, vec3_t{ }, 0.2f );
				position += velocity;
			} else {
				vec3_t vector2 = local_player->transform( )->position( ) - position + vec3_t( 0.f, 1.f, 0.f );
				rotation = quat_t::look_rot( vector2 );
			}

			camera->transform( )->set_position( vec3_t::lerp( camera->transform( )->position( ), position, convar::c_client::cam_lerp( ) ) );
			camera->transform( )->set_rotation( unity::c_quaternion::slerp( camera->transform( )->rotation( ), rotation, convar::c_client::cam_lerp( ) ) );
			camera->set_field_of_view( lerp( 1.f, 120.f, zoom ) );
		}
	private:
		vec3_t view;
		vec3_t position;
		quat_t rotation;
		vec3_t velocity;

		float zoom;
		float move_speed = 1.f;
		float look_sensitivity = 1.f;

		bool start_culling;
		bool need_both_mouse_buttons_to_zoom;
	};
}
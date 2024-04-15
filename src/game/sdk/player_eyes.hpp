#pragma once

namespace sdk {
	class c_player_eyes {
	public:
		il2cpp_field_get( quat_t, body_rotation, _( "" ), _( "PlayerEyes" ), _( "<bodyRotation>k__BackingField" ) );

		il2cpp_field_get( vec3_t, view_offset, _( "" ), _( "PlayerEyes" ), _( "viewOffset" ) );
		il2cpp_field_set( vec3_t, set_view_offset, _( "" ), _( "PlayerEyes" ), _( "viewOffset" ) );

		inline auto center( ) {
			il2cpp_method( ptr, _( "" ), _( "PlayerEyes" ), _( "get_center" ), -1, vec3_t( * )( c_player_eyes* ) );
			return safe.call< vec3_t >( ptr, this );
		}

		inline auto position( ) {
			il2cpp_method( ptr, _( "" ), _( "PlayerEyes" ), _( "get_position" ), -1, vec3_t( * )( c_player_eyes* ) );
			return safe.call< vec3_t >( ptr, this );
		}

		inline auto rotation( ) {
			il2cpp_method( ptr, _( "" ), _( "PlayerEyes" ), _( "get_rotation" ), -1, quat_t( * )( c_player_eyes* ) );
			return safe.call< quat_t >( ptr, this );
		}

		inline auto set_rotation( quat_t rot ) {
			il2cpp_method( ptr, _( "" ), _( "PlayerEyes" ), _( "set_rotation" ), -1, void( * )( c_player_eyes*, quat_t ) );
			safe.call< void >( ptr, this, rot );
		}

		inline void set_body_rotation( quat_t rotation ) {
			il2cpp_method( ptr, _( "" ), _( "PlayerEyes" ), _( "set_bodyRotation" ), 1, void ( * )( c_player_eyes*, quat_t ) );
			safe.call< void >( ptr, this, rotation );
		}

		inline auto head_forward( ) {
			il2cpp_method( ptr, _( "" ), _( "PlayerEyes" ), _( "HeadForward" ), -1, vec3_t( * )( c_base_player* ) );
			return safe.call< vec3_t >( ptr, this );
		}

		inline auto body_forward( ) {
			il2cpp_method( ptr, _( "" ), _( "PlayerEyes" ), _( "BodyForward" ), -1, vec3_t( * )( c_base_player* ) );
			return safe.call< vec3_t >( ptr, this );
		}

		inline auto body_right( ) {
			il2cpp_method( ptr, _( "" ), _( "PlayerEyes" ), _( "BodyRight" ), -1, vec3_t( * )( c_base_player* ) );
			return safe.call< vec3_t >( ptr, this );
		}

		inline auto movement_right( ) {
			il2cpp_method( ptr, _( "" ), _( "PlayerEyes" ), _( "MovementRight" ), -1, vec3_t( * )( c_base_player* ) );
			return safe.call< vec3_t >( ptr, this );
		}
	};
}
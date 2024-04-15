#pragma once

namespace sdk {
	enum e_button {
		FORWARD = 2,
		BACKWARD = 4,
		LEFT = 8,
		RIGHT = 16,
		JUMP = 32,
		DUCK = 64,
		SPRINT = 128,
		USE = 256,
		FIeye_positionRIMARY = 1024,
		FIRE_SECONDARY = 2048,
		RELOAD = 8192,
		FIRE_THIRD = 134217728
	};

	class c_input_message {
	public:
		il2cpp_field_set( vec3_t, set_aim_angles, _( "" ), _( "InputMessage" ), _( "aimAngles" ) );
	};

	class c_input_state {
	public:
		il2cpp_field_get( c_input_message*, current, _( "" ), _( "InputState" ), _( "current" ) );

		inline auto is_down( e_button button ) {
			il2cpp_method( ptr, _( "" ), _( "InputState" ), _( "IsDown" ), 1, bool( * )( c_held_entity*, e_button ) );
			return safe.call< bool >( ptr, this, button );
		}

		inline auto was_just_pressed( e_button button ) {
			il2cpp_method( ptr, _( "" ), _( "InputState" ), _( "WasJustPressed" ), 1, void( * )( c_held_entity*, e_button ) );
			return safe.call< bool >( ptr, this, button );
		}
	};
}
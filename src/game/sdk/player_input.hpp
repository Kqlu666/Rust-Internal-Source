#pragma once

namespace sdk {

	class c_player_input {
	public:
		il2cpp_field_get( c_input_state*, state, _( "" ), _( "PlayerInput" ), _( "state" ) );
		il2cpp_field_get( vec2_t, recoil_angles, _( "" ), _( "PlayerInput" ), _( "recoilAngles" ) );

		il2cpp_field_get( vec2_t, body_angles, _( "" ), _( "PlayerInput" ), _( "bodyAngles" ) );
		il2cpp_field_set( vec2_t, set_body_angles, _( "" ), _( "PlayerInput" ), _( "bodyAngles" ) );
	};
}
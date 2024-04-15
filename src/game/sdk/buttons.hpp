#pragma once

namespace sdk {
	class c_con_button {
	public:
		inline auto is_down( ) {
			static const auto& game_assembly = pe::get_image( HASH( "gameassembly" ) );
			return safe.call< bool >( game_assembly + 0x3DCF10, this );
		}
	};

	class c_buttons {
	public:
		il2cpp_static_field_get( c_con_button*, jump, _( "" ), _( "Buttons" ), _( "Jump" ) );
		il2cpp_static_field_get( c_con_button*, left, _( "" ), _( "Buttons" ), _( "Left" ) );
		il2cpp_static_field_get( c_con_button*, duck, _( "" ), _( "Buttons" ), _( "Duck" ) );
		il2cpp_static_field_get( c_con_button*, right, _( "" ), _( "Buttons" ), _( "Right" ) );
		il2cpp_static_field_get( c_con_button*, attack, _( "" ), _( "Buttons" ), _( "Attack" ) );
		il2cpp_static_field_get( c_con_button*, sprint, _( "" ), _( "Buttons" ), _( "Sprint" ) );
		il2cpp_static_field_get( c_con_button*, attack2, _( "" ), _( "Buttons" ), _( "Attack2" ) );
		il2cpp_static_field_get( c_con_button*, forward, _( "" ), _( "Buttons" ), _( "Forward" ) );
		il2cpp_static_field_get( c_con_button*, backward, _( "" ), _( "Buttons" ), _( "Backward" ) );
	};
}
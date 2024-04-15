#pragma once

namespace sdk {
	class c_base_movement {
	public:
		inline auto target_movement( ) {
			il2cpp_method( ptr, _( "" ), _( "BaseMovement" ), _( "get_TargetMovement" ), -1, vec3_t( * )( c_base_movement* ) );
			return safe.call< vec3_t >( ptr, this );
		}

		inline auto set_target_movement( vec3_t pos ) {
			il2cpp_method( ptr, _( "" ), _( "BaseMovement" ), _( "set_TargetMovement" ), 1, void( * )( c_base_movement*, vec3_t ) );
			return safe.call< void>( ptr, this, pos );
		}
	};
}
#pragma once

namespace sdk {
	class c_metabolism_attribute {
	public:
		il2cpp_field_set( float, set_value, _( "" ), _( "MetabolismAttribute" ), _( "value" ) );
	};

	class c_player_metabolism {
	public:
		il2cpp_field_get( c_metabolism_attribute*, oxygen, _( "" ), _( "PlayerMetabolism" ), _( "oxygen" ) );
	};
}
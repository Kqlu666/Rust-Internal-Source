#pragma once

namespace sdk {
	class c_water_level {
	public:
		static auto test( vec3_t pos, bool waves, bool volumes, c_base_player* for_entity ) {
			il2cpp_method( ptr, _( "" ), _( "WaterLevel" ), _( "Test" ), 4,
						   bool( * )( vec3_t, bool, bool, c_base_entity* ) );
			return safe.call< bool >( ptr, pos, waves, volumes, for_entity );
		}

		static auto water_depth( vec3_t pos, bool waves, bool volumes, c_base_player* for_entity ) {
			il2cpp_method( ptr, _( "" ), _( "WaterLevel" ), _( "GetWaterDepth" ), 4,
						   bool( * )( vec3_t, bool, bool, c_base_entity* ) );
			return safe.call< bool >( ptr, pos, waves, volumes, for_entity );
		}
	};
}
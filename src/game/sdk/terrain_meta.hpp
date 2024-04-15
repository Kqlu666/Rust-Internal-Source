#pragma once

namespace sdk {
	class c_terrain_height_map {
	public:
		inline auto normal( vec3_t world_pos ) {
			il2cpp_method( ptr, _( "" ), _( "TerrainHeightMap" ), _( "GetNormal" ), 1, vec3_t( * )( c_terrain_height_map*, vec3_t ) );
			return safe.call< vec3_t >( ptr, this, world_pos );
		}
	};

	class c_terrain_meta {
	public:
		static auto height_map( ) {
			il2cpp_method( ptr, _( "" ), _( "TerrainMeta" ), _( "get_HeightMap" ), -1, c_terrain_height_map * ( * )( ) );
			return safe.call< c_terrain_height_map* >( ptr );
		}
	};
}
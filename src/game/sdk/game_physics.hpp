#pragma once

#include "../../options/config_mgr.hpp"

namespace sdk {
	class c_game_physics {
	public:
		static auto is_visible( vec3_t start, vec3_t end, float p1 = 0.2f ) {
			il2cpp_method( ptr, _( "" ), _( "GamePhysics" ), _( "LineOfSightRadius" ), 5, bool( * )( vec3_t, vec3_t, int, float, void* ) );

			int mask = 1503731969;
			return safe.call< bool >( ptr, start, end, mask, p1, nullptr );
		}

		static auto line_of_sight( vec3_t p0, vec3_t p1, int layer_mask, void* ignore_entity ) {
			il2cpp_method( ptr, _( "" ), _( "GamePhysics" ), _( "LineOfSight" ), 4, bool( * )( vec3_t, vec3_t, int, void* ) );
			return safe.call< bool >( ptr, p0, p1, layer_mask, ignore_entity );
		}

		static auto verify( unity::raycast_hit_t hit_info, void* ignore_entity ) {
			il2cpp_method( ptr, _( "" ), _( "GamePhysics" ), _( "Verify" ), 2, bool( * )( vec3_t, vec3_t, int, float ) );
			return safe.call< bool >( ptr, hit_info, nullptr );
		}

		static auto trace( unity::ray_t ray, float radius, unity::raycast_hit_t hit_info, float max_distance, int layer_mask, unity::e_query_trigger_interaction trigger_interaction, void* ignore_entity ) {
			il2cpp_method( ptr, _( "" ), _( "GamePhysics" ), _( "Trace" ), 7, bool( * )( vec3_t, vec3_t, int, float ) );
			return safe.call< bool >( ptr, ray, radius, hit_info, max_distance, layer_mask, trigger_interaction, ignore_entity );
		}
	};
}
#pragma once

namespace sdk {
	class c_skinned_multi_mesh {
	public:
		inline auto renderers( ) {
			il2cpp_method( ptr, _( "" ), _( "SkinnedMultiMesh" ), _( "get_Renderers" ), -1, unity::c_list< unity::c_renderer* >*( * )( c_skinned_multi_mesh* ) );
			return safe.call< unity::c_list< unity::c_renderer* >* >( ptr, this );
		}
	};

	class c_player_model {
	public:
		il2cpp_field_get( vec3_t, velocity, _( "" ), _( "PlayerModel" ), _( "velocity" ) );
		il2cpp_field_get( vec3_t, position, _( "" ), _( "PlayerModel" ), _( "position" ) );
		il2cpp_field_get( c_skinned_multi_mesh*, multi_mesh, _( "" ), _( "PlayerModel" ), _( "_multiMesh" ) );
		il2cpp_field_get( vec3_t, new_velocity, _( "" ), _( "PlayerModel" ), _( "newVelocity" ) );
		il2cpp_field_get( bool, is_local_player, _( "" ), _( "PlayerModel" ), _( "isLocalPlayer" ) );

		inline auto is_npc( ) {
			il2cpp_method( ptr, _( "" ), _( "PlayerModel" ), _( "get_IsNpc" ), -1, bool( * )( c_player_model* ) );
			return safe.call< bool >( ptr, this );
		}

		static auto rebuild_all( ) {
			il2cpp_method( ptr, _( "" ), _( "PlayerModel" ), _( "RebuildAll" ), -1, void( * )( ) );
			safe.call< void >( ptr );
		}
	};
}
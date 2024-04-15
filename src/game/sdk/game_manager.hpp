#pragma once

namespace sdk {
	class c_game_manager {
	public:
		il2cpp_static_field_get( c_game_manager*, instance, _( "" ), _( "GameManager" ), _( "client" ) );

		inline auto create_prefab( const char* prefab ) {
			il2cpp_method( ptr, _( "" ), _( "GameManager" ), _( "CreatePrefab" ), 2, unity::c_game_object * ( * )( c_game_manager*, unity::c_string*, bool ) );
			return safe.call< unity::c_game_object* >( ptr, this, il2cpp::new_string( prefab ), true );
		}
	};
}
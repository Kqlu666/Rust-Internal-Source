#pragma once

namespace sdk {
	class c_dropped_item_container {
	public:
		il2cpp_field_get( uint32_t, player_steam_id, _( "" ), _( "DroppedItemContainer" ), _( "playerSteamID" ) );

		inline auto player_name( ) {
			il2cpp_method( ptr, _( "" ), _( "DroppedItemContainer" ), _( "get_playerName" ), -1, unity::c_string * ( * )( c_dropped_item_container* ) );
			return safe.call< unity::c_string* >( ptr, this );
		}
	};
}
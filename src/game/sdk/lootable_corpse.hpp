#pragma once

namespace sdk {
	class c_lootable_corpse {
	public:
		inline auto player_name( ) {
			il2cpp_method( ptr, _( "" ), _( "LootableCorpse" ), _( "get_playerName" ), -1, unity::c_string * ( * )( c_lootable_corpse* ) );
			return safe.call< unity::c_string* >( ptr, this );
		}
	};
}
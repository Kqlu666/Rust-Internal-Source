#pragma once

namespace sdk {
	namespace facepunch {

		class c_steam_platform {
		public:
			inline auto set_username( unity::c_string* name ) {
				il2cpp_method( ptr, _( "Rust.Platform.Steam" ), _( "SteamPlatform" ), _( "set_UserName" ), 1, void ( * )( c_steam_platform*, unity::c_string* ) );
				return safe.call< unity::c_string* >( ptr, this, name );
			}
		};

		class c_random_usernames {
		public:
			static auto get( unsigned v ) {
				il2cpp_method( ptr, _( "Facepunch" ), _( "RandomUsernames" ), _( "Get" ), 1, unity::c_string*( * )( unsigned ) );
				return safe.call< unity::c_string* >( ptr, v );
			}
		};
	}
}
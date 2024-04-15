#pragma once

namespace sdk {
	class c_stash_container {
	public:
		inline auto is_hidden( ) {
			il2cpp_method( ptr, _( "" ), _( "StashContainer" ), _( "IsHidden" ), -1, bool( * )( c_stash_container* ) );
			return safe.call< bool >( ptr, this );
		}
	};
}
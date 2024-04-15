#pragma once

namespace sdk {
	class c_model_state {
	public:
		il2cpp_field_get( int, flags, _( "" ), _( "ModelState" ), _( "flags" ) );
		il2cpp_field_set( int, set_flags, _( "" ), _( "ModelState" ), _( "flags" ) );

		inline void remove_flag( int flag ) {
			auto player_flags = flags( );
			player_flags &= ~flag;

			set_flags( player_flags );
		}

		inline void add_flag( int flag ) {
			auto player_flags = flags( );
			set_flags( player_flags |= flag );
		}

		inline bool has_flag( int flag ) {
			auto player_flags = flags( );
			return ( player_flags & flag );
		}

		inline auto set_sprinting( bool state ) {
			il2cpp_method( ptr, _( "" ), _( "ModelState" ), _( "set_sprinting" ), 1, void ( * )( c_model_state*, bool ) );
			safe.call< void >( ptr, this, state );
		}

		inline auto set_on_ladder( bool state ) {
			il2cpp_method( ptr, _( "" ), _( "ModelState" ), _( "set_onLadder" ), -1, void ( * )( c_model_state*, bool ) );
			safe.call< void >( ptr, this, state );
		}

		inline auto ducked( ) {
			il2cpp_method( ptr, _( "" ), _( "ModelState" ), _( "get_ducked" ), -1, bool ( * )( c_base_player* ) );
			return safe.call< bool >( ptr, this );
		}

		inline auto crawling( ) {
			il2cpp_method( ptr, _( "" ), _( "ModelState" ), _( "get_crawling" ), -1, bool ( * )( c_base_player* ) );
			return safe.call< bool >( ptr, this );
		}

		inline auto mounted( ) {
			il2cpp_method( ptr, _( "" ), _( "ModelState" ), _( "get_mounted" ), -1, bool ( * )( c_base_player* ) );
			return safe.call< bool >( ptr, this );
		}
	};
}
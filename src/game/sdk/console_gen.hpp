#pragma once

namespace sdk {
	struct cs_option_t {
		bool IsServer; // 0x0
		bool IsClient;// 0x1
		bool ForwardtoServerOnMissing; // 0x2
		bool PrintOutput; // 0x3
		bool IsUnrestricted; // 0x4
		bool FromRcon; // 0x5
		bool IsFromServer; // 0x6

		cs_option_t( ) : IsClient( true ), ForwardtoServerOnMissing( true ), PrintOutput( true ) { }
	};

	class c_console_command {
	public:
		inline auto name( ) {
			return mem::read< unity::c_string* >( this + 0x10 );
		}

		inline auto allow_run_from_server( ) {
			return mem::read< bool >( this + 0x58 );
		}

		inline auto set_allow_run_from_server( bool state ) {
			mem::write< bool >( this + 0x58, state );
		}
	};

	class c_console_system {
	public:
		static auto all( ) {
			static const auto& gameassembly = pe::get_image( HASH( "gameassembly" ) );
			static const auto& ptr = reinterpret_cast< unity::c_array< c_console_command* >*( * )( ) >( gameassembly + 0xE8C570 ); // ConsoleSystem.Index$$get_All
			return safe.call< unity::c_array< c_console_command* >* >( ptr );
		}

		static auto run( const char* cmd, uintptr_t* args ) {
			il2cpp_method( ptr, _( "" ), _( "ConsoleSystem" ), _( "Run" ), 3, unity::c_string*( * )( cs_option_t *, uintptr_t, uintptr_t * ) );
			return safe.call< unity::c_string* >( ptr, new cs_option_t( ), il2cpp::new_string( cmd ), args );
		}
	};
}
#pragma once

namespace sdk {
	class c_console_ui {
	public:
		static auto instance( ) {
			static const auto& game_assembly = pe::get_image( HASH( "gameassembly" ) );
			return mem::read_chain< c_console_ui* >( game_assembly, { 0x3480BF8, 0xB8, 0x0 } );
		}

		inline auto log( const char* message ) {
			il2cpp_method( ptr, _( "" ), _( "ConsoleUI" ), _( "Log" ), 1, void( * )( c_console_ui*, unity::c_string* ) );
			safe.call< void >( ptr, this, il2cpp::new_string( message ) );
		}
	};
}
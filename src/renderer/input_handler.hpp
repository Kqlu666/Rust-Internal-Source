#pragma once

class c_input_handler : public singleton< c_input_handler > {
public:
	vec2_t scroll_delta{ };

	inline bool valid_key( int key ) {
		if ( key > 0 && key < 256 )
			return true;

		return false;
	}

	inline bool is_hold( int key ) {
		if ( !valid_key( key ) )
			return false;

		return _is_down[ key ];
	}

	inline bool is_down( int key ) {
		if ( !valid_key( key ) )
			return false;

		return _went_down[ key ];
	}

	inline bool is_up( int key ) {
		if ( !valid_key( key ) )
			return false;

		return _went_up[ key ];
	}

	inline void reset( ) {
		for ( uint32_t i = 0; i < 256; i++ ) {
			_is_down[ i ] = false;

			_went_up[ i ] = false;
			_went_down[ i ] = false;
		}
	}

	inline void update( ) {
		for ( uint32_t i = 0; i < 256; i++ ) {
			auto result = nt::NtUserGetAsyncKeyState( i );

			if ( result != 0 ) {
				if ( !_is_down[ i ] )
					_went_down[ i ] = true;
				else
					_went_down[ i ] = false;

				_went_up[ i ] = false;
			} else {
				if ( _is_down[ i ] )
					_went_up[ i ] = true;
				else
					_went_up[ i ] = false;

				_went_down[ i ] = false;
			}

			_is_down[ i ] = result != 0;

		}
	}

	inline auto handle( ) {
		if ( ImGui::GetCurrentContext( ) == nullptr )
			return;

		ImGuiIO& io = ImGui::GetIO( );

		if ( nt::NtUserGetForegroundWindow( ) != renderer.get_window( ) ) {
			reset( );
			io.AddFocusEvent( false );
			return;
		}
		
		update( );

		auto mouse_position = unity::c_input::mouse_position( );
		mouse_position.y = unity::c_screen::height( ) - mouse_position.y;

		io.AddMousePosEvent( mouse_position.x, mouse_position.y );
		io.AddFocusEvent( true );

		io.AddMouseWheelEvent( scroll_delta.x, scroll_delta.y );

		if ( is_down( VK_LBUTTON ) )
			io.AddMouseButtonEvent( 0, true );
		else if ( is_up( VK_LBUTTON ) )
			io.AddMouseButtonEvent( 0, false );

		if ( is_down( VK_RBUTTON ) )
			io.AddMouseButtonEvent( 1, true );
		else if ( is_up( VK_RBUTTON ) )
			io.AddMouseButtonEvent( 1, false );

		if ( is_down( VK_MBUTTON ) )
			io.AddMouseButtonEvent( 2, true );
		else if ( is_up( VK_MBUTTON ) )
			io.AddMouseButtonEvent( 2, false );

		if ( is_down( VK_XBUTTON1 ) )
			io.AddMouseButtonEvent( 3, true );
		else if ( is_up( VK_XBUTTON1 ) )
			io.AddMouseButtonEvent( 3, false );

		if ( is_down( VK_XBUTTON2 ) )
			io.AddMouseButtonEvent( 4, true );
		else if ( is_up( VK_XBUTTON2 ) )
			io.AddMouseButtonEvent( 4, false );

		for ( auto i = 0x07; i < 256; ++i ) {
			const auto& is_key_down = is_down( i );
			const auto& is_key_up = is_up( i );
			if ( !is_key_down && !is_key_up )
				continue;

			ImGui_ImplWin32_UpdateKeyModifiers( );

			const ImGuiKey key = ImGui_ImplWin32_VirtualKeyToImGuiKey( i );
			if ( key != ImGuiKey_None )
				ImGui_ImplWin32_AddKeyEvent( key, is_key_down, i );

			if ( i == VK_SHIFT ) {
				if ( IsVkDown( VK_LSHIFT ) == is_key_down ) 
					ImGui_ImplWin32_AddKeyEvent( ImGuiKey_LeftShift, is_key_down, VK_LSHIFT );
				if ( IsVkDown( VK_RSHIFT ) == is_key_down ) 
					ImGui_ImplWin32_AddKeyEvent( ImGuiKey_RightShift, is_key_down, VK_RSHIFT );
			} else if ( i == VK_CONTROL ) {
				if ( IsVkDown( VK_LCONTROL ) == is_key_down ) 
					ImGui_ImplWin32_AddKeyEvent( ImGuiKey_LeftCtrl, is_key_down, VK_LCONTROL );
				if ( IsVkDown( VK_RCONTROL ) == is_key_down ) 
					ImGui_ImplWin32_AddKeyEvent( ImGuiKey_RightCtrl, is_key_down, VK_RCONTROL );
			} else if ( i == VK_MENU ) {
				if ( IsVkDown( VK_LMENU ) == is_key_down ) 
					ImGui_ImplWin32_AddKeyEvent( ImGuiKey_LeftAlt, is_key_down, VK_LMENU );
				if ( IsVkDown( VK_RMENU ) == is_key_down ) 
					ImGui_ImplWin32_AddKeyEvent( ImGuiKey_RightAlt, is_key_down, VK_RMENU );
			}
		}
	}

private:
	bool _is_down[ 256 ];

	bool _went_up[ 256 ];
	bool _went_down[ 256 ];
};

auto& input_handler = c_input_handler::instance( );
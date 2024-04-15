#pragma once

namespace sdk {
	class c_item_definition;

	class c_magazine {
	public:
		inline int contents( ) {
			return mem::read< int >( this + 0x1c );
		}

		inline void set_contents( int d ) {
			mem::write< int >( this + 0x1c, d );
		}

		inline auto ammo_type( ) {
			return mem::read< sdk::c_item_definition* >( this + 0x20 );
		}
	};
}
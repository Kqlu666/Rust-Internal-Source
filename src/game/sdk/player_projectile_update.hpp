#pragma once

namespace sdk {
	class c_player_projectile_update {
	public:
		inline auto set_projectile_id( int id ) {
			mem::write< int >( this + 0x10, id );
		}

		inline auto set_travel_time( float time ) {
			mem::write< float >( this + 0x2C, time );
		}

		inline auto set_cur_velocity( vec3_t velocity ) {
			mem::write< vec3_t >( this + 0x20, velocity );
		}

		inline auto cur_position( ) {
			return mem::read< vec3_t >( this + 0x14 );
		}

		inline auto set_cur_position( vec3_t pos ) {
			mem::write< vec3_t >( this + 0x14, pos );
		}
	};
}
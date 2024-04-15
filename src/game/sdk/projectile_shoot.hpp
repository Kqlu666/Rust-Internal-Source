#pragma once

namespace sdk {
	class c_projectile_shoot {
	public:

		class c_projectile {
		public:
			inline auto start_velocity( ) {
				return mem::read< vec3_t >( this + 0x20 );
			}

			inline auto set_start_velocity( vec3_t velocity ) {
				mem::write< vec3_t >( this + 0x20, velocity );
			}

			inline auto start_position( ) {
				return mem::read< vec3_t >( this + 0x14 );
			}

			inline auto set_start_position( vec3_t pos ) {
				mem::write< vec3_t >( this + 0x14, pos );
			}
		};

		il2cpp_field_get( unity::c_list< c_projectile* >*, projectiles, _( "ProtoBuf" ), _( "ProjectileShoot" ), _( "projectiles" ) );
	};
}
#pragma once

namespace sdk {
	class c_base_heli_weak_spot {
	public:
		inline auto health( ) {
			return mem::read< float >( this + 0x24 );
		}

		inline auto max_health( ) {
			return mem::read< float >( this + 0x20 );
		}
	};

	class c_patrol_helicopter : public c_base_combat_entity {
	public:
		il2cpp_field_get( unity::c_array< c_base_heli_weak_spot* >*, weakspots, _( "" ), _( "PatrolHelicopter" ), _( "weakspots" ) );
	};
}
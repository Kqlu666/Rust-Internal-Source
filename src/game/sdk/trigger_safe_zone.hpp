#pragma once

namespace sdk {
	class c_trigger_safe_zone {
	public:
		il2cpp_static_field_get( unity::c_list< c_trigger_safe_zone* >*, all_safe_zones, _( "" ), _( "TriggerSafeZone" ), _( "allSafeZones" ) );
		il2cpp_static_field_set( unity::c_list< c_trigger_safe_zone* >*, set_all_safe_zones, _( "" ), _( "TriggerSafeZone" ), _( "allSafeZones" ) );

		inline auto on_enable( ) {
			il2cpp_method( ptr, _( "" ), _( "TriggerSafeZone" ), _( "OnEnable" ), -1, void ( * )( c_trigger_safe_zone* ) );
			safe.call< void >( ptr, this );
		}

		inline auto on_disable( ) {
			il2cpp_method( ptr, _( "" ), _( "TriggerSafeZone" ), _( "OnDisable" ), -1, void ( * )( c_trigger_safe_zone* ) );
			safe.call< void >( ptr, this );
		}
	};
}
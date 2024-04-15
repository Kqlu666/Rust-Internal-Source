#pragma once

namespace sdk {

	enum class e_marker_type {
		undefined,
		player,
		explosion,
		vending_machine,
		ch47,
		cargo_ship,
		crate,
		generic_radius,
		patrol_helicopter
	};

	class c_map_marker : public c_base_entity {
	public:
		il2cpp_static_field_get( unity::c_list< c_map_marker* >*, map_markers, _( "" ), _( "MapMarker" ), _( "mapMarkers" ) )

		il2cpp_field_set( e_marker_type, set_app_type, _( "" ), _( "MapMarker" ), _( "appType" ) );

		static auto add( c_map_marker* map_marker ) {
			map_markers( )->add( map_marker );
		}
	};
}
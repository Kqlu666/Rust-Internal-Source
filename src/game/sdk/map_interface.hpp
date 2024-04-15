#pragma once

namespace sdk {

	struct vec2_unity_t {
	public:
		float x;
		float y;
	};

	class c_map_view {
	public:
		il2cpp_field_get( unity::c_graphic*, map_image, _( "" ), _( "MapView" ), _( "mapImage" ) );
		il2cpp_field_get( unity::c_scroll_rect_ex*, scroll_rect, _( "" ), _( "MapView" ), _( "scrollRect" ) );

		inline vec2_unity_t world_to_map( vec3_t world_pos ) {
			il2cpp_method( ptr, _( "" ), _( "MapView" ), _( "WorldPosToImagePos" ), 1, vec2_unity_t*( * )( c_map_view*, vec3_t ) );
			return safe.call< vec2_unity_t >( ptr, this, world_pos );
		}
	};

	class c_map_interface {
	public:
		il2cpp_static_field_get( bool, is_open, _( "" ), _( "MapInterface" ), _( "IsOpen" ) );

		il2cpp_field_get( c_map_view*, map_view, _( "" ), _( "MapInterface" ), _( "View" ) );
		il2cpp_field_get( unity::c_graphic*, camera_positon, _( "" ), _( "MapInterface" ), _( "cameraPositon" ) );
	};

	class c_ui_inventory {
	public:
		il2cpp_static_field_get( bool, is_open, _( "" ), _( "UIInventory" ), _( "isOpen" ) );
	};
}
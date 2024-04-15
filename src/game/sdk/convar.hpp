#pragma once

namespace sdk {
	namespace convar {
		class c_input {
		public:
			il2cpp_static_field_set( bool, set_sprinting, _( "ConVar" ), _( "Input" ), _( "alwayssprint" ) );
		};

		class c_admin {
		public:
			il2cpp_static_field_set( float, set_admintime, _( "ConVar" ), _( "Admin" ), _( "admintime" ) );
		};

		class c_client {
		public:
			il2cpp_static_field_get( float, cam_lerp, _( "ConVar" ), _( "Client" ), _( "camlerp" ) );
			il2cpp_static_field_get( float, cam_speed, _( "ConVar" ), _( "Client" ), _( "_camspeed" ) );

			il2cpp_static_field_set( float, set_cam_lerp, _( "ConVar" ), _( "Client" ), _( "camlerp" ) );
			il2cpp_static_field_set( float, set_cam_speed, _( "ConVar" ), _( "Client" ), _( "_camspeed" ) );
		};

		class c_weather {
		public:
			static void set_fog( float value ) {
				il2cpp_method( ptr, _( "ConVar" ), _( "Weather" ), _( "set_fog" ), 1, void ( * )( float ) );
				safe.call< void >( ptr, value );
			}

			static void set_wind( float value ) {
				il2cpp_method( ptr, _( "ConVar" ), _( "Weather" ), _( "set_wind" ), 1, void ( * )( float ) );
				safe.call< void >( ptr, value );
			}

			static void set_rain( float value ) {
				il2cpp_method( ptr, _( "ConVar" ), _( "Weather" ), _( "set_rain" ), 1, void ( * )( float ) );
				safe.call< void >( ptr, value );
			}

			static void set_rainbow( float value ) {
				il2cpp_method( ptr, _( "ConVar" ), _( "Weather" ), _( "set_rainbow" ), 1, void ( * )( float ) );
				safe.call< void >( ptr, value );
			}

			static void set_thunder( float value ) {
				il2cpp_method( ptr, _( "ConVar" ), _( "Weather" ), _( "set_thunder" ), 1, void ( * )( float ) );
				safe.call< void >( ptr, value );
			}

			static void set_cloud_opacity( float value ) {
				il2cpp_method( ptr, _( "ConVar" ), _( "Weather" ), _( "set_cloud_opacity" ), 1, void ( * )( float ) );
				safe.call< void >( ptr, value );
			}

			static void set_atmosphere_contrast( float value ) {
				il2cpp_method( ptr, _( "ConVar" ), _( "Weather" ), _( "set_atmosphere_contrast" ), 1, void ( * )( float ) );
				safe.call< void >( ptr, value );
			}
		};

		class c_culling {
		public:
			il2cpp_static_field_set( float, set_entity_min_cull_dist, _( "ConVar" ), _( "Culling" ), _( "entityMinCullDist" ) );
		};

		class c_graphics {
		public:
			il2cpp_static_field_set( float, set_fov, _( "ConVar" ), _( "Graphics" ), _( "_fov" ) );
		};
	}
}
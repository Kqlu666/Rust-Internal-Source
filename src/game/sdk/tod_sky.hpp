#pragma once

namespace sdk {
	class c_day_parameters {
	public:
		il2cpp_field_set( float, set_ambient_multiplier, _( "" ), _( "TOD_DayParameters" ), _( "AmbientMultiplier" ) );
		il2cpp_field_set( float, set_reflection_multiplier, _( "" ), _( "TOD_DayParameters" ), _( "ReflectionMultiplier" ) );
	};

	class c_sun_parameters {
	public:
		il2cpp_field_set( float, set_mesh_size, _( "" ), _( "TOD_SunParameters" ), _( "MeshSize" ) );
		il2cpp_field_set( float, set_mesh_contrast, _( "" ), _( "TOD_SunParameters" ), _( "MeshContrast" ) );
		il2cpp_field_set( float, set_mesh_brightness, _( "" ), _( "TOD_SunParameters" ), _( "MeshBrightness" ) );
	};

	class c_night_parameters {
	public:
		il2cpp_field_set( float, set_light_intensity, _( "" ), _( "TOD_NightParameters" ), _( "LightIntensity" ) );
		il2cpp_field_set( float, set_ambient_multiplier, _( "" ), _( "TOD_NightParameters" ), _( "AmbientMultiplier" ) );
	};

	class c_tod_ambient_parameters {
	public:
		il2cpp_field_set( float, set_saturation, _( "" ), _( "TOD_AmbientParameters" ), _( "Saturation" ) );
	};

	class c_tod_sky {
	public:
		il2cpp_field_get( c_day_parameters*, day, _( "" ), _( "TOD_Sky" ), _( "Day" ) );
		il2cpp_field_get( c_sun_parameters*, sun, _( "" ), _( "TOD_Sky" ), _( "Sun" ) );
		il2cpp_field_get( c_night_parameters*, night, _( "" ), _( "TOD_Sky" ), _( "Night" ) );
		il2cpp_field_get( c_tod_ambient_parameters*, ambient, _( "" ), _( "TOD_Sky" ), _( "Ambient" ) );

		static auto instance( ) {
			il2cpp_method( ptr, _( "" ), _( "TOD_Sky" ), _( "get_Instance" ), -1, c_tod_sky*( * )( ) );
			return safe.call< c_tod_sky* >( ptr );
		}
	};
}
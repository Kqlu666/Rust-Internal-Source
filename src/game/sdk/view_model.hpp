#pragma once

namespace sdk {
	class c_view_model_bob {
	public:
		il2cpp_field_get( float, bob_speed_run, _( "" ), _( "ViewmodelBob" ), _( "bobSpeedRun" ) );
		il2cpp_field_set( float, set_bob_speed_run, _( "" ), _( "ViewmodelBob" ), _( "bobSpeedRun" ) );

		il2cpp_field_get( float, bob_speed_walk, _( "" ), _( "ViewmodelBob" ), _( "bobSpeedWalk" ) );
		il2cpp_field_set( float, set_bob_speed_walk, _( "" ), _( "ViewmodelBob" ), _( "bobSpeedWalk" ) );
	};

	class c_view_model_lower {
	public:
		il2cpp_field_get( float, lower_scale, _( "" ), _( "ViewmodelLower" ), _( "lowerScale" ) );
		il2cpp_field_set( float, set_lower_scale, _( "" ), _( "ViewmodelLower" ), _( "lowerScale" ) );
	};

	class c_base_view_model : public unity::c_behaviour {
	public:
		il2cpp_field_get( uintptr_t, animator, _( "" ), _( "BaseViewModel" ), _( "animator" ) );
		il2cpp_field_set( uintptr_t, set_animator, _( "" ), _( "BaseViewModel" ), _( "animator" ) );

		il2cpp_field_get( c_view_model_bob*, bob, _( "" ), _( "BaseViewModel" ), _( "bob" ) );
		il2cpp_field_get( c_view_model_lower*, lower, _( "" ), _( "BaseViewModel" ), _( "lower" ) );

		il2cpp_field_set( bool, set_use_view_model_camera, _( "" ), _( "BaseViewModel" ), _( "useViewModelCamera" ) );

		static auto active_model( ) {
			il2cpp_method( ptr, _( "" ), _( "BaseViewModel" ), _( "get_ActiveModel" ), -1, c_base_view_model * ( * )( ) );
			return safe.call< c_base_view_model* >( ptr );
		}
	};

	class c_view_model {
	public:
		il2cpp_field_get( c_base_view_model*, instance, _( "" ), _( "ViewModel" ), _( "instance" ) );

		inline auto play( unity::c_string name, int layer = 0 ) {
			il2cpp_method( ptr, _( "" ), _( "ViewModel" ), _( "Play" ), -1, void( * )( c_view_model*, unity::c_string, int ) );
			safe.call< void >( ptr, this, name, layer );
		}
	};
}
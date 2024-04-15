#pragma once

namespace sdk {

	class c_game_manifest {
	public:
		static auto guid_to_object( unity::c_string* guid ) {
			il2cpp_method( ptr, _( "" ), _( "GameManifest" ), _( "GUIDToObject" ), 1, unity::c_game_object * ( * )( unity::c_string* ) );
			return safe.call< unity::c_game_object* >( ptr, guid );
		}
	};

	class c_game_object_ref {
	public:
		inline auto guid( ) {
			return mem::read< unity::c_string* >( this + 0x10 );
		}
	};

	class c_item_mod_projectile {
	public:
		il2cpp_field_get( c_game_object_ref*, projectile_object, _( "" ), _( "ItemModProjectile" ), _( "projectileObject" ) );
		il2cpp_field_get( float, projectile_spread, _( "" ), _( "ItemModProjectile" ), _( "projectileSpread" ) );
		il2cpp_field_get( float, projectile_velocity, _( "" ), _( "ItemModProjectile" ), _( "projectileVelocity" ) );
		il2cpp_field_get( float, projectile_velocity_spread, _( "" ), _( "ItemModProjectile" ), _( "projectileVelocitySpread" ) );

		il2cpp_field_set( float, set_projectile_spread, _( "" ), _( "ItemModProjectile" ), _( "projectileSpread" ) );
		il2cpp_field_set( float, set_projectile_velocity, _( "" ), _( "ItemModProjectile" ), _( "projectileVelocity" ) );
		il2cpp_field_set( float, set_projectile_velocity_spread, _( "" ), _( "ItemModProjectile" ), _( "projectileVelocitySpread" ) );
	};
}
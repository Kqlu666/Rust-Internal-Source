#pragma once

namespace sdk {
	class c_base_combat_entity : public c_base_entity {
	public:
		il2cpp_field_get( bool, lifestate, _( "" ), _( "BaseCombatEntity" ), _( "lifestate" ) );

		il2cpp_field_get( float, health, _( "" ), _( "BaseCombatEntity" ), _( "_health" ) );
		il2cpp_field_get( float, max_health, _( "" ), _( "BaseCombatEntity" ), _( "_maxHealth" ) );

		inline auto is_dead( ) {
			il2cpp_method( ptr, _( "" ), _( "BaseCombatEntity" ), _( "IsDead" ), -1, bool( * )( c_base_combat_entity* ) );
			return safe.call< bool >( ptr, this );
		}

		inline auto health_fraction( ) {
			il2cpp_method( ptr, _( "" ), _( "BaseCombatEntity" ), _( "get_healthFraction" ), -1, float( * )( c_base_combat_entity* ) );
			return safe.call< float >( ptr, this );
		}
	};
}
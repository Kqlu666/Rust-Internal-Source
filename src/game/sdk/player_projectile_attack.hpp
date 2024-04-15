#pragma once

namespace sdk {
	class c_player_projectile_attack {
	public:
		il2cpp_field_get( c_player_attack*, player_attack, _( "ProtoBuf" ), _( "PlayerProjectileAttack" ), _( "playerAttack" ) );
		il2cpp_field_set( c_player_attack*, set_player_attack, _( "ProtoBuf" ), _( "PlayerProjectileAttack" ), _( "playerAttack" ) );
		il2cpp_field_set( vec3_t, set_hit_velocity, _( "ProtoBuf" ), _( "PlayerProjectileAttack" ), _( "hitVelocity" ) );
		il2cpp_field_set( float, set_hit_distance, _( "ProtoBuf" ), _( "PlayerProjectileAttack" ), _( "hitDistance" ) );
	};
}
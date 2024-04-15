#pragma once

namespace sdk {
	class c_player_attack {
	public:
		il2cpp_field_get( c_attack*, attack, _( "ProtoBuf" ), _( "PlayerAttack" ), _( "attack" ) );
		il2cpp_field_set( c_attack*, set_attack, _( "ProtoBuf" ), _( "PlayerAttack" ), _( "attack" ) );

		il2cpp_field_set( int, set_projectile_id, _( "ProtoBuf" ), _( "PlayerAttack" ), _( "projectileID" ) );
	};
}
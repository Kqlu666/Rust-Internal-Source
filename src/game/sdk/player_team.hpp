#pragma once

namespace sdk {
	class c_team_member {
	public:
		inline auto user_id( ) {
			return mem::read< unsigned long >( this + 0x18 );
		}
	};

	class c_player_team {
	public:
		il2cpp_field_get( unity::c_list< c_team_member* >*, members, _( "ProtoBuf" ), _( "PlayerTeam" ), _( "members" ) );
	};
}
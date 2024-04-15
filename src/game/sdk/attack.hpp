#pragma once

namespace sdk {
	class c_attack {
	public:
		il2cpp_field_get( vec3_t, point_end, _( "ProtoBuf" ), _( "Attack" ), _( "pointEnd" ) );

		il2cpp_field_set( unsigned int, set_hit_bone, _( "ProtoBuf" ), _( "Attack" ), _( "hitBone" ) );
		il2cpp_field_set( unsigned int, set_hit_part_id, _( "ProtoBuf" ), _( "Attack" ), _( "hitPartID" ) );
		il2cpp_field_set( vec3_t, set_hit_normal_local, _( "ProtoBuf" ), _( "Attack" ), _( "hitNormalLocal" ) );
		il2cpp_field_set( vec3_t, set_hit_position_local, _( "ProtoBuf" ), _( "Attack" ), _( "hitPositionLocal" ) );
	};
}
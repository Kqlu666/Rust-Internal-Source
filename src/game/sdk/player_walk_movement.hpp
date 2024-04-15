#pragma once

namespace sdk {
	class c_player_walk_movement : public c_base_movement {
	public:
		il2cpp_field_get( bool, flying, _( "" ), _( "PlayerWalkMovement" ), _( "flying" ) );
		il2cpp_field_get( unity::c_rigid_body*, body, _( "" ), _( "PlayerWalkMovement" ), _( "body" ) );
		il2cpp_field_get( unity::c_capsule_collider*, capsule, _( "" ), _( "PlayerWalkMovement" ), _( "capsule" ) );

		il2cpp_field_get( uintptr_t, ladder, _( "" ), _( "PlayerWalkMovement" ), _( "ladder" ) );
		il2cpp_field_get( float, running, _( "" ), _( "BaseMovement" ), _( "<Running>k__BackingField" ) );
		il2cpp_field_get( float, ground_time, _( "" ), _( "PlayerWalkMovement" ), _( "groundTime" ) );

		il2cpp_field_set( uintptr_t, set_ladder, _( "" ), _( "PlayerWalkMovement" ), _( "ladder" ) );
		il2cpp_field_set( bool, set_flying, _( "" ), _( "PlayerWalkMovement" ), _( "flying" ) );
		il2cpp_field_set( bool, set_jumping, _( "" ), _( "PlayerWalkMovement" ), _( "jumping" ) );
		il2cpp_field_set( bool, set_sliding, _( "" ), _( "PlayerWalkMovement" ), _( "sliding" ) );
		il2cpp_field_set( float, set_running, _( "" ), _( "BaseMovement" ), _( "<Running>k__BackingField" ) );
		il2cpp_field_set( bool, set_climbing, _( "" ), _( "PlayerWalkMovement" ), _( "climbing" ) );
		il2cpp_field_set( bool, set_grounded, _( "" ), _( "PlayerWalkMovement" ), _( "grounded" ) );
		il2cpp_field_set( bool, set_was_falling, _( "" ), _( "PlayerWalkMovement" ), _( "wasFalling" ) );
		il2cpp_field_set( float, set_land_time, _( "" ), _( "PlayerWalkMovement" ), _( "landTime" ) );
		il2cpp_field_set( float, set_jump_time, _( "" ), _( "PlayerWalkMovement" ), _( "jumpTime" ) );
		il2cpp_field_set( float, set_ground_time, _( "" ), _( "PlayerWalkMovement" ), _( "groundTime" ) );
		il2cpp_field_set( float, set_ground_angle, _( "" ), _( "PlayerWalkMovement" ), _( "groundAngle" ) );
		il2cpp_field_set( float, set_capsule_height, _( "" ), _( "PlayerWalkMovement" ), _( "capsuleHeight" ) );
		il2cpp_field_set( float, set_ground_angle_new, _( "" ), _( "PlayerWalkMovement" ), _( "groundAngleNew" ) );
		il2cpp_field_set( vec3_t, set_previous_velocity, _( "" ), _( "PlayerWalkMovement" ), _( "previousVelocity" ) );
		il2cpp_field_set( float, set_gravity_multiplier, _( "" ), _( "PlayerWalkMovement" ), _( "gravityMultiplier" ) );
		il2cpp_field_set( float, set_gravity_multiplier_swimming, _( "" ), _( "PlayerWalkMovement" ), _( "gravityMultiplierSwimming" ) );
		il2cpp_field_set( float, set_capsule_center, _( "" ), _( "PlayerWalkMovement" ), _( "capsuleCenter" ) );
		il2cpp_field_set( bool, set_admin_cheat, _( "" ), _( "BaseMovement" ), _( "adminCheat" ) );

		inline void teleport_to( vec3_t pos ) {
			il2cpp_method( ptr, _( "" ), _( "PlayerWalkMovement" ), _( "TeleportTo" ), 2, void( * )( c_base_movement*, vec3_t, c_base_player* ) );
			safe.call< void >( ptr, this, pos, sdk::local_player );
		}
	};
}
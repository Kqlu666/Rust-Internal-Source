#pragma once

namespace sdk {
	inline std::vector< sdk::e_bone > bones = {

		pelvis,
		l_hip,
		l_knee,
		l_foot,
		l_toe,
		l_ankle_scale,
		penis,
		GenitalCensor,
		GenitalCensor_LOD0,
		Inner_LOD0,
		GenitalCensor_LOD1,
		GenitalCensor_LOD2,
		r_hip,
		r_knee,
		r_foot,
		r_toe,
		r_ankle_scale,
		spine1,
		spine1_scale,
		spine2,
		spine3,
		spine4,
		l_clavicle,
		l_upperarm,
		l_forearm,
		l_hand,
		l_index1,
		l_index2,
		l_index3,
		l_little1,
		l_little2,
		l_little3,
		l_middle1,
		l_middle2,
		l_middle3,
		l_prop,
		l_ring1,
		l_ring2,
		l_ring3,
		l_thumb1,
		l_thumb2,
		l_thumb3,
		IKtarget_righthand_min,
		IKtarget_righthand_max,
		l_ulna,
		neck,
		head,
		jaw,
		eyeTranform,
		l_eye,
		l_Eyelid,
		r_eye,
		r_Eyelid,
		r_clavicle,
		r_upperarm,
		r_forearm,
		r_hand,
		r_index1,
		r_index2,
		r_index3,
		r_little1,
		r_little2,
		r_little3,
		r_middle2,
		r_middle3,
		r_prop,
		r_ring1,
		r_ring2,
		r_ring3,
		r_thumb1,
		r_thumb2,
		r_thumb3,
		IKtarget_lefthand_min,
		IKtarget_lefthand_max,
		r_ulna,
		l_breast,
		r_breast,
		BoobCensor,
		BreastCensor_LOD0,
		BreastCensor_LOD1,
		BreastCensor_LOD2
	};

	class c_held_entity;

	class c_base_player : public c_patrol_helicopter {
	public:
		il2cpp_field_get( c_player_eyes*, eyes, _( "" ), _( "BasePlayer" ), _( "eyes" ) );
		il2cpp_field_get( c_player_input*, input, _( "" ), _( "BasePlayer" ), _( "input" ) );
		il2cpp_field_get( c_base_mountable*, mounted, _( "" ), _( "BasePlayer" ), _( "mounted" ) );
		il2cpp_field_get( float, last_sent_tick_time, _( "" ), _( "BasePlayer" ), _( "lastSentTickTime" ) );
		il2cpp_field_get( int, flags, _( "" ), _( "BasePlayer" ), _( "playerFlags" ) );
		il2cpp_field_set( int, set_flags, _( "" ), _( "BasePlayer" ), _( "playerFlags" ) );
		il2cpp_field_get( float, last_headshot_sound_time, _( "" ), _( "BasePlayer" ), _( "lastHeadshotSoundTime" ) );
		il2cpp_field_set( float, set_last_headshot_sound_time, _( "" ), _( "BasePlayer" ), _( "lastHeadshotSoundTime" ) );
		il2cpp_field_get( unsigned int, active_item, _( "" ), _( "BasePlayer" ), _( "clActiveItem" ) );
		il2cpp_field_get( unsigned long, user_id, _( "" ), _( "BasePlayer" ), _( "userID" ) );
		il2cpp_field_get( c_player_model*, model, _( "" ), _( "BasePlayer" ), _( "playerModel" ) );
		il2cpp_field_get( c_model_state*, model_state, _( "" ), _( "BasePlayer" ), _( "modelState" ) );
		il2cpp_field_get( c_player_inventory*, inventory, _( "" ), _( "BasePlayer" ), _( "inventory" ) );
		il2cpp_field_get( c_player_walk_movement*, walk_movement, _( "" ), _( "BasePlayer" ), _( "movement" ) );
		il2cpp_field_get( c_player_blueprints*, blueprints, _( "" ), _( "BasePlayer" ), _( "blueprints" ) );

		il2cpp_field_get( c_player_team*, client_team, _( "" ), _( "BasePlayer" ), _( "clientTeam" ) );

		il2cpp_field_get( unity::c_string*, display_name, _( "" ), _( "BasePlayer" ), _( "_displayName" ) );

		il2cpp_field_get( unity::capsule_collider_t*, collider, _( "" ), _( "BasePlayer" ), _( "playerCollider" ) );
		il2cpp_field_get( c_player_metabolism*, metabolism, _( "" ), _( "BasePlayer" ), _( "metabolism" ) );
		il2cpp_field_get( float, clothing_move_speed_reduction, _( "" ), _( "BasePlayer" ), _( "clothingMoveSpeedReduction" ) );

		il2cpp_field_get( bool, frozen, _( "" ), _( "BasePlayer" ), _( "Frozen" ) );
		il2cpp_field_set( bool, set_frozen, _( "" ), _( "BasePlayer" ), _( "Frozen" ) );

		il2cpp_field_set( float, set_client_tick_interval, _( "" ), _( "BasePlayer" ), _( "clientTickInterval" ) );
		il2cpp_field_set( float, set_clothing_blocks_aiming, _( "" ), _( "BasePlayer" ), _( "clothingBlocksAiming" ) );
		il2cpp_field_set( float, set_clothing_move_speed_reduction, _( "" ), _( "BasePlayer" ), _( "clothingMoveSpeedReduction" ) );

		il2cpp_static_field_get( unity::c_list_dictionary*, visible_player_list, _( "" ), _( "BasePlayer" ), _( "visiblePlayerList" ) );

		inline void remove_flag( int flag ) {
			auto player_flags = flags( );
			player_flags &= ~flag;

			set_flags( player_flags );
		}

		inline void add_flag( int flag ) {
			auto player_flags = flags( );
			set_flags( player_flags |= flag );
		}

		inline bool has_flag( int flag ) {
			auto player_flags = flags( );
			return ( player_flags & flag );
		}

		inline auto last_sent_tick( ) {
			return mem::read< c_player_tick* >( this + 0x8A0 );
		}

		inline auto radius( ) {
			il2cpp_method( ptr, _( "" ), _( "BasePlayer" ), _( "GetRadius" ), -1, float ( * )( c_base_player* ) );
			return safe.call< float >( ptr, this );
		}

		inline auto pivot_point( ) {
			il2cpp_method( ptr, _( "" ), _( "BaseEntity" ), _( "PivotPoint" ), -1, vec3_t ( * )( c_base_player* ) );
			return safe.call< vec3_t >( ptr, this );
		}

		inline auto height( bool ducked ) {
			il2cpp_method( ptr, _( "" ), _( "BasePlayer" ), _( "GetHeight" ), 1, float ( * )( c_base_player*, bool ) );
			return safe.call< float >( ptr, this, ducked );
		}

		inline auto jump_height( ) {
			il2cpp_method( ptr, _( "" ), _( "BasePlayer" ), _( "GetJumpHeight" ), -1, float ( * )( c_base_player* ) );
			return safe.call< float >( ptr, this );
		}

		inline auto is_ducked( ) {
			il2cpp_method( ptr, _( "" ), _( "BasePlayer" ), _( "IsDucked" ), -1, bool ( * )( c_base_player* ) );
			return safe.call< bool >( ptr, this );
		}

		inline auto is_swimming( ) {
			il2cpp_method( ptr, _( "" ), _( "BasePlayer" ), _( "IsSwimming" ), -1, bool ( * )( c_base_player* ) );
			return safe.call< bool >( ptr, this );
		}

		inline auto is_running( ) {
			il2cpp_method( ptr, _( "" ), _( "BasePlayer" ), _( "IsRunning" ), -1, bool ( * )( c_base_player* ) );
			return safe.call< bool >( ptr, this );
		}

		inline auto is_crawling( ) {
			il2cpp_method( ptr, _( "" ), _( "BasePlayer" ), _( "IsCrawling" ), -1, bool ( * )( c_base_player* ) );
			return safe.call< bool >( ptr, this );
		}

		inline auto on_ladder( ) {
			il2cpp_method( ptr, _( "" ), _( "BasePlayer" ), _( "OnLadder" ), -1, bool ( * )( c_base_player* ) );
			return safe.call< bool >( ptr, this );
		}

		inline auto on_land( float f_velocity ) {
			il2cpp_method( ptr, _( "" ), _( "BasePlayer" ), _( "OnLand" ), -1, void( * )( c_base_player*, float ) );
			safe.call< void >( ptr, this, f_velocity );
		}

		inline auto held_entity( ) {
			il2cpp_method( ptr, _( "" ), _( "BasePlayer" ), _( "GetHeldEntity" ), -1, c_held_entity*( * )( c_base_player* ) );
			return safe.call< c_held_entity* >( ptr, this );
		}

		inline auto send_client_tick( ) {
			il2cpp_method( ptr, _( "" ), _( "BasePlayer" ), _( "SendClientTick" ), -1, void ( * )( c_base_player* ) );
			safe.call< void >( ptr, this );
		}

		struct obb_t {
		public:
			// Fields
			quat_t rotation; // 0x0
			vec3_t position; // 0x10
			vec3_t extents; // 0x1C
			vec3_t forward; // 0x28
			vec3_t right; // 0x34
			vec3_t up; // 0x40

			vec3_t closest_point( vec3_t target ) {
				bool flag = false;
				bool flag2 = false;
				bool flag3 = false;

				vec3_t vector = position;
				vec3_t lhs = target - position;

				float num = lhs.dot( right );
				if ( num > extents.x ) {
					vector += right * extents.x;
				} else if ( num < -extents.x ) {
					vector -= right * extents.x;
				} else {
					flag = true;
					vector += right * num;
				}

				float num2 = lhs.dot( up );
				if ( num2 > extents.y ) {
					vector += up * extents.y;
				} else if ( num2 < -extents.y ) {
					vector -= up * extents.y;
				} else {
					flag2 = true;
					vector += up * num2;
				}

				float num3 = lhs.dot( forward );
				if ( num3 > extents.z ) {
					vector += forward * extents.z;
				} else if ( num3 < -extents.z ) {
					vector -= forward * extents.z;
				} else {
					flag3 = true;
					vector += forward * num3;
				}

				if ( flag && flag2 && flag3 )
					return target;

				return vector;
			}

			float distance_to( vec3_t position ) { return position.distance_to( closest_point( position ) ); }
		};

		inline auto bounds( ) {
			il2cpp_method( ptr, _( "" ), _( "BasePlayer" ), _( "GetBounds" ), -1, unity::bounds_t* ( * )( c_base_player* ) );
			return safe.call< unity::bounds_t >( ptr, this );
		}

		inline auto world_space_bounds( ) {
			il2cpp_method( ptr, _( "" ), _( "BasePlayer" ), _( "WorldSpaceBounds" ), -1, void ( * )( c_base_player* ) );
			return safe.call< obb_t >( ptr, this );
		}

		inline bool is_knocked( ) {
			return flags( ) & sdk::e_flag::wounded;
		}

		inline bool is_sleeping( ) {
			return flags( ) & sdk::e_flag::sleeping;
		}

		inline bool is_teammate( ) {
			const auto& local_team = sdk::local_player->client_team( );
			if ( !memory::is_valid( local_team ) )
				return false;

			const auto& members = local_team->members( );
			if ( !memory::is_valid( members ) )
				return false;

			const auto& steam_id = user_id( );

			for ( auto i = 0; i < members->count( ); ++i ) {
				const auto& member = members->value( i );
				if ( !memory::is_valid( member ) )
					continue;

				if ( member->user_id( ) == steam_id )
					return true;
			}

			return false;
		}

		inline float speed( float running, float ducking, float crawling ) {
			il2cpp_method( ptr, _( "" ), _( "BasePlayer" ), _( "GetSpeed" ), 3, float ( * )( c_base_player*, float, float, float ) );
			return safe.call< float >( ptr, this, running, ducking, crawling );
		}

		inline float max_speed( ) {
			il2cpp_method( ptr, _( "" ), _( "BasePlayer" ), _( "GetMaxSpeed" ), -1, void ( * )( c_base_player* ) );
			return safe.call< float >( ptr, this );
		}

		inline float max_velocity( ) {
			il2cpp_method( ptr, _( "" ), _( "BasePlayer" ), _( "MaxVelocity" ), -1, void ( * )( c_base_player* ) );
			return safe.call< float >( ptr, this );
		}

		inline float bounds_padding( ) {
			il2cpp_method( ptr, _( "" ), _( "BasePlayer" ), _( "BoundsPadding" ), 0, void ( * )( c_base_player* ) );
			return safe.call< float >( ptr, this );
		}

		inline vec3_t mount_velocity( ) {
			il2cpp_method( ptr, _( "" ), _( "BasePlayer" ), _( "GetMountVelocity" ), 0, void ( * )( c_base_player* ) );
			return safe.call< vec3_t >( ptr, this );
		}

		inline void on_view_mode_changed( ) {
			il2cpp_method( ptr, _( "" ), _( "BasePlayer" ), _( "OnViewModeChanged" ), -1, void ( * )( c_base_player* ) );
			safe.call< void >( ptr, this );
		}

		inline void send_projectile_update( c_player_projectile_update* player_projectile_update ) {
			il2cpp_method( ptr, _( "" ), _( "BasePlayer" ), _( "SendProjectileUpdate" ), 1, void ( * )( c_base_player*, c_player_projectile_update* ) );
			safe.call< void >( ptr, this, player_projectile_update );
		}

		inline vec3_t closest_point( vec3_t position ) {
			il2cpp_method( ptr, _( "" ), _( "BaseEntity" ), _( "ClosestPoint" ), -1, vec3_t ( * )( c_base_player*, vec3_t ) );
			return safe.call< vec3_t >( ptr, this, position );
		}

		inline bool is_local_player( ) {
			const auto& _model = model( );
			if ( !memory::is_valid( _model ) )
				return false;

			return _model->is_local_player( );
		}

		inline c_item* item( ) {
			const auto& active_uid = active_item( );
			if ( !active_uid )
				return { };

			const auto& inv = inventory( );
			if ( !memory::is_valid( inv ) )
				return { };

			const auto& container_belt = inv->container_belt( );
			if ( !memory::is_valid( container_belt ) )
				return { };

			const auto& item_list = container_belt->item_list( );
			if ( !memory::is_valid( item_list ) )
				return { };

			const auto& count = item_list->count( );

			for ( auto i = 0u; i < count; ++i ) {
				const auto& item = item_list->value( i );
				if ( !memory::is_valid( item ) )
					continue;

				if ( active_uid == item->uid( ) )
					return item;
			}

			return { };
		}

		inline auto set_visible_state( bool state ) {
			info.visible_states[ this ] = state;
		}

		inline auto set_player_states( e_player_state state ) {
			info.player_states[ this ] = state;
		}

		inline auto player_state( ) {
			if ( !info.player_states.contains( this ) )
				return e_player_state::inside;

			return info.player_states[ this ];
		}

		inline bool is_visible( ) {
			if ( !info.visible_states.contains( this ) )
				return false;

			return info.visible_states[ this ];
		}

		inline unity::c_transform* bone_transform( int bone ) {
			const auto& model = entity_model( );
			if ( !memory::is_valid( model ) )
				return { };

			const auto& bone_transforms = model->bone_transforms( );
			if ( !memory::is_valid( bone_transforms ) )
				return { };

			return bone_transforms->get( bone );
		}

		inline void set_bone_positions( ) {
			for ( auto& bone : bones ) {
				auto& player = info.bone_positions[ this ];
				player[ bone ] = bone_transform( bone )->position( );
			}
		}

		inline vec3_t bone_position( sdk::e_bone bone ) {
			if ( !info.bone_positions.contains( this ) )
				return { };

			if ( !info.bone_positions[ this ].contains( bone ) )
				return { };

			return info.bone_positions[ this ][ bone ];
		}
	};

	class c_local_player {
	public:
		static auto entity( ) {
			il2cpp_method( ptr, _( "" ), _( "LocalPlayer" ), _( "get_Entity" ), -1, c_base_player * ( * )( ) );
			return safe.call< c_base_player* >( ptr );
		}
	};
}
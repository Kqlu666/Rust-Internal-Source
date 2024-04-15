#pragma once

#include "../vcruntime/memory/virtual.hpp"

namespace hooks {

    namespace item_icon {
        static void try_to_move( sdk::c_item_icon* instance ) {
            if ( config::settings_panic )
                return g_hooks.call( try_to_move )( instance );

            if ( !memory::is_valid( sdk::local_player ) )
                return g_hooks.call( try_to_move )( instance );

            g_hooks.call( try_to_move )( instance );

            if ( config::exploits_fast_loot && instance->queued_for_looting( ) )
                instance->run_timed_action( );
        }
    }

    namespace base_melee {
        static void process_attack( sdk::c_held_entity* instance, sdk::c_hit_test* hit ) {
            if ( config::settings_panic )
                return g_hooks.call( process_attack )( instance, hit );

            if ( !memory::is_valid( sdk::local_player ) )
                return g_hooks.call( process_attack )( instance, hit );

            if ( !config::misc_farm_assist )
                return g_hooks.call( process_attack )( instance, hit );

            const auto& hit_entity = hit->hit_entity( );
            if ( !memory::is_valid( hit_entity ) )
                return g_hooks.call( process_attack )( instance, hit );

            const auto& hit_entity_transform = hit_entity->transform( );
            if ( !memory::is_valid( hit_entity_transform ) )
                return g_hooks.call( process_attack )( instance, hit );

            const auto& max_dist = config::misc_weapon_extended_melee ? 2.f + config::misc_weapon_extended_melee_radius : 2.f;

            if ( hit_entity->is_class( _( "OreResourceEntity" ) ) ) {
                const auto& target_entity = sdk::c_base_entity::find_closest< sdk::c_base_entity* >( _( "OreHotSpot" ), max_dist );
                if ( memory::is_valid( target_entity ) ) {
                    hit->set_hit_entity( hit_entity );

                    const auto& hot_spot_transform = target_entity->transform( );
                    hit->set_hit_transform( hot_spot_transform );

                    const auto& hotspot_position = hot_spot_transform->position( );
                    const auto& inverse_point = hot_spot_transform->inverse_transform_point( hotspot_position );
                    hit->set_hit_point( inverse_point );
                }
            } else if ( hit_entity->is_class( _( "TreeEntity" ) ) ) {
                const auto& tree_pos = hit_entity_transform->position( );

                const auto& dist = sdk::local_player->closest_point( tree_pos ).distance_to( tree_pos );

                const auto& target_entity = sdk::c_base_entity::find_closest< sdk::c_base_entity* >( _( "TreeMarker" ), max_dist );
                if ( !memory::is_valid( target_entity ) ) {
                    hit->set_hit_distance( dist );

                    const auto& inverse_point = hit_entity_transform->inverse_transform_point( tree_pos + vec3_t( 0.f, 1.f, 0.f ) );
                    hit->set_hit_point( inverse_point );
                } else {
                    const auto& transform = target_entity->transform( );
                    if ( !memory::is_valid( transform ) )
                        return g_hooks.call( process_attack )( instance, hit );

                    const auto& pos3 = transform->position( );
                    const auto& pos3_inverse_point = hit_entity_transform->inverse_transform_point( pos3 );
                    hit->set_hit_distance( dist );
                    hit->set_hit_point( pos3_inverse_point );

                    hit_entity->set_hit_direction( vec3_t( tree_pos.x, 0, tree_pos.z ) - vec3_t( pos3.x, 0, pos3.z ) );
                }
            }

            g_hooks.call( process_attack )( instance, hit );
        }

        static void on_viewmodel_event( sdk::c_held_entity* instance, unity::c_string* name ) {
            if ( config::settings_panic )
                return g_hooks.call( on_viewmodel_event )( instance, name );

            if ( !config::aimbot_melee )
                return g_hooks.call( on_viewmodel_event )( instance, name );

            if ( !memory::is_valid( sdk::local_player ) )
                return g_hooks.call( on_viewmodel_event )( instance, name );

            static const auto& item_mod_projectile_type = il2cpp::type_object( _( "" ), _( "ItemModProjectile" ) );
            static const auto& projectile_type = il2cpp::type_object( _( "" ), _( "Projectile" ) );

            const auto& target = features::aimbot.get_target( );
            if ( !memory::is_valid( target.player ) )
                return g_hooks.call( on_viewmodel_event )( instance, name );

            const auto& eyes = sdk::local_player->eyes( );
            if ( !memory::is_valid( eyes ) )
                return g_hooks.call( on_viewmodel_event )( instance, name );

            const auto& movement = sdk::local_player->walk_movement( );
            if ( !memory::is_valid( movement ) )
                return g_hooks.call( on_viewmodel_event )( instance, name );

            const auto& item = sdk::local_player->item( );
            if ( !memory::is_valid( item ) )
                return g_hooks.call( on_viewmodel_event )( instance, name );

            const auto& info = item->info( );
            if ( !memory::is_valid( info ) )
                return g_hooks.call( on_viewmodel_event )( instance, name );

            const auto& item_mod_projectile = info->component< sdk::c_item_mod_projectile* >( item_mod_projectile_type );
            if ( !memory::is_valid( item_mod_projectile ) )
                return g_hooks.call( on_viewmodel_event )( instance, name );

            const auto& projectile_component = sdk::c_game_manifest::guid_to_object( item_mod_projectile->projectile_object( )->guid( ) )->component< sdk::c_projectile* >( projectile_type );
            if ( !memory::is_valid( projectile_component ) )
                return g_hooks.call( on_viewmodel_event )( instance, name );

            if ( features::bullet_tp_angle.empty( ) )
                features::bullet_tp_angle = target.pos;

            const auto& spread = item_mod_projectile->projectile_spread( );
            const auto& old_spread = item_mod_projectile->projectile_velocity_spread( );

            const auto& bullet_speed = item_mod_projectile->projectile_velocity( );

            float new_spread = bullet_speed + old_spread;
            new_spread *= config::aimbot_instant_bullet ? 1.495f : 1.f;

            quat_t old_rotation;
            float old_running = 0.f;
            bool eye_changed = false;

            const auto& passes_chance = !config::aimbot_hit_chance || ( ( rand( ) % ( 100 - 1 + 1 ) ) + 1 ) <= config::aimbot_hit_chance_value;

            const auto& is_throw = HASH_EQUAL( RHASH( name->string( ).c_str( ) ), _( "Throw" ) );
            if ( is_throw ) {
                old_running = movement->running( );
                movement->set_running( 0.f );

                if ( passes_chance && ( config::aimbot_key == 0 || input_handler.is_hold( config::aimbot_key ) ) ) {
                    old_rotation = eyes->rotation( );

                    const auto& data = features::aimbot.prediction( eyes->position( ), target.pos, target.velocity );
                    if ( data.possible ) {
                        eyes->set_rotation( unity::c_quaternion::look_rotation( data.out_velocity ) );
                        eye_changed = true;
                    }
                }
            }

            item_mod_projectile->set_projectile_velocity_spread( 0.f );
            item_mod_projectile->set_projectile_spread( 0.f );
            item_mod_projectile->set_projectile_velocity( new_spread );

            g_hooks.call( on_viewmodel_event )( instance, name );

            if ( is_throw ) {
                movement->set_running( old_running );

                if ( eye_changed )
                    eyes->set_rotation( old_rotation );
            }

            item_mod_projectile->set_projectile_velocity_spread( old_spread );
            item_mod_projectile->set_projectile_spread( spread );
            item_mod_projectile->set_projectile_velocity( bullet_speed );
        }
    }

    namespace projectile_shoot {
        static void write_to_stream( sdk::c_projectile_shoot* __this, void* stream ) {
            if ( config::settings_panic )
                return g_hooks.call( write_to_stream )( __this, stream );

            if ( !memory::is_valid( sdk::local_player ) )
                return g_hooks.call( write_to_stream )( __this, stream );

            features::fixed_time_last_shot = unity::c_time::fixed_time( );
            features::just_shot = true;
            features::did_reload = false;

            const auto& item = sdk::local_player->item( );
            if ( !memory::is_valid( item ) )
                return g_hooks.call( write_to_stream )( __this, stream );

            if ( !item->is_weapon( ) )
                return g_hooks.call( write_to_stream )( __this, stream );

            const auto& held_entity = item->held_entity( );
            if ( !memory::is_valid( held_entity ) )
                return g_hooks.call( write_to_stream )( __this, stream );

            const auto& primary_magazine = held_entity->primary_magazine( );
            if ( !memory::is_valid( primary_magazine ) )
                return g_hooks.call( write_to_stream )( __this, stream );

            const auto& ammo_type = primary_magazine->ammo_type( );
            if ( !memory::is_valid( ammo_type ) )
                return g_hooks.call( write_to_stream )( __this, stream );

            const auto& projectiles = __this->projectiles( );
            if ( !memory::is_valid( projectiles ) )
                return g_hooks.call( write_to_stream )( __this, stream );

            const auto& eye_position = sdk::local_player->eyes( )->position( );
            if ( eye_position.empty( ) )
                return g_hooks.call( write_to_stream )( __this, stream );

            const auto& target = features::aimbot.get_target( );

            if ( features::bullet_tp_angle.empty( ) && memory::is_valid( target.player ) )
                features::bullet_tp_angle = target.pos;
            
            vec3_t start_position;
            vec3_t original_velocity;

            vec3_t aim_angles;

            bool visible = false;
            bool manipulated = false;

            for ( auto i = 0; i < projectiles->count( ); ++i ) {
                const auto& projectile = projectiles->value( i );
                if ( !memory::is_valid( projectile ) )
                    continue;

                start_position = projectile->start_position( );
                if ( start_position.empty( ) )
                    continue;

                original_velocity = projectile->start_velocity( );
                if ( original_velocity.empty( ) )
                    continue;

                if ( config::aimbot_manipulation && input_handler.is_hold( config::aimbot_manipulation_key ) ) {
                    vec3_t eye_position = sdk::local_player->eyes( )->position( );

                    auto old_position = start_position;
                    if ( !features::manipulation_angle.empty( ) ) {
                        start_position += ( features::cached_manip_point - eye_position );

                        visible = sdk::c_game_physics::is_visible( features::cached_manip_point, features::cached_bullet_tp_position, 0.2f );
                        if ( start_position != old_position ) {
                            manipulated = true;

                            if ( visible ) {
                                visible = true;

                                projectile->set_start_position( start_position );
                            }
                        }
                    }
                }
            }

            const auto& item_mod_projectile = ammo_type->component< sdk::c_item_mod_projectile* >( il2cpp::type_object( _( "" ), _( "ItemModProjectile" ) ) );
            if ( !memory::is_valid( item_mod_projectile ) )
                return;

            //item_mod_projectile->set_projectile_spread( config::aimbot_no_spread_value / 100.f );
            //item_mod_projectile->set_projectile_velocity_spread( config::aimbot_no_spread_value / 100.f );

            const auto& created_projectiles = held_entity->created_projectiles( );
            if ( !memory::is_valid( created_projectiles ) )
                return g_hooks.call( write_to_stream )( __this, stream );

            for ( auto i = 0; i < created_projectiles->count( ); ++i ) {
                const auto& projectile = created_projectiles->value( i );
                if ( !memory::is_valid( projectile ) )
                    continue;

                if ( projectile->projectile_id( ) == 0 )
                    continue;

                if ( !memory::is_valid( target.player ) )
                    continue;

                vec3_t aim_angle = features::aimbot.aim_direction_to_target( features::bullet_tp_angle, target.velocity, item_mod_projectile, start_position ) - start_position;
                aim_angles = ( aim_angle ).normalized( ) * original_velocity.length( );
            }

            int hit_chance = ( rand( ) % ( 100 - 1 + 1 ) ) + 1;

            bool can_hit = !config::aimbot_hit_chance || hit_chance <= config::aimbot_hit_chance_value;

            for ( auto i = 0; i < projectiles->count( ); ++i ) {
                const auto& projectile = projectiles->value( i );
                if ( !memory::is_valid( projectile ) )
                    continue;

                if ( config::aimbot && can_hit ) {
                    if ( config::aimbot && ( config::aimbot_key == 0 || input_handler.is_hold( config::aimbot_key ) ) || input_handler.is_hold( config::aimbot_autoshoot_key ) ) {
                        if ( memory::is_valid( target.player ) || manipulated )
                            projectile->set_start_velocity( aim_angles );
                    }
                }
            }

            float delta_time = 0.03125f;

            for ( auto i = 0; i < created_projectiles->count( ); ++i ) {
                const auto& projectile = created_projectiles->value( i );
                if ( !memory::is_valid( projectile ) )
                    continue;

                if ( projectile->projectile_id( ) == 0 )
                    continue;

                if ( config::aimbot_thickness ) {
                    projectile->set_thickness( config::aimbot_thickness_value );
                } else {
                    projectile->set_thickness( 0.1f );
                }

                if ( config::aimbot_instant_bullet )
                    projectile->set_initial_distance( features::initial_distance_debug );

                if ( config::aimbot_big_bullet )
                    projectile->transform( )->set_local_scale( { 20.f, 20.f, 20.f } );

                if ( config::aimbot_fake_hit_distance )
                    projectile->set_traveled_distance( config::aimbot_hit_distance );

                if ( config::aimbot && can_hit ) {
                    if ( ( config::aimbot_key == 0 || input_handler.is_hold( config::aimbot_key ) ) || input_handler.is_hold( config::aimbot_autoshoot_key ) ) {
                        if ( memory::is_valid( target.player ) || manipulated ) {
                            projectile->set_initial_velocity( aim_angles );
                            projectile->set_current_velocity( aim_angles );
                        }
                    }
                }

                if ( config::aimbot_manipulation && input_handler.is_hold( config::aimbot_manipulation_key ) ) {
                    visible = sdk::c_game_physics::is_visible( features::cached_manip_point, features::cached_bullet_tp_position, 0.2f );
                    if ( visible ) {
                        if ( manipulated ) {
                            const auto& game_object = projectile->game_object( );
                            if ( !memory::is_valid( game_object ) )
                                continue;

                            const auto& transform = game_object->transform( );
                            if ( !memory::is_valid( transform ) )
                                continue;

                            transform->set_position( start_position );
                        }
                    }
                }
            }

            g_hooks.call( write_to_stream )( __this, stream );
        }
    }

    namespace player_projectile_attack {
        static void write_to_stream( sdk::c_player_projectile_attack* __this, void* stream ) {
            if ( config::settings_panic )
                return g_hooks.call( write_to_stream )( __this, stream );

            if ( !memory::is_valid( sdk::local_player ) )
                return g_hooks.call( write_to_stream )( __this, stream );

            const auto& player_attack = __this->player_attack( );
            if ( !memory::is_valid( player_attack ) )
                return g_hooks.call( write_to_stream )( __this, stream );

            const auto& attack = player_attack->attack( );
            if ( !memory::is_valid( attack ) )
                return g_hooks.call( write_to_stream )( __this, stream );

            if ( config::aimbot_hitbox_override ) {
                const auto& target = features::aimbot.get_target( );
                if ( memory::is_valid( target.player ) ) {
                    if ( !target.is_heli ) {
                        switch ( config::aimbot_hitbox_override_value ) {
                        case 0: // Head
                            attack->set_hit_bone( 698017942 );
                            attack->set_hit_part_id( 2173623152 );
                            break;
                        case 1: // pelvis
                            attack->set_hit_part_id( 1031402764 );
                            attack->set_hit_part_id( 1750816991 );
                            break;
                        case 2: // Random hitbox
                            switch ( rand( ) % 9 ) {
                            case 0: // Head
                                attack->set_hit_bone( 698017942 );
                                attack->set_hit_part_id( 2173623152 );
                                break;
                            case 1: // pelvis
                                attack->set_hit_bone( 2811218643 );
                                attack->set_hit_part_id( 1750816991 );
                                break;
                            case 2: // r_hip
                                attack->set_hit_bone( 2331610670 );
                                attack->set_hit_part_id( 1750816991 );
                                break;
                            case 3: // r_foot
                                attack->set_hit_bone( 920055401 );
                                attack->set_hit_part_id( 1750816991 );
                                break;
                            case 4: // spine1
                                attack->set_hit_bone( 3771021956 );
                                attack->set_hit_part_id( 1750816991 );
                                break;
                            case 5: // l_hand
                                attack->set_hit_bone( 736328754 );
                                attack->set_hit_part_id( 1750816991 );
                                break;
                            case 6: // r_upperarm
                                attack->set_hit_bone( 3901657145 );
                                attack->set_hit_part_id( 1750816991 );
                                break;
                            case 7: // l_knee
                                attack->set_hit_bone( 3892428003 );
                                attack->set_hit_part_id( 1750816991 );
                                break;
                            case 8: // spine4
                                attack->set_hit_bone( 827230707 );
                                attack->set_hit_part_id( 1750816991 );
                                break;
                            }
                            break;
                        }

                        attack->set_hit_position_local( { -.1f, -.1f, 0 } );
                        attack->set_hit_normal_local( { 0, -1, 0 } );
                    } else {
                        const auto& weakspots = target.player->weakspots( );
                        if ( memory::is_valid( weakspots ) ) {
                            bool tail_alive = false;
                            bool main_alive = false;

                            for ( int i = 0; i < weakspots->count( ); ++i ) {
                                const auto& weakspot = weakspots->get( i );
                                if ( !memory::is_valid( weakspot ) )
                                    continue;

                                auto health = weakspot->health( );
                                if ( health > 0 ) {
                                    if ( i == 0 ) {
                                        main_alive = true;
                                    } else {
                                        tail_alive = true;
                                    }
                                }
                            }

                            attack->set_hit_bone( main_alive ? 224139191 : 2699525250 );
                            attack->set_hit_part_id( 2306822461 );
                            attack->set_hit_position_local( { .9f, -.4f, .1f } );
                            attack->set_hit_normal_local( { .9f, -.4f, .1f } );
                        }
                    }
                }
            }

            return g_hooks.call( write_to_stream )( __this, stream );
        }
    }

    namespace base_player {
        static void on_attacked( sdk::c_base_player* player, sdk::c_hit_info* hit_info ) {
            if ( config::settings_panic )
                return g_hooks.call( on_attacked )( player, hit_info );

            if ( !memory::is_valid( sdk::local_player ) )
                return g_hooks.call( on_attacked )( player, hit_info );

            if ( config::aimbot_hit_effects && hit_info->did_hit( ) && hit_info->initiator( ) == sdk::local_player ) {
                const auto& target = features::aimbot.get_target( );
                if ( memory::is_valid( target.player ) ) {
                    sdk::bundles::ghost_hit_effect_prefab->set_active( true );
                    unity::c_object::instantiate( sdk::bundles::ghost_hit_effect_prefab, target.pos, target.player->eyes( )->rotation( ) );
                } else {
                    sdk::bundles::ghost_hit_effect_prefab->set_active( false );
                }
            }

            g_hooks.call( on_attacked )( player, hit_info );
        }

        static void client_input( sdk::c_base_player* player, sdk::c_input_state* state ) {
            if ( config::settings_panic )
                return g_hooks.call( client_input )( player, state );

            if ( !memory::is_valid( player ) )
                return g_hooks.call( client_input )( player, state );

            sdk::camera = sdk::c_main_camera::main_camera( );
            if ( !memory::is_valid( sdk::camera ) ) {
                sdk::clear( );
                return g_hooks.call( client_input )( player, state );
            }

            if ( !memory::is_valid( sdk::bundles::flat_bundle ) )
                sdk::bundles::initialize( );

            if ( config::visuals_players_chams )
                sdk::convar::c_culling::set_entity_min_cull_dist( config::visuals_players_distance );

            if ( !memory::is_valid( sdk::local_player ) )
                return g_hooks.call( client_input )( player, state );

            sdk::local_position = sdk::local_player->model( )->position( );

            features::misc.loop( );

            if ( config::visuals_players ) {
                for ( auto&& player : sdk::info.players ) {
                    if ( player->is_local_player( ) || player->lifestate( ) )
                        continue;

                    player->set_bone_positions( );

                    const auto& head_position = player->bone_position( sdk::e_bone::head );

                    unity::raycast_hit_t hit_info;
                    if ( sdk::c_game_physics::trace( unity::ray_t( head_position, head_position + vec3_t( 0.f, 500.f, 0.f ) ), 0.f, hit_info, 500.f, 2097152, unity::e_query_trigger_interaction::ignore, nullptr ) ) {
                        player->set_player_states( sdk::e_player_state::inside );
                    } else {
                        player->set_player_states( sdk::e_player_state::outside );
                    }

                    if ( config::visuals_players_visible_check ) {
                        const auto& transform = sdk::camera->transform( );
                        if ( memory::is_valid( transform ) )
                            player->set_visible_state( sdk::c_game_physics::is_visible( transform->position( ), player->bone_position( sdk::e_bone::head ) ) );
                    }
                }
            }

            return g_hooks.call( client_input )( player, state );
        }

        static void block_sprint( sdk::c_base_player* player, float duration ) {
            if ( config::misc_modify_can_attack && !config::settings_panic )
                return;

            return g_hooks.call( block_sprint )( player, duration );
        }
    }

    namespace player_walk_movement {
        static void client_input( sdk::c_player_walk_movement* walk_movement, sdk::c_input_state* state, sdk::c_model_state* model_state ) { 
            if ( config::settings_panic )
                return g_hooks.call( client_input )( walk_movement, state, model_state );

            if ( !memory::is_valid( sdk::local_player ) )
                return g_hooks.call( client_input )( walk_movement, state, model_state );

            g_hooks.call( client_input )( walk_movement, state, model_state );

            if ( config::exploits_admin_flag )
                model_state->remove_flag( sdk::e_model_state_flags::in_fly );

            if ( config::misc_climb_assist ) {
                walk_movement->set_ground_angle( 0.f );
                walk_movement->set_ground_angle_new( 0.f );

                const auto& capsule = walk_movement->capsule( );
                if ( memory::is_valid( capsule ) )
                    capsule->set_radius( 0.44f );
            }


            if ( config::exploits_walk_on_water && sdk::c_water_level::test( sdk::local_player->model( )->position( ), true, false, sdk::local_player ) ) {
                walk_movement->set_gravity_multiplier( 0.f );
                walk_movement->set_ground_angle( 0.f );
                walk_movement->set_ground_angle_new( 0.f );
            }

            if ( config::exploits_spinbot ) {
                vec3_t remain{ };
                remain.x = 100;
                remain.y = rand( ) % 999 + -999;
                remain.z = 100;

                const auto& current = state->current( );
                if ( memory::is_valid( current ) )
                    current->set_aim_angles( remain );
            }

            if ( !walk_movement->flying( ) && config::exploits_omni_sprint ) {
                vec3_t vel = walk_movement->target_movement( );
                model_state->set_sprinting( true );

                float max_speed = ( sdk::local_player->is_swimming( ) || sdk::local_player->is_ducked( ) ) ? 1.7f : 5.5f;
                if ( vel.length( ) > 0.f ) {
                    vec3_t target_vel = vec3_t( vel.x / vel.length( ) * max_speed, vel.y, vel.z / vel.length( ) * max_speed );
                    walk_movement->set_target_movement( target_vel );
                }
            }
        }
    }

    namespace env_sync {
        class c_load_info {
        public:
            inline auto msg( ) {
                return mem::read< sdk::c_entity* >( this + 0x0 );
            }
        };

        static void load( void* instance, c_load_info* load_info ) {
            const auto& msg = load_info->msg( );
            if ( !memory::is_valid( msg ) )
                return g_hooks.call( load )( instance, load_info );

            const auto& env = msg->environment( );
            if ( !memory::is_valid( env ) )
                return g_hooks.call( load )( instance, load_info );

            auto date_time = env->date_time( );
            date_time &= 4611686018427387903UL;

            features::current_hour = ( date_time / 36000000000L ) % 24;
            features::current_minute = ( date_time / 600000000L ) % 60;

            g_hooks.call( load )( instance, load_info );
        }
    }

    namespace steam_platforom {
        static void update( sdk::facepunch::c_steam_platform* instance ) {
            g_hooks.call( update )( instance );

            input_handler.scroll_delta = unity::c_input::mouse_scroll_delta( ) / 4;

            if ( config::exploits_name_spoofer )
                instance->set_username( sdk::facepunch::c_random_usernames::get( ( rand( ) % 100 ) + 1 ) );
        }
    }

    //namespace projectile {
    //    static void update( sdk::c_projectile* projectile ) {
    //        if ( !memory::is_valid( sdk::local_player ) )
    //            return g_hooks.call( update )( projectile );

    //        if ( sdk::local_player->is_dead( ) || sdk::local_player->is_sleeping( ) )
    //            return g_hooks.call( update )( projectile );

    //        if ( config::aimbot_thickness ) {
    //            projectile->set_thickness( config::aimbot_thickness_value );
    //        } else
    //            projectile->set_thickness( 0.1f );

    //        float delta_time = 0.03125f;

    //        const auto& target = features::aimbot.get_target( );
    //        if ( !memory::is_valid( target.player ) )
    //            return g_hooks.call( update )( projectile );

    //        const auto& owner = projectile->owner( );
    //        if ( !memory::is_valid( owner ) )
    //            return;

    //        if ( owner->user_id( ) != sdk::local_player->user_id( ) )
    //            return;

    //        if ( projectile->projectile_id( ) == 0 || !memory::is_valid( owner ) )
    //            return g_hooks.call( update )( projectile );

    //        auto original_distance = projectile->initial_distance( );

    //        if ( config::aimbot_instant_bullet )
    //            projectile->set_initial_distance( features::initial_distance_debug );

    //        const auto& transform = projectile->transform( );

    //        while ( projectile->integrity( ) > 0.001f &&
    //             projectile->max_distance( ) > projectile->traveled_distance( ) &&
    //             projectile->traveled_time( ) < 8.f &&
    //             projectile->launch_time( ) > 0.f &&
    //             ( unity::c_time::time( ) - projectile->launch_time( ) ) >= ( projectile->traveled_time( ) + delta_time ) ) {

    //            vec3_t previous_position = projectile->previous_position( );
    //            if ( projectile->traveled_time( ) != 0.f )
    //                projectile->set_previous_position( projectile->current_position( ) );

    //            projectile->set_current_position( transform->position( ) );
    //            if ( projectile->traveled_time( ) == 0.f ) {
    //                projectile->set_sent_position( projectile->previous_position( ) );
    //                projectile->set_previous_position( projectile->current_position( ) );
    //            }

    //            delta_time *= unity::c_time::timescale( );

    //            if ( features::misc.do_movement( delta_time, projectile ) )
    //                features::misc.do_velocity_update( delta_time, projectile );

    //            transform->set_position( projectile->current_position( ) );
    //            transform->set_rotation( unity::c_quaternion::look_rotation( projectile->current_velocity( ) ) );
    //        }

    //        if ( projectile->integrity( ) <= 0.01f || projectile->max_distance( ) <= projectile->traveled_distance( ) || projectile->traveled_time( ) > 8.f )
    //            projectile->retire( );
    //        else {
    //            projectile->set_effect_scale( projectile->effect_scale( ) );
    //            projectile->do_flyby_sound( );
    //        }

    //        projectile->set_initial_distance( original_distance );
    //    }
    //}

    namespace client {
        enum e_message_type : byte {
            first,
            welcome,
            auth,
            approved,
            ready,
            entities,
            entity_destroy,
            group_change,
            group_destroy,
            rpc_message,
            entity_position,
            console_message,
            console_command,
            effect,
            disconnect_reason,
            tick,
            message,
            request_user_information,
            give_user_information,
            group_enter,
            group_leave,
            voice_data,
            eac,
            entity_flags,
            world,
            console_replicated_vars,
            last = 25
        };

        class c_effect {
        public:
            il2cpp_field_get( vec3_t, origin, _( "" ), _( "EffectData" ), _( "origin" ) );
            il2cpp_field_get( vec3_t, world_pos, _( "" ), _( "Effect" ), _( "worldPos" ) );

            il2cpp_field_get( uint32_t, pooledstringid, _( "" ), _( "EffectData" ), _( "pooledstringid" ) );
        };

        class c_message {
        public:
            il2cpp_field_get( e_message_type, type, _( "Network" ), _( "Message" ), _( "type" ) );
        };

        static void on_network_update( void* client, c_message* packet ) {
            g_hooks.call( on_network_update )( client, packet );

            if ( packet->type( ) == e_message_type::effect ) {
                const auto& effect_network = il2cpp::get_class( _( "" ), _( "EffectNetwork" ) );
                if ( !memory::is_valid( effect_network ) )
                    return;

                const auto& effect = mem::read_chain< c_effect* >( ( void* )effect_network, { 0xB8, 0x0 } );
                if ( !memory::is_valid( effect ) )
                    return;

                auto pos = effect->world_pos( );
                if ( pos.empty( ) )
                    pos = effect->origin( );

                switch ( effect->pooledstringid( ) ) {
                    case 1798302402:
                        TRACE( _( "Explosive Ammo: %.f, %.f, %.f" ), pos.x, pos.y, pos.z );
                        break;
                    case 857997843:
                        TRACE( _( "C4: %.f, %.f, %.f" ), pos.x, pos.y, pos.z );
                        break;
                    case 1289728008:
                        TRACE( _( "Satchel: %.f, %.f, %.f" ), pos.x, pos.y, pos.z );
                        break;
                    case -1241151013:
                        TRACE( _( "Rocket: %.f, %.f, %.f" ), pos.x, pos.y, pos.z );
                        break;
                }
            }
        }
    }

    static void initialize( ) {
        g_hooks.add( il2cpp::get_virtual_method( _( "" ), _( "EnvSync" ), _( "Load" ) ), env_sync::load );
        // g_hooks.add( il2cpp::get_method( il2cpp::get_class( _( "" ), _( "Projectile" ) ), _( "Update" ) ), projectile::update );
        g_hooks.add( il2cpp::get_virtual_method( _( "" ), _( "ItemIcon" ), _( "TryToMove" ) ), item_icon::try_to_move );
        g_hooks.add( il2cpp::get_virtual_method( _( "" ), _( "BasePlayer" ), _( "OnAttacked" ) ), base_player::on_attacked );
        g_hooks.add( il2cpp::get_virtual_method( _( "" ), _( "BasePlayer" ), _( "ClientInput" ) ), base_player::client_input );
        g_hooks.add( il2cpp::get_virtual_method( _( "" ), _( "BasePlayer" ), _( "BlockSprint" ) ), base_player::block_sprint );
       //g_hooks.add( il2cpp::get_virtual_method( _( "" ), _( "Client" ), _( "OnNetworkMessage" ) ), client::on_network_update );
        g_hooks.add( il2cpp::get_virtual_method( _( "" ), _( "BaseMelee" ), _( "ProcessAttack" ) ), base_melee::process_attack );
        g_hooks.add( il2cpp::get_virtual_method( _( "" ), _( "BaseMelee" ), _( "OnViewmodelEvent" ) ), base_melee::on_viewmodel_event );
        g_hooks.add( il2cpp::get_virtual_method( _( "Rust.Platform.Steam" ), _( "SteamPlatform" ), _( "Update" ) ), steam_platforom::update );
        g_hooks.add( il2cpp::get_virtual_method( _( "" ), _( "PlayerWalkMovement" ), _( "ClientInput" ) ), player_walk_movement::client_input );
        g_hooks.add( il2cpp::get_virtual_method( _( "ProtoBuf" ), _( "ProjectileShoot" ), _( "WriteToStream" ) ), projectile_shoot::write_to_stream );
        g_hooks.add( il2cpp::get_virtual_method( _( "ProtoBuf" ), _( "PlayerProjectileAttack" ), _( "WriteToStream" ) ), player_projectile_attack::write_to_stream );
    }
}
#pragma once
#include "rconfig.hpp"

namespace config {
    class i_config_item {
    public:
        virtual void read( ) = 0;
        virtual void save( ) = 0;

        std::string name;
        virtual void* get_val_ptr( ) = 0;
    };

    namespace manager {
        static std::vector<i_config_item*> config_items{};
        static rconfig config;

        static int cfg_index = 0;

        void read_config( ) {
            config.read( );
        }

        void write_config( ) {
            config.write( );
        }

        void load( ) {
            read_config( );

            for ( auto&& item : config_items )
                item->read( );
        }

        void save( ) {
            for ( auto&& item : config_items )
                item->save( );

            write_config( );
        }

        void autosave( ) {
            for ( auto&& item : config_items )
                item->save( );

            if ( config.is_stale( ) ) {
                write_config( );
                TRACE( "config autosave successful" );
            }
        }

        __declspec( noinline ) void init( ) {
            char user_buff[ 64 ] = { };
            DWORD user_sz = sizeof( user_buff );
            proc_call( GetUserNameA )( user_buff, &user_sz );

            char computer_buff[ 64 ] = { };
            DWORD computer_sz = sizeof( computer_buff );
            proc_call( GetComputerNameA )( computer_buff, &computer_sz );

            auto pc_hash = RHASH( computer_buff );
            auto user_hash = RHASH( user_buff );

            auto crypt_key = 0x24252;

            std::string path;
            std::string input_path = util::game_path( );

            switch ( cfg_index ) {
            case 0:
                path = util::format( _( "%s\\warrion\\configs\\legit_%x%x" ), input_path.c_str( ), pc_hash + crypt_key, user_hash + crypt_key );
                break;
            case 1:
                path = util::format( _( "%s\\warrion\\configs\\semi_rage_%x%x" ), input_path.c_str( ), pc_hash + crypt_key, user_hash + crypt_key );
                break;
            case 2:
                path = util::format( _( "%s\\warrion\\configs\\rage_%x%x" ), input_path.c_str( ), pc_hash + crypt_key, user_hash + crypt_key );
                break;
            }

            config = rconfig( path );

            load( );
        }
    }

    template <class T>
    class config_item : i_config_item {
    public:
        T value;

        config_item( std::string _name, T _value ) {
            value = _value;
            name = std::to_string( RHASH( _name.c_str( ) ) );

            manager::config_items.push_back( this );
            this->read( );
        };

        operator T( ) {
            return value;
        }

        operator T* ( ) {
            return &value;
        }

        virtual void read( ) {
            value = manager::config.get( name, value );
        }

        virtual void save( ) {
            manager::config.set( name, value );
        }

        virtual void* get_val_ptr( ) {
            return &this->value;
        }
    };

#define CFG_ITEM(type, name, value) static config_item<type> name = config_item<type>(_(#name), value)

    // aimbot tab
    CFG_ITEM( bool, aimbot, false );
    CFG_ITEM( int, aimbot_key, 0 );
    CFG_ITEM( int, aimbot_bone, 0 );
    CFG_ITEM( bool, aimbot_melee, false );
    CFG_ITEM( bool, aimbot_no_sway, false );
    CFG_ITEM( bool, aimbot_friend_list, false );
    CFG_ITEM( bool, aimbot_ignore_team, false );
    CFG_ITEM( bool, aimbot_silent_melee, false );
    CFG_ITEM( bool, aimbot_visible_check, false );

    CFG_ITEM( bool, aimbot_autoshoot, false );
    CFG_ITEM( int, aimbot_autoshoot_key, 0 );

    CFG_ITEM( bool, aimbot_bullet_tp, false );
    CFG_ITEM( int, aimbot_bullet_tp_mode, 0 );
    CFG_ITEM( int, aimbot_bullet_tp_key, 0 );

    CFG_ITEM( bool, aimbot_manipulation, false );
    CFG_ITEM( int, aimbot_manipulation_key, 0 );
    CFG_ITEM( int, aimbot_manipulation_mode, 0 );
    CFG_ITEM( float, aimbot_manipulation_points, 5.f );

    CFG_ITEM( bool, aimbot_no_spread, false );
    CFG_ITEM( float, aimbot_no_spread_value, 100.f );

    CFG_ITEM( bool, aimbot_no_recoil, false );
    CFG_ITEM( float, aimbot_no_recoil_value, 100.f );

    CFG_ITEM( bool, aimbot_big_bullet, false );
    CFG_ITEM( bool, aimbot_instant_bullet, false );

    CFG_ITEM( bool, aimbot_hitbox_override, false );
    CFG_ITEM( int, aimbot_hitbox_override_value, 0 );

    CFG_ITEM( bool, aimbot_fake_hit_distance, false );
    CFG_ITEM( float, aimbot_hit_distance, 0.f );

    CFG_ITEM( bool, aimbot_thickness, false );
    CFG_ITEM( float, aimbot_thickness_value, 0.f );

    CFG_ITEM( bool, aimbot_hit_chance, false );
    CFG_ITEM( int, aimbot_hit_chance_value, 100 );

    CFG_ITEM( bool, aimbot_fov, false );
    CFG_ITEM( float, aimbot_fov_value, 180.f );
    CFG_ITEM( c_color, aimbot_fov_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( bool, aimbot_crosshair, false );
    CFG_ITEM( c_color, aimbot_crosshair_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( bool, aimbot_hit_effects, false );

    // esp tab
    CFG_ITEM( bool, visuals_players, true );

    CFG_ITEM( bool, visuals_players_npc, true );
    CFG_ITEM( c_color, visuals_players_npc_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( bool, visuals_players_chams, false );
    CFG_ITEM( int, visuals_players_chams_value, 0 );

    CFG_ITEM( bool, visuals_players_teammate, true );
    CFG_ITEM( c_color, visuals_players_teammate_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( bool, visuals_players_sleepers, false );
    CFG_ITEM( c_color, visuals_players_sleepers_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( bool, visuals_players_visible_check, false );
    CFG_ITEM( c_color, visuals_players_visible_check_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( bool, visuals_players_box, true );
    CFG_ITEM( c_color, visuals_players_box_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( bool, visuals_players_name, true );
    CFG_ITEM( c_color, visuals_players_name_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( bool, visuals_players_hotbar, true );

    CFG_ITEM( bool, visuals_players_weapon, true );
    CFG_ITEM( c_color, visuals_players_weapon_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( bool, visuals_players_skeleton, true );
    CFG_ITEM( c_color, visuals_players_skeleton_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( bool, visuals_players_distance, true );
    CFG_ITEM( c_color, visuals_players_distance_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( float, visuals_players_max_distance, 400.f );
    CFG_ITEM( float, visuals_players_npc_max_distance, 400.f );

    //visuals sky
    CFG_ITEM( bool, visuals_sky_bright_cave, false );
    CFG_ITEM( bool, visuals_sky_bright_night, false );

    //visuals time
    CFG_ITEM( bool, visuals_time_change_time, false );
    CFG_ITEM( int, visuals_time_change_time_value, 12 );

    CFG_ITEM( bool, visuals_time_show_server_time, false );

    //visuals weather
    CFG_ITEM( bool, visuals_weather_no_sun, false );
    CFG_ITEM( bool, visuals_weather_no_fog, false );
    CFG_ITEM( bool, visuals_weather_no_rain, false );
    CFG_ITEM( bool, visuals_weather_no_wind, false );
    CFG_ITEM( bool, visuals_weather_rainbows, false );
    CFG_ITEM( bool, visuals_weather_no_clouds, false );
    CFG_ITEM( bool, visuals_weather_no_thunder, false );
    CFG_ITEM( bool, visuals_weather_atmosphere, false );

    // world esp tab

    // ore tab
    CFG_ITEM( bool, visuals_entities_ore, false );

    CFG_ITEM( bool, visuals_entities_ore_stone, true );
    CFG_ITEM( c_color, visuals_entities_ore_stone_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( bool, visuals_entities_ore_metal, true );
    CFG_ITEM( c_color, visuals_entities_ore_metal_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( bool, visuals_entities_ore_sulfur, true );
    CFG_ITEM( c_color, visuals_entities_ore_sulfur_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( float, visuals_entities_ore_max_distance, 400.f );

    // loot tab
    CFG_ITEM( bool, visuals_entities_loot, false );

    CFG_ITEM( bool, visuals_entities_loot_corpse, true );
    CFG_ITEM( c_color, visuals_entities_loot_corpse_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( bool, visuals_entities_loot_backpack, true );
    CFG_ITEM( c_color, visuals_entities_loot_backpack_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( bool, visuals_entities_loot_small_stash, true );
    CFG_ITEM( c_color, visuals_entities_loot_small_stash_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( bool, visuals_entities_loot_supply_drop, true );
    CFG_ITEM( c_color, visuals_entities_loot_supply_drop_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( float, visuals_entities_loot_max_distance, 400.f );

    // food tab
    CFG_ITEM( bool, visuals_entities_food, false );

    CFG_ITEM( bool, visuals_entities_food_corn, true );
    CFG_ITEM( c_color, visuals_entities_food_corn_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( bool, visuals_entities_food_potato, true );
    CFG_ITEM( c_color, visuals_entities_food_potato_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( bool, visuals_entities_food_pumpkin, true );
    CFG_ITEM( c_color, visuals_entities_food_pumpkin_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( bool, visuals_entities_food_mushroom, true );
    CFG_ITEM( c_color, visuals_entities_food_mushroom_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( float, visuals_entities_food_max_distance, 400.f );

    // berry tab
    CFG_ITEM( bool, visuals_entities_berry, false );

    CFG_ITEM( bool, visuals_entities_berry_red, true );
    CFG_ITEM( c_color, visuals_entities_berry_red_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( bool, visuals_entities_berry_blue, true );
    CFG_ITEM( c_color, visuals_entities_berry_blue_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( bool, visuals_entities_berry_black, true );
    CFG_ITEM( c_color, visuals_entities_berry_black_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( bool, visuals_entities_berry_green, true );
    CFG_ITEM( c_color, visuals_entities_berry_green_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( bool, visuals_entities_berry_white, true );
    CFG_ITEM( c_color, visuals_entities_berry_white_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( bool, visuals_entities_berry_yellow, true );
    CFG_ITEM( c_color, visuals_entities_berry_yellow_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( float, visuals_entities_berry_max_distance, 400.f );

    // trap tab
    CFG_ITEM( bool, visuals_entities_trap, false );

    CFG_ITEM( bool, visuals_entities_trap_gun, true );
    CFG_ITEM( c_color, visuals_entities_trap_gun_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( bool, visuals_entities_trap_bear, true );
    CFG_ITEM( c_color, visuals_entities_trap_bear_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( bool, visuals_entities_trap_landmine, true );
    CFG_ITEM( c_color, visuals_entities_trap_landmine_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( bool, visuals_entities_trap_sam_site, true );
    CFG_ITEM( c_color, visuals_entities_trap_sam_site_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( bool, visuals_entities_trap_auto_turret, true );
    CFG_ITEM( c_color, visuals_entities_trap_auto_turret_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( bool, visuals_entities_trap_flame_turret, true );
    CFG_ITEM( c_color, visuals_entities_trap_flame_turret_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( float, visuals_entities_trap_max_distance, 400.f );

    // crate tab
    CFG_ITEM( bool, visuals_entities_crate, false );

    CFG_ITEM( bool, visuals_entities_crate_fuel, true );
    CFG_ITEM( c_color, visuals_entities_crate_fuel_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( bool, visuals_entities_crate_food, true );
    CFG_ITEM( c_color, visuals_entities_crate_food_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( bool, visuals_entities_crate_mine, true );
    CFG_ITEM( c_color, visuals_entities_crate_mine_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( bool, visuals_entities_crate_tools, true );
    CFG_ITEM( c_color, visuals_entities_crate_tools_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( bool, visuals_entities_crate_elite, true );
    CFG_ITEM( c_color, visuals_entities_crate_elite_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( bool, visuals_entities_crate_basic, true );
    CFG_ITEM( c_color, visuals_entities_crate_basic_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( bool, visuals_entities_crate_normal, true );
    CFG_ITEM( c_color, visuals_entities_crate_normal_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( bool, visuals_entities_crate_medical, true );
    CFG_ITEM( c_color, visuals_entities_crate_medical_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( bool, visuals_entities_crate_military, true );
    CFG_ITEM( c_color, visuals_entities_crate_military_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( bool, visuals_entities_crate_ammunition, true );
    CFG_ITEM( c_color, visuals_entities_crate_ammunition_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( bool, visuals_entities_crate_underwater_basic, true );
    CFG_ITEM( c_color, visuals_entities_crate_underwater_basic_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( bool, visuals_entities_crate_underwater_advanced, true );
    CFG_ITEM( c_color, visuals_entities_crate_underwater_advanced_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( float, visuals_entities_crate_max_distance, 400.f );

    // barrel tab
    CFG_ITEM( bool, visuals_entities_barrel, false );

    CFG_ITEM( bool, visuals_entities_barrel_oil, true );
    CFG_ITEM( c_color, visuals_entities_barrel_oil_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( bool, visuals_entities_barrel_blue, true );
    CFG_ITEM( c_color, visuals_entities_barrel_blue_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( bool, visuals_entities_barrel_basic, true );
    CFG_ITEM( c_color, visuals_entities_barrel_basic_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( float, visuals_entities_barrel_max_distance, 400.f );

    // animal tab
    CFG_ITEM( bool, visuals_entities_animal, false );

    CFG_ITEM( bool, visuals_entities_animal_stag, true );
    CFG_ITEM( c_color, visuals_entities_animal_stag_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( bool, visuals_entities_animal_boar, true );
    CFG_ITEM( c_color, visuals_entities_animal_boar_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( bool, visuals_entities_animal_bear, true );
    CFG_ITEM( c_color, visuals_entities_animal_bear_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( bool, visuals_entities_animal_wolf, true );
    CFG_ITEM( c_color, visuals_entities_animal_wolf_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( bool, visuals_entities_animal_polar_bear, true );
    CFG_ITEM( c_color, visuals_entities_animal_polar_bear_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( float, visuals_entities_animal_max_distance, 400.f );

    // vehicle tab
    CFG_ITEM( bool, visuals_entities_vehicle, false );

    CFG_ITEM( bool, visuals_entities_vehicle_base, true );
    CFG_ITEM( c_color, visuals_entities_vehicle_base_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( bool, visuals_entities_vehicle_horse, true );
    CFG_ITEM( c_color, visuals_entities_vehicle_horse_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( bool, visuals_entities_vehicle_scrap_heli, true );
    CFG_ITEM( c_color, visuals_entities_vehicle_scrap_heli_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( bool, visuals_entities_vehicle_helicopter, true );
    CFG_ITEM( c_color, visuals_entities_vehicle_helicopter_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( bool, visuals_entities_vehicle_air_baloon, true );
    CFG_ITEM( c_color, visuals_entities_vehicle_air_baloon_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( bool, visuals_entities_vehicle_mini_copter, true );
    CFG_ITEM( c_color, visuals_entities_vehicle_mini_copter_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( bool, visuals_entities_vehicle_bradley_apc, true );
    CFG_ITEM( c_color, visuals_entities_vehicle_bradley_apc_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( float, visuals_entities_vehicle_max_distance, 400.f );

    // cupboard tab
    CFG_ITEM( bool, visuals_entities_cupboard, false );
    CFG_ITEM( c_color, visuals_entities_cupboard_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( bool, visuals_entities_cupboard_authorized, true );

    CFG_ITEM( float, visuals_entities_cupboard_max_distance, 400.f );

    // construction tab
    CFG_ITEM( bool, visuals_entities_construction, false );

    CFG_ITEM( bool, visuals_entities_construction_recycler, true );
    CFG_ITEM( c_color, visuals_entities_construction_recycler_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( bool, visuals_entities_construction_workbench_1, true );
    CFG_ITEM( c_color, visuals_entities_construction_workbench_1_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( bool, visuals_entities_construction_workbench_2, true );
    CFG_ITEM( c_color, visuals_entities_construction_workbench_2_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( bool, visuals_entities_construction_workbench_3, true );
    CFG_ITEM( c_color, visuals_entities_construction_workbench_3_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( bool, visuals_entities_construction_large_wood_box, true );
    CFG_ITEM( c_color, visuals_entities_construction_large_wood_box_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( bool, visuals_entities_construction_wood_storage_box, true );
    CFG_ITEM( c_color, visuals_entities_construction_wood_storage_box_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( float, visuals_entities_construction_max_distance, 400.f );

    // collectible tab
    CFG_ITEM( bool, visuals_entities_collectible, false );

    CFG_ITEM( bool, visuals_entities_collectible_hemp, true );
    CFG_ITEM( c_color, visuals_entities_collectible_hemp_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( bool, visuals_entities_collectible_wood, true );
    CFG_ITEM( c_color, visuals_entities_collectible_wood_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( bool, visuals_entities_collectible_stone, true );
    CFG_ITEM( c_color, visuals_entities_collectible_stone_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( bool, visuals_entities_collectible_metal, true );
    CFG_ITEM( c_color, visuals_entities_collectible_metal_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( bool, visuals_entities_collectible_sulfur, true );
    CFG_ITEM( c_color, visuals_entities_collectible_sulfur_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( bool, visuals_entities_collectible_diesel, true );
    CFG_ITEM( c_color, visuals_entities_collectible_diesel_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( float, visuals_entities_collectible_max_distance, 400.f );

    // dropped_item tab
    CFG_ITEM( bool, visuals_entities_dropped_item, false );

    CFG_ITEM( bool, visuals_entities_dropped_item_all, true );
    CFG_ITEM( c_color, visuals_entities_dropped_item_all_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( bool, visuals_entities_dropped_item_fun, true );
    CFG_ITEM( c_color, visuals_entities_dropped_item_fun_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( bool, visuals_entities_dropped_item_food, true );
    CFG_ITEM( c_color, visuals_entities_dropped_item_food_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( bool, visuals_entities_dropped_item_tool, true );
    CFG_ITEM( c_color, visuals_entities_dropped_item_tool_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( bool, visuals_entities_dropped_item_misc, true );
    CFG_ITEM( c_color, visuals_entities_dropped_item_misc_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( bool, visuals_entities_dropped_item_items, true );
    CFG_ITEM( c_color, visuals_entities_dropped_item_items_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( bool, visuals_entities_dropped_item_traps, true );
    CFG_ITEM( c_color, visuals_entities_dropped_item_traps_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( bool, visuals_entities_dropped_item_common, true );
    CFG_ITEM( c_color, visuals_entities_dropped_item_common_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( bool, visuals_entities_dropped_item_weapon, true );
    CFG_ITEM( c_color, visuals_entities_dropped_item_weapon_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( bool, visuals_entities_dropped_item_attire, true );
    CFG_ITEM( c_color, visuals_entities_dropped_item_attire_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( bool, visuals_entities_dropped_item_search, true );
    CFG_ITEM( c_color, visuals_entities_dropped_item_search_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( bool, visuals_entities_dropped_item_medical, true );
    CFG_ITEM( c_color, visuals_entities_dropped_item_medical_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( bool, visuals_entities_dropped_item_resources, true );
    CFG_ITEM( c_color, visuals_entities_dropped_item_resources_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( bool, visuals_entities_dropped_item_component, true );
    CFG_ITEM( c_color, visuals_entities_dropped_item_component_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( bool, visuals_entities_dropped_item_favourite, true );
    CFG_ITEM( c_color, visuals_entities_dropped_item_favourite_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( bool, visuals_entities_dropped_item_electrical, true );
    CFG_ITEM( c_color, visuals_entities_dropped_item_electrical_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( bool, visuals_entities_dropped_item_ammunition, true );
    CFG_ITEM( c_color, visuals_entities_dropped_item_ammunition_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( bool, visuals_entities_dropped_item_construction, true );
    CFG_ITEM( c_color, visuals_entities_dropped_item_construction_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( bool, visuals_entities_dropped_item_chams, false );
    CFG_ITEM( bool, visuals_entities_dropped_item_icons, false );
    CFG_ITEM( bool, visuals_entities_dropped_item_amount, true );
    CFG_ITEM( bool, visuals_entities_dropped_item_despawn_duration, true );

    CFG_ITEM( float, visuals_entities_dropped_item_max_distance, 400.f );

    // vehicle water tab
    CFG_ITEM( bool, visuals_entities_vehicle_water, false );

    CFG_ITEM( bool, visuals_entities_vehicle_water_rhib, true );
    CFG_ITEM( c_color, visuals_entities_vehicle_water_rhib_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( bool, visuals_entities_vehicle_water_kayak, true );
    CFG_ITEM( c_color, visuals_entities_vehicle_water_kayak_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( bool, visuals_entities_vehicle_water_rowboat, true );
    CFG_ITEM( c_color, visuals_entities_vehicle_water_rowboat_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( bool, visuals_entities_vehicle_water_tugboat, true );
    CFG_ITEM( c_color, visuals_entities_vehicle_water_tugboat_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( bool, visuals_entities_vehicle_water_submarine, true );
    CFG_ITEM( c_color, visuals_entities_vehicle_water_submarine_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( bool, visuals_entities_vehicle_water_submarine_duo, true );
    CFG_ITEM( c_color, visuals_entities_vehicle_water_submarine_duo_color, ( c_color{ 255, 255, 255, 255 } ) );

    CFG_ITEM( float, visuals_entities_vehicle_water_max_distance, 400.f );

    // misc

    CFG_ITEM( bool, misc_fov_zoom_fov, false );
    CFG_ITEM( float, misc_fov_zoom_fov_amount, 15.f );
    CFG_ITEM( int, misc_fov_zoom_fov_key, 0 );

    CFG_ITEM( bool, misc_fov_fov_changer, false );
    CFG_ITEM( float, misc_fov_fov_changer_amount, 90.f );

    CFG_ITEM( bool, misc_automatic_auto_heal, false );
    CFG_ITEM( bool, misc_automatic_auto_trade, false );
    CFG_ITEM( bool, misc_automatic_auto_reload, false );

    CFG_ITEM( bool, misc_automatic_auto_upgrade, false );
    CFG_ITEM( int, misc_automatic_auto_upgrade_skin, 0 );
    CFG_ITEM( int, misc_automatic_auto_upgrade_tier, 0 );

    CFG_ITEM( bool, misc_farm_assist, false );
    CFG_ITEM( bool, misc_climb_assist, false );
    CFG_ITEM( bool, misc_infinite_jump, false );
    CFG_ITEM( bool, misc_disarm_landmines, false );
    CFG_ITEM( bool, misc_modify_can_attack, false );
    CFG_ITEM( bool, misc_disable_land_damage, false );

    CFG_ITEM( bool, misc_suicide, false );
    CFG_ITEM( int, misc_suicide_key, 0 );

    CFG_ITEM( bool, misc_debug_camera, false );
    CFG_ITEM( int, misc_debug_camera_key, 0 );

    CFG_ITEM( bool, misc_high_jump, false );
    CFG_ITEM( float, misc_high_jump_height, 100.f );

    // misc weapon
    CFG_ITEM( bool, misc_weapon_spam, false );
    CFG_ITEM( int, misc_weapon_spam_key, 0 );
    CFG_ITEM( float, misc_weapon_spam_delay, 10.f );

    CFG_ITEM( bool, misc_weapon_chams, false );
    CFG_ITEM( int, misc_weapon_chams_value, 0 );

    CFG_ITEM( bool, misc_weapon_fast_bow, false );
    CFG_ITEM( bool, misc_weapon_automatic, false );
    CFG_ITEM( bool, misc_weapon_instant_charge_compound, false );
    CFG_ITEM( bool, misc_weapon_disable_slow_down_melee, false );

    CFG_ITEM( bool, misc_weapon_fast_eoka, false );
    CFG_ITEM( float, misc_weapon_fast_eoka_chance, 0.f );

    CFG_ITEM( bool, misc_weapon_extended_melee, false );
    CFG_ITEM( float, misc_weapon_extended_melee_radius, 0.f );

    CFG_ITEM( bool, misc_weapon_rapid_fire, false );
    CFG_ITEM( float, misc_weapon_rapid_fire_speed, 0.f );

    // exploits
    CFG_ITEM( bool, exploits_spinbot, false );
    CFG_ITEM( bool, exploits_fast_loot, false );
    CFG_ITEM( bool, exploits_admin_flag, false );
    CFG_ITEM( bool, exploits_omni_sprint, false );
    CFG_ITEM( bool, exploits_name_spoofer, false );
    CFG_ITEM( bool, exploits_walk_on_water, false );
    CFG_ITEM( bool, exploits_anti_fly_hack_kick, false );
    CFG_ITEM( bool, exploits_walk_through_trees, false );
    CFG_ITEM( bool, exploits_walk_through_players, false );
    CFG_ITEM( bool, exploits_can_attack_in_vehicles, false );

    CFG_ITEM( bool, exploits_speedhack, false );
    CFG_ITEM( int, exploits_speedhack_key, 0 );
    CFG_ITEM( float, exploits_speedhack_value, 0.f );

    // settings
    CFG_ITEM( bool, settings_panic, false );
    CFG_ITEM( int, settings_panic_key, 0 );

    CFG_ITEM( int, settings_language, 0 );
}

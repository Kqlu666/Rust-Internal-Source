#pragma once

#include "../../options/config_mgr.hpp"

namespace sdk {
    namespace bundles {
        inline bool initialized = false;

        inline unity::c_shader* flat_shader;
        inline unity::c_asset_bundle* flat_bundle;

        inline unity::c_shader* fire_a_shader;
        inline unity::c_material* fire_a_material;
        inline unity::c_asset_bundle* fire_a_bundle;

        inline unity::c_shader* fire_b_shader;
        inline unity::c_material* fire_b_material;
        inline unity::c_asset_bundle* fire_b_bundle;

        inline unity::c_shader* glitter_shader;
        inline unity::c_material* glitter_material;
        inline unity::c_asset_bundle* glitter_bundle;

        inline unity::c_shader* damascus_shader;
        inline unity::c_material* damascus_material;
        inline unity::c_asset_bundle* damascus_bundle;

        inline unity::c_shader* lightning_shader;
        inline unity::c_material* lightning_material;
        inline unity::c_asset_bundle* lightning_bundle;

        inline unity::c_shader* geometric_shader;
        inline unity::c_material* geometric_material;
        inline unity::c_asset_bundle* geometric_bundle;

        inline unity::c_shader* wire_frame_shader;
        inline unity::c_material* wire_frame_material;
        inline unity::c_asset_bundle* wire_frame_bundle;

        inline unity::c_game_object* ghost_hit_effect_prefab;
        inline unity::c_asset_bundle* ghost_hit_effect_bundle;

        inline bool initialize( ) {
            if ( initialized )
                return false;

            const auto& game_path = util::game_path( );
            const auto& bundles_path = game_path + _( "\\secret\\bundles" );

            flat_bundle = unity::c_asset_bundle::load_from_file( ( bundles_path + _( "\\flat.secret" ) ).c_str( ) );
            if ( memory::is_valid( flat_bundle ) ) {
                TRACE( "flat_bundle @ 0x%llX", flat_bundle );

                flat_shader = flat_bundle->load_asset< unity::c_shader* >( _( "chams.shader" ), il2cpp::type_object( _( "UnityEngine" ), _( "Shader" ) ) );

                if ( memory::is_valid( flat_shader ) )
                    TRACE( "flat_shader @ 0x%llX", flat_shader );
            }

            fire_a_bundle = unity::c_asset_bundle::load_from_file( ( bundles_path + _( "\\fire_a.secret" ) ).c_str( ) );
            if ( memory::is_valid( fire_a_bundle ) ) {
                TRACE( "fire_bundle_a @ 0x%llX", fire_a_bundle );

                fire_a_shader = fire_a_bundle->load_asset< unity::c_shader* >( _( "new amplifyshader.shader" ), il2cpp::type_object( _( "UnityEngine" ), _( "Shader" ) ) );
                fire_a_material = fire_a_bundle->load_asset< unity::c_material* >( _( "fire.mat" ), il2cpp::type_object( _( "UnityEngine" ), _( "Material" ) ) );

                if ( memory::is_valid( fire_a_shader ) )
                    TRACE( "fire_a_shader @ 0x%llX", fire_a_shader );

                if ( memory::is_valid( fire_a_material ) )
                    TRACE( "fire_a_material @ 0x%llX", fire_a_material );
            }

            fire_b_bundle = unity::c_asset_bundle::load_from_file( ( bundles_path + _( "\\fire_b.secret" ) ).c_str( ) );
            if ( memory::is_valid( fire_b_bundle ) ) {
                TRACE( "fire_b_bundle @ 0x%llX", fire_b_bundle );

                fire_b_shader = fire_b_bundle->load_asset< unity::c_shader* >( _( "new amplifyshader.shader" ), il2cpp::type_object( _( "UnityEngine" ), _( "Shader" ) ) );
                fire_b_material = fire_b_bundle->load_asset< unity::c_material* >( _( "fire2.mat" ), il2cpp::type_object( _( "UnityEngine" ), _( "Material" ) ) );

                if ( memory::is_valid( fire_b_shader ) )
                    TRACE( "fire_b_shader @ 0x%llX", fire_b_shader );

                if ( memory::is_valid( fire_b_material ) )
                    TRACE( "fire_b_material @ 0x%llX", fire_b_material );
            }

            glitter_bundle = unity::c_asset_bundle::load_from_file( ( bundles_path + _( "\\glitter.secret" ) ).c_str( ) );
            if ( memory::is_valid( glitter_bundle ) ) {
                TRACE( "glitter_bundle @ 0x%llX", glitter_bundle );

                glitter_shader = glitter_bundle->load_asset< unity::c_shader* >( _( "el_glitter.shader" ), il2cpp::type_object( _( "UnityEngine" ), _( "Shader" ) ) );
                glitter_material = glitter_bundle->load_asset< unity::c_material* >( _( "el_glitter.mat" ), il2cpp::type_object( _( "UnityEngine" ), _( "Material" ) ) );

                if ( memory::is_valid( glitter_shader ) )
                    TRACE( "glitter_shader @ 0x%llX", glitter_shader );

                if ( memory::is_valid( glitter_material ) )
                    TRACE( "glitter_material @ 0x%llX", glitter_material );
            }

            damascus_bundle = unity::c_asset_bundle::load_from_file( ( bundles_path + _( "\\damascus.secret" ) ).c_str( ) );
            if ( memory::is_valid( damascus_bundle ) ) {
                TRACE( "damascus_bundle @ 0x%llX", damascus_bundle );

                damascus_shader = damascus_bundle->load_asset< unity::c_shader* >( _( "el_designshader.shader" ), il2cpp::type_object( _( "UnityEngine" ), _( "Shader" ) ) );
                damascus_material = damascus_bundle->load_asset< unity::c_material* >( _( "el_designeffect.mat" ), il2cpp::type_object( _( "UnityEngine" ), _( "Material" ) ) );

                if ( memory::is_valid( damascus_shader ) )
                    TRACE( "damascus_shader @ 0x%llX", damascus_shader );

                if ( memory::is_valid( damascus_material ) )
                    TRACE( "damascus_material @ 0x%llX", damascus_material );
            }

            lightning_bundle = unity::c_asset_bundle::load_from_file( ( bundles_path + _( "\\lightning.secret" ) ).c_str( ) );
            if ( memory::is_valid( lightning_bundle ) ) {
                TRACE( "lightning_bundle @ 0x%llX", lightning_bundle );

                lightning_shader = lightning_bundle->load_asset< unity::c_shader* >( _( "poiyomi pro.shader" ), il2cpp::type_object( _( "UnityEngine" ), _( "Shader" ) ) );
                lightning_material = lightning_bundle->load_asset< unity::c_material* >( _( "lightning.mat" ), il2cpp::type_object( _( "UnityEngine" ), _( "Material" ) ) );

                if ( memory::is_valid( lightning_shader ) )
                    TRACE( "lightning_shader @ 0x%llX", lightning_shader );

                if ( memory::is_valid( lightning_material ) )
                    TRACE( "lightning_material @ 0x%llX", lightning_material );
            }

            geometric_bundle = unity::c_asset_bundle::load_from_file( ( bundles_path + _( "\\geometric.secret" ) ).c_str( ) );
            if ( memory::is_valid( geometric_bundle ) ) {
                TRACE( "geometric_bundle @ 0x%llX", geometric_bundle );

                geometric_shader = geometric_bundle->load_asset< unity::c_shader* >( _( "poiyomi pro geometric dissolve.shader" ), il2cpp::type_object( _( "UnityEngine" ), _( "Shader" ) ) );
                geometric_material = geometric_bundle->load_asset< unity::c_material* >( _( "galaxy.mat" ), il2cpp::type_object( _( "UnityEngine" ), _( "Material" ) ) );

                if ( memory::is_valid( geometric_shader ) )
                    TRACE( "geometric_shader @ 0x%llX", geometric_shader );

                if ( memory::is_valid( geometric_material ) )
                    TRACE( "geometric_material @ 0x%llX", geometric_material );
            }

            wire_frame_bundle = unity::c_asset_bundle::load_from_file( ( bundles_path + _( "\\wire_frame.secret" ) ).c_str( ) );
            if ( memory::is_valid( wire_frame_bundle ) ) {
                TRACE( "wire_frame_bundle @ 0x%llX", wire_frame_bundle );

                wire_frame_shader = wire_frame_bundle->load_asset< unity::c_shader* >( _( "poiyomi pro wireframe.shader" ), il2cpp::type_object( _( "UnityEngine" ), _( "Shader" ) ) );
                wire_frame_material = wire_frame_bundle->load_asset< unity::c_material* >( _( "wireframe.mat" ), il2cpp::type_object( _( "UnityEngine" ), _( "Material" ) ) );

                if ( memory::is_valid( wire_frame_shader ) )
                    TRACE( "wire_frame_shader @ 0x%llX", wire_frame_shader );

                if ( memory::is_valid( wire_frame_material ) )
                    TRACE( "wire_frame_material @ 0x%llX", wire_frame_material );
            }

            ghost_hit_effect_bundle = unity::c_asset_bundle::load_from_file( ( bundles_path + _( "\\ghost_hit_effect.secret" ) ).c_str( ) );
            if ( memory::is_valid( ghost_hit_effect_bundle ) ) {
                TRACE( "ghost_hit_effect_bundle @ 0x%llX", ghost_hit_effect_bundle );

                ghost_hit_effect_prefab = ghost_hit_effect_bundle->load_asset< unity::c_game_object* >( _( "cfxr2 souls escape.prefab" ), il2cpp::type_object( _( "UnityEngine" ), _( "GameObject" ) ) );
                ghost_hit_effect_prefab->dont_destroy_on_load( );

                if ( memory::is_valid( ghost_hit_effect_prefab ) )
                    TRACE( "ghost_hit_effect_prefab @ 0x%llX", ghost_hit_effect_prefab );
            }

            initialized = true;
            return true;
        }

        inline bool destroy_all( ) {
            if ( !initialized )
                return false;

            if ( memory::is_valid( flat_bundle ) ) {
                flat_bundle->unload( true );
                flat_shader = nullptr;
                flat_bundle = nullptr;
            }

            if ( memory::is_valid( fire_a_bundle ) ) {
                fire_a_bundle->unload( true );
                fire_a_shader = nullptr;
                fire_a_material = nullptr;
                fire_a_bundle = nullptr;
            }

            if ( memory::is_valid( fire_b_bundle ) ) {
                fire_b_bundle->unload( true );
                fire_b_shader = nullptr;
                fire_b_material = nullptr;
                fire_b_bundle = nullptr;
            }

            if ( memory::is_valid( glitter_bundle ) ) {
                glitter_bundle->unload( true );
                glitter_shader = nullptr;
                glitter_material = nullptr;
                glitter_bundle = nullptr;
            }

            if ( memory::is_valid( damascus_bundle ) ) {
                damascus_bundle->unload( true );
                damascus_shader = nullptr;
                damascus_material = nullptr;
                damascus_bundle = nullptr;
            }

            if ( memory::is_valid( lightning_bundle ) ) {
                lightning_bundle->unload( true );
                lightning_shader = nullptr;
                lightning_material = nullptr;
                lightning_bundle = nullptr;
            }

            if ( memory::is_valid( geometric_bundle ) ) {
                geometric_bundle->unload( true );
                geometric_shader = nullptr;
                geometric_material = nullptr;
                geometric_bundle = nullptr;
            }

            if ( memory::is_valid( wire_frame_bundle ) ) {
                wire_frame_bundle->unload( true );
                wire_frame_shader = nullptr;
                wire_frame_material = nullptr;
                wire_frame_bundle = nullptr;
            }

            if ( memory::is_valid( ghost_hit_effect_bundle ) ) {
                ghost_hit_effect_bundle->unload( true );
                ghost_hit_effect_prefab = nullptr;
                ghost_hit_effect_bundle = nullptr;
            }

            initialized = false;
            return true;
        }
    }
}
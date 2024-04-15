#pragma once

namespace features {
	inline std::vector< std::pair< sdk::e_bone, sdk::e_bone > > bones = {

		{ sdk::e_bone::head, sdk::e_bone::spine4 },
		{ sdk::e_bone::spine4, sdk::e_bone::l_upperarm },
		{ sdk::e_bone::l_upperarm, sdk::e_bone::l_forearm },

		{ sdk::e_bone::l_forearm, sdk::e_bone::l_hand },
		{ sdk::e_bone::spine4, sdk::e_bone::r_upperarm },
		{ sdk::e_bone::r_upperarm, sdk::e_bone::r_forearm },

		{ sdk::e_bone::r_forearm, sdk::e_bone::r_hand },
		{ sdk::e_bone::spine4, sdk::e_bone::pelvis },
		{ sdk::e_bone::pelvis, sdk::e_bone::l_hip },

		{ sdk::e_bone::l_hip , sdk::e_bone::l_knee },
		{ sdk::e_bone::l_knee, sdk::e_bone::l_foot },
		{ sdk::e_bone::pelvis, sdk::e_bone::r_hip },

		{ sdk::e_bone::r_hip, sdk::e_bone::r_knee },
		{ sdk::e_bone::r_knee, sdk::e_bone::r_foot },
		{ sdk::e_bone::r_foot, sdk::e_bone::r_toe },
		{ sdk::e_bone::l_foot, sdk::e_bone::l_toe }
	};

	class c_visuals : public singleton< c_visuals > {
	public:
		inline void render_hotbar( ) {
			if ( !config::visuals_players_hotbar )
				return;

			constexpr auto base_size = ImVec2( 60.f, 60.f );

			float aspect_ratio = static_cast< float >( unity::c_screen::width( ) ) / static_cast< float >( unity::c_screen::height( ) );
			float scale = std::min( static_cast< float >( unity::c_screen::width( ) ) / 1920.0f, static_cast< float >( unity::c_screen::height( ) ) / 1080.0f );

			const auto& target = features::aimbot.get_target( );
			if ( !memory::is_valid( target.player ) )
				return;

			if ( target.teammate )
				return;

			const auto& model = target.player->model( );
			if ( !memory::is_valid( model ) )
				return;

			if ( model->is_npc( ) )
				return;

			const auto& inv = target.player->inventory( );
			if ( !memory::is_valid( inv ) )
				return;

			const auto& container_belt = inv->container_belt( );
			if ( !memory::is_valid( container_belt ) )
				return;

			const auto& item_list = container_belt->item_list( );
			if ( !memory::is_valid( item_list ) )
				return;

			std::string nickname = {};
			if ( memory::is_valid( target.player->display_name( ) ) )
				nickname = target.player->display_name( )->string( );

			const auto& nickname_size = ImGui::CalcTextSize( nickname.c_str( ) );
			renderer.draw_text_outline( ImVec2{ 710.f * scale + 50.f * scale + ( ( ( base_size.x + 5.f ) * 6 ) / 2.f ) - nickname_size.x / 2.f, 900.f * scale - nickname_size.y - 1.f }, 10.f * scale, c_color{ 255, 255, 255, 255 }, nickname.c_str( ) );

			for ( int i = 0; i < 6; ++i ) {
				const auto& position = ImVec2{ 710.f * scale + 50.f * scale + ( i * ( base_size.x * scale + 5.f ) ), 900.f * scale };
				renderer.draw_filled_rect( position, base_size * scale, c_color{ 34, 34, 34, 120 }, 8.f * scale );

				const auto& item = item_list->value( i );
				if ( !memory::is_valid( item ) )
					continue;

				const auto& sprite = item->icon_sprite( );
				if ( !memory::is_valid( sprite ) )
					continue;

				const auto& texture = sprite->texture( );
				if ( !memory::is_valid( texture ) )
					continue;

				const auto& srv = texture->to_srv( texture->texture_id( ) );
				if ( !memory::is_valid( srv ) )
					continue;

				renderer.draw_texture( srv, position, base_size * scale, c_color::white( ) );

				const auto& amount = item->amount( );
				if ( amount <= 1 )
					continue;

				const auto& amount_text = util::format( _( "%d" ), amount );

				const auto& amount_size = ImGui::CalcTextSize( amount_text );
				renderer.draw_text_outline( { position.x + base_size.x * scale - amount_size.x - 1.f, position.y + base_size.y * scale - amount_size.y - 1.f }, 10.f * scale, c_color{ 255, 255, 255, 255 }, amount_text );
			}
		}

		inline void render_player( sdk::c_base_player* player ) {
			if ( !config::visuals_players )
				return;

			if ( !config::visuals_players_teammate && player->is_teammate( ) )
				return;

			const auto& model = player->model( );
			if ( !memory::is_valid( model ) )
				return;

			const auto& is_npc = model->is_npc( );
			if ( ( is_npc && !config::visuals_players_npc ) )
				return;

			const auto& is_sleeping = player->is_sleeping( );
			if ( is_sleeping && !config::visuals_players_sleepers )
				return;

			const auto& transform = player->transform( );
			if ( !memory::is_valid( transform ) )
				return;

			const auto& base_position = transform->position( );

			const auto& distance = sdk::local_position.distance_to( base_position );
			if ( ( is_npc && distance > config::visuals_players_npc_max_distance ) || ( !is_npc && distance > config::visuals_players_max_distance ) )
				return;

			vec2_t base_point, head_point, standing_point;
			if ( !sdk::camera->world_to_screen_point( base_position, base_point )
				 || !sdk::camera->world_to_screen_point( player->bone_position( sdk::e_bone::head ), head_point )
				 || !sdk::camera->world_to_screen_point( base_position + vec3_t{ 0.f, 1.9f, 0.f }, standing_point ) )
				return;

			const auto& box_size = vec2_t{ ( base_point.y - standing_point.y ) * 0.45f, ( base_point.y - head_point.y ) * 1.15f };
			const auto& box_position = vec2_t{ base_point.x - box_size.x * 0.5f, base_point.y - box_size.y };

			const auto& is_visible = player->is_visible( );
			const auto& is_teammate = player->is_teammate( );

			c_color ignore_color = { };
			if ( is_sleeping )
				ignore_color = config::visuals_players_sleepers_color;
			else if ( config::visuals_players_visible_check && is_visible )
				ignore_color = config::visuals_players_visible_check_color;
			else if ( is_npc )
				ignore_color = config::visuals_players_npc_color;
			else if ( is_teammate )
				ignore_color = config::visuals_players_teammate_color;

			const auto& is_has_state = is_npc || is_sleeping || is_teammate || ( config::visuals_players_visible_check && is_visible );

			if ( config::visuals_players_box )
				renderer.draw_corner_box_border( box_position.get( ), box_size.get( ), is_has_state ? ignore_color : config::visuals_players_box_color, c_color{ 0, 0, 0, 180 } );

			vec2_t top_pad( 0.f, 0.f );

			if ( config::visuals_players_name ) {
				std::string name = { };
				if ( is_npc ) {
					name += player->class_name( );
				} else {
					const auto& display_name = player->display_name( );
					if ( memory::is_valid( display_name ) )
						name += display_name->string( );
				}
	
				auto size = ImGui::CalcTextSize( name.c_str( ) );
				renderer.draw_text_outline( { box_position.x + ( box_size.x * 0.5f ), box_position.y - 14.f - top_pad.y - 2.f }, 12.f, text_center_h, is_has_state ? ignore_color : config::visuals_players_name_color, name.c_str( ) );
				top_pad.y += size[ 1 ];
			}

			if ( !is_npc ) {
				const auto player_state = player->player_state( );

				char display[ 256 ] = { };
				sprintf_s( display, ARRAYSIZE( display ), _( "%s" ), player_state == sdk::e_player_state::inside ? REPLACE_TEXT( "Inside", "里面", "В доме" ) : REPLACE_TEXT( "Outside", "外部", "На улице" ) );

				const auto& size = ImGui::CalcTextSize( display );
				renderer.draw_text_outline( { box_position.x + ( box_size.x * 0.5f ), box_position.y - 14.f - top_pad.y - 2.f }, 12.f, text_center_h, player_state == sdk::e_player_state::inside ? c_color( 255, 0, 0, 255 ) : c_color( 0, 255, 0, 255 ), display );
				top_pad.y += size[ 1 ];
			}

			vec2_t bottom_pad( 0.f, 0.f );

			if ( config::visuals_players_distance ) {
				char display[ 256 ] = { };
				sprintf_s( display, ARRAYSIZE( display ), _( "%.fm" ), distance );

				const auto size = ImGui::CalcTextSize( display );
				renderer.draw_text_outline( { box_position.x + ( box_size.x * 0.5f ), box_position.y + box_size.y + bottom_pad.y + 2.f }, 12.f, text_center_h, is_has_state ? ignore_color : config::visuals_players_distance_color, display );
				bottom_pad.y += size[ 1 ];
			}

			if ( config::visuals_players_weapon ) {
				const auto& weapon = player->item( );
				if ( memory::is_valid( weapon ) ) {
					const auto& name = weapon->info( )->display_name( )->name( );
					if ( memory::is_valid( name ) ) {
						const auto size = ImGui::CalcTextSize( name->string( ).c_str( ) );
						renderer.draw_text_outline( { box_position.x + ( box_size.x * 0.5f ), box_position.y + box_size.y + bottom_pad.y + 2.f }, 12.f, text_center_h, is_has_state ? ignore_color : config::visuals_players_weapon_color, name->string( ).c_str( ) );
						bottom_pad.y += size[ 1 ];
					}
				}
			}

			if ( config::visuals_players_skeleton ) {
				for ( const auto& [first, second] : bones ) {
					const auto& start_position = player->bone_position( first );
					const auto& end_position = player->bone_position( second );

					vec2_t start_point, end_point;
					if ( !sdk::camera->world_to_screen_point( start_position, start_point ) || !sdk::camera->world_to_screen_point( end_position, end_point ) )
						continue;

					renderer.draw_line( start_point.get( ), end_point.get( ), is_has_state ? ignore_color : config::visuals_players_skeleton_color );
				}
			}

			if ( config::visuals_players_chams && sdk::bundles::initialized ) {
				const auto& multi_mesh = model->multi_mesh( );
				if ( memory::is_valid( multi_mesh ) ) {
					const auto& renderers = multi_mesh->renderers( );
					if ( memory::is_valid( renderers ) ) {
						for ( auto i = 0; i < renderers->count( ); ++i ) {
							const auto& renderer = renderers->value( i );
							if ( !memory::is_valid( renderer ) )
								continue;

							const auto& material = renderer->material( );
							if ( !memory::is_valid( material ) )
								continue;

							switch ( config::visuals_players_chams_value ) {
							case 0:
								if ( material->shader( ) != sdk::bundles::flat_shader ) {
									material->set_shader( sdk::bundles::flat_shader );

									if ( is_npc ) {
										material->set_color( _( "_ColorVisible" ), unity::c_color( 29, 31, 161, 255 ) );
										material->set_color( _( "_ColorBehind" ), unity::c_color( 29, 31, 161, 255 ) );
									} else {
										material->set_color( _( "_ColorVisible" ), unity::c_color( 0, 255, 0, 255 ) );
										material->set_color( _( "_ColorBehind" ), unity::c_color( 255, 0, 0, 255 ) );
									}
								}
								break;
							case 1:
								if ( material->shader( ) != sdk::bundles::glitter_shader ) {
									sdk::bundles::glitter_material->set_shader( sdk::bundles::glitter_shader );
									renderer->set_material( sdk::bundles::glitter_material );
								}
								break;
							case 2:
								if ( material->shader( ) != sdk::bundles::damascus_shader ) {
									sdk::bundles::damascus_material->set_shader( sdk::bundles::damascus_shader );
									renderer->set_material( sdk::bundles::damascus_material );
								}
								break;
							case 3:
								if ( material->shader( ) != sdk::bundles::lightning_shader ) {
									sdk::bundles::lightning_material->set_shader( sdk::bundles::lightning_shader );
									renderer->set_material( sdk::bundles::lightning_material );
								}
								break;
							case 4:
								if ( material->shader( ) != sdk::bundles::wire_frame_shader ) {
									sdk::bundles::wire_frame_material->set_shader( sdk::bundles::wire_frame_shader );
									renderer->set_material( sdk::bundles::wire_frame_material );
								}
								break;
							case 5:
								if ( material->shader( ) != sdk::bundles::fire_a_shader ) {
									sdk::bundles::fire_a_material->set_shader( sdk::bundles::fire_a_shader );
									renderer->set_material( sdk::bundles::fire_a_material );
								}
								break;
							case 6:
								if ( material->shader( ) != sdk::bundles::fire_b_shader ) {
									sdk::bundles::fire_b_material->set_shader( sdk::bundles::fire_b_shader );
									renderer->set_material( sdk::bundles::fire_b_material );
								}
								break;
							case 7:
								if ( material->shader( ) != sdk::bundles::geometric_shader ) {
									sdk::bundles::geometric_material->set_shader( sdk::bundles::geometric_shader );
									renderer->set_material( sdk::bundles::geometric_material );
								}
								break;
							}
						}
					}
				}
			}
		}

		inline void render_map( ) {

		}

		inline void render_screen( ) {
			const auto& width = static_cast< float >( unity::c_screen::width( ) );
			const auto& height = static_cast< float >( unity::c_screen::height( ) );

			const auto& screen = vec2_t{ width / 2.f, height / 2.f };

			if ( config::aimbot_crosshair ) {
				renderer.draw_filled_rect( { screen.x - 5.f, screen.y - 1.f }, { 11.f, 3.f }, c_color{ 0, 0, 0, 255 } );
				renderer.draw_filled_rect( { screen.x - 1.f, screen.y - 5.f }, { 3.f, 11.f }, c_color{ 0, 0, 0, 255 } );

				renderer.draw_filled_rect( { screen.x - 4.f, screen.y }, { 9.f, 1.f }, config::aimbot_crosshair_color );
				renderer.draw_filled_rect( { screen.x, screen.y - 4.f }, { 1.f, 9.f }, config::aimbot_crosshair_color );
			}

			if ( config::aimbot_fov )
				renderer.draw_circle( screen.get( ), config::aimbot_fov_value, config::aimbot_fov_color );

			if ( config::visuals_time_show_server_time ) {
				const auto& format = util::format( _( "%s" ), REPLACE_TEXT( "Server time", "服务器时间", "Серверное время" ) );
				renderer.draw_text_outline( { 10.f, 10.f }, 12.f, c_color( 255, 255, 255, 255 ), _( "%s: %02d:%02d" ), format, features::current_hour, features::current_minute );
			}

			auto float_to_traffic_light = [ ]( float f ) {
				int r, g, b = 63;
				if ( f <= 0.5f ) {
					r = 255;
					g = 255 * ( f * 2.f );
				} else {
					g = 255;
					r = 255 * ( 1 - f ) * 2.f;
				}

				return c_color( r, g, b );
			};

			int bottom_items_count = 0;

			if ( config::aimbot_manipulation && manipulation_indicator && input_handler.is_hold( config::aimbot_manipulation_key ) ) {
				renderer.draw_text_outline( ImVec2( screen.x, screen.y - bottom_items_count * 8 ), 11.f, 
											text_center, c_color::red( ), REPLACE_TEXT( "Manipulated", "被操纵", "Манипуляция" ) );
				bottom_items_count++;
			}

			if ( config::misc_automatic_auto_reload && !did_reload && time_since_last_shot <= ( reload_time - ( reload_time / 10 ) ) && time_since_last_shot > 0 ) {
				float reload_threshold = ( reload_time - ( reload_time / 10 ) );
				float reload_progress_percentage = ( ( time_since_last_shot / reload_threshold ) * 100.0f + 1.f ) / 100;

				if ( reload_progress_percentage > 0.01f ) {
					auto x_position = screen.x - 50;
					auto x_position2 = screen.x - 49;
					auto y_position = screen.y + bottom_items_count * 8;

					renderer.draw_filled_rect( { x_position, y_position - 1 }, { 100, 2 }, c_color( 0, 0, 0 ) );
					renderer.draw_filled_rect( { x_position, y_position + 1 }, { 100, 4 }, c_color( 0, 0, 0 ) );
					renderer.draw_filled_rect( { x_position2, y_position }, { ( 100 * reload_progress_percentage ), 3 }, float_to_traffic_light( reload_progress_percentage ) );
					bottom_items_count++;
				}
			}
			
			if ( config::aimbot_manipulation && input_handler.is_hold( config::aimbot_manipulation_key ) ) {
				auto desync_time = unity::c_time::realtime_since_startup( ) - sdk::local_player->last_sent_tick_time( );
				auto desync_percentage = ( ( desync_time / 0.85f ) * 100.0f + 1.f ) / 100;

				if ( desync_percentage < 0.1 )
					desync_percentage = 0;

				if ( desync_percentage != 0 ) {
					auto x_position = screen.x - 50;
					auto x_position2 = screen.x - 49;
					auto y_position = screen.y + bottom_items_count * 8;

					renderer.draw_filled_rect( { x_position, y_position - 1 }, { 100, 2 }, c_color( 0, 0, 0, 255 ) );
					renderer.draw_filled_rect( { x_position, y_position + 1 }, { 100, 4 }, c_color( 0, 0, 0, 255 ) );
					renderer.draw_filled_rect( { x_position2, y_position }, { ( 100 * desync_time ), 3 }, float_to_traffic_light( desync_percentage ) );
					bottom_items_count++;
				}
			}

			if ( config::exploits_anti_fly_hack_kick ) {
				static float alpha[ 2 ]{ 0.f };

				if ( flyhack_vertical > 0.f
					 && alpha[ 1 ] < 1.f )
					alpha[ 1 ] = std::lerp( alpha[ 1 ], 1.f, 0.15f );
				else if ( alpha[ 1 ] > 0.f )
					alpha[ 1 ] = std::lerp( alpha[ 1 ], 0.f, 0.15f );

				if ( flyhack_horizontal > 0.f
					 && alpha[ 0 ] < 1.f )
					alpha[ 0 ] = std::lerp( alpha[ 0 ], 1.f, 0.15f );
				else if ( alpha[ 0 ] > 0.f )
					alpha[ 0 ] = std::lerp( alpha[ 0 ], 0.f, 0.15f );

				auto vert_percentage = ( flyhack_vertical / flyhack_max_vertical );
				const auto red = vert_percentage * 255.f;
				const auto green = 255.f - red;
				vert_percentage = std::clamp( vert_percentage, 0.f, 1.f );

				if ( alpha[ 1 ] ) {
					renderer.draw_filled_rect( { screen.x - 200, screen.y - 426 }, { 402, 6 },
											   c_color( 0.f, 0.f, 0.f, 255.f * alpha[ 1 ] ) );

					renderer.draw_filled_rect( { screen.x - 199, screen.y - 425 }, { 400 * vert_percentage, 4 },
											   c_color( red, green, 0.f, 255.f * alpha[ 1 ] ) );
				}

				auto hor_percentage = ( flyhack_horizontal / flyhack_max_horizontal );
				const auto red_horizontal = hor_percentage * 255.f;
				const auto green_horizontal = 255.f - red_horizontal;
				hor_percentage = std::clamp( hor_percentage, 0.f, 1.f );

				if ( alpha[ 0 ] ) {
					renderer.draw_filled_rect( { screen.x - 200, screen.y - 446 }, { 402, 6 },
											   c_color( 0.f, 0.f, 0.f, 255.f * alpha[ 0 ] ) );

					renderer.draw_filled_rect( { screen.x - 199, screen.y - 445 }, { 400 * hor_percentage, 4 },
											   c_color( red_horizontal, green_horizontal, 0.f, 255.f * alpha[ 0 ] ) );
				}
			}
		}

		inline void render_world_item( sdk::world_item_t world_item ) {
			const auto& distance = sdk::local_position.distance_to( world_item.position );

			const auto bb_minmax = world_item.find_bb_minmax( world_item.position, world_item.bounds.center,
															  world_item.bounds.extents, world_item.rotation );
			const auto& min = bb_minmax.first, & max = bb_minmax.second;
			if ( min.empty( ) || max.empty( ) )
				return;

			const auto& box_size = vec2_t{ max.x - min.x, max.y - min.y };
			const auto& box_position = vec2_t{ min.x, min.y };

			if ( world_item.is_dropped_item ) {
				if ( config::visuals_entities_dropped_item_amount ) {
					const auto& format = util::format( _( "%s x%d ( %.fm )" ), world_item.name.c_str( ), world_item.amount, distance );
					renderer.draw_text_outline( { box_position.x, box_position.y }, 11.f, world_item.color, format );
				} else {
					const auto& format = util::format( _( "%s ( %.fm )" ), world_item.name.c_str( ), distance );
					renderer.draw_text_outline( { box_position.x, box_position.y }, 11.f, world_item.color, format );
				}

				if ( world_item.image )
					renderer.draw_texture( world_item.image, { box_position.x - ( ( box_size.x * 2.f ) * 0.5f ), box_position.y - ( box_size.y * 2.f ) - 5.f },
										   { box_size.x * 2.f, box_size.y * 2.f }, c_color::white( ) );
			} else {
				const auto& format = util::format( _( "%s ( %.fm )" ), world_item.name.c_str( ), distance );
				renderer.draw_text_outline( { box_position.x, box_position.y }, 11.f, world_item.color, format );

				if ( world_item.is_corpse ) {
					const auto& size = ImGui::CalcTextSize( format );
					const auto& player_name_size = ImGui::CalcTextSize( world_item.player_name.c_str( ) );
					renderer.draw_text_outline( { box_position.x + ( size.x * 0.5f ) - ( player_name_size.x * 0.5f ), box_position.y + size.y + 2.f }, 11.f, world_item.color, world_item.player_name.c_str( ) );
				}
			}
		}

		inline void world_cache( sdk::c_base_entity* base_networkable ) {

			const auto& prefab = base_networkable->prefab( );
			if ( !memory::is_valid( prefab ) )
				return;

			const auto& hash = RHASH( base_networkable->class_name( ) );
			const auto& prefab_hash = RHASH( prefab->string( ).c_str( ) );

			if ( !sdk::entity_types_t::has( hash, prefab_hash ) )
				return;

			const auto& entity_type = sdk::entity_types_t::get( hash, prefab_hash );
			if ( !entity_type.state( ) )
				return;

			const auto& transform = base_networkable->transform( );
			if ( !memory::is_valid( transform ) )
				return;

			sdk::world_item_t world_item;

			if ( entity_type.name )
				world_item.name = entity_type.name( );

			world_item.color = entity_type.current_color( );
			world_item.bounds = base_networkable->bounds( );
			world_item.position = transform->position( );
			world_item.rotation = transform->rotation( );

			const auto& distance = sdk::local_position.distance_to( world_item.position );

			if ( config::visuals_entities_ore && sdk::entity_types_t::contains( sdk::ores, hash, prefab_hash ) && distance < config::visuals_entities_ore_max_distance )
				return sdk::info.world_items.push_back( world_item );

			if ( config::visuals_entities_food && sdk::entity_types_t::contains( sdk::foods, hash, prefab_hash ) && distance < config::visuals_entities_food_max_distance )
				return sdk::info.world_items.push_back( world_item );

			if ( config::visuals_entities_trap && sdk::entity_types_t::contains( sdk::traps, hash, prefab_hash ) && distance < config::visuals_entities_trap_max_distance )
				return sdk::info.world_items.push_back( world_item );

			if ( config::visuals_entities_crate && sdk::entity_types_t::contains( sdk::crates, hash, prefab_hash ) && distance < config::visuals_entities_crate_max_distance )
				return sdk::info.world_items.push_back( world_item );

			if ( config::visuals_entities_berry && sdk::entity_types_t::contains( sdk::berries, hash, prefab_hash ) && distance < config::visuals_entities_berry_max_distance )
				return sdk::info.world_items.push_back( world_item );

			if ( config::visuals_entities_barrel && sdk::entity_types_t::contains( sdk::barrels, hash, prefab_hash ) && distance < config::visuals_entities_barrel_max_distance )
				return sdk::info.world_items.push_back( world_item );

			if ( config::visuals_entities_animal && sdk::entity_types_t::contains( sdk::animals, hash, prefab_hash ) && distance < config::visuals_entities_animal_max_distance )
				return sdk::info.world_items.push_back( world_item );

			if ( config::visuals_entities_vehicle && sdk::entity_types_t::contains( sdk::vehicles, hash, prefab_hash ) && distance < config::visuals_entities_vehicle_max_distance )
				return sdk::info.world_items.push_back( world_item );

			if ( config::visuals_entities_collectible && sdk::entity_types_t::contains( sdk::collectibles, hash, prefab_hash ) && distance < config::visuals_entities_collectible_max_distance )
				return sdk::info.world_items.push_back( world_item );

			if ( config::visuals_entities_vehicle_water && sdk::entity_types_t::contains( sdk::vehicles_water, hash, prefab_hash ) && distance < config::visuals_entities_vehicle_water_max_distance )
				return sdk::info.world_items.push_back( world_item );

			if ( config::visuals_entities_loot && distance < config::visuals_entities_loot_max_distance && 
				 sdk::entity_types_t::contains( sdk::loots, hash, prefab_hash ) ) {
				if ( base_networkable->is_class( _( "StashContainer" ) ) ) {
					const auto& stash_container = mem::read_chain< sdk::c_stash_container* >( base_networkable, { 0x10, 0x28 } );
					if ( !memory::is_valid( stash_container ) )
						return;

					if ( !stash_container->is_hidden( ) )
						return;

					return sdk::info.world_items.push_back( world_item );
				}

				if ( base_networkable->is_class( _( "DroppedItemContainer" ) ) ) {
					const auto& dropped_item_container = mem::read_chain< sdk::c_dropped_item_container* >( base_networkable, { 0x10, 0x28 } );
					if ( !memory::is_valid( dropped_item_container ) )
						return;

					if ( dropped_item_container->player_steam_id( ) == 0 )
						return;

					const auto& player_name = dropped_item_container->player_name( );
					if ( !memory::is_valid( player_name ) )
						return;

					world_item.player_name = player_name->string( );
					world_item.is_corpse = true;
					return sdk::info.world_items.push_back( world_item );
				}

				if ( base_networkable->is_class( _( "PlayerCorpse" ) ) ) {
					const auto& lootable_corpse = mem::read_chain< sdk::c_lootable_corpse* >( base_networkable, { 0x10, 0x28 } );
					if ( !memory::is_valid( lootable_corpse ) )
						return;

					const auto& player_name = lootable_corpse->player_name( );
					if ( !memory::is_valid( player_name ) )
						return;

					world_item.player_name = player_name->string( );
					world_item.is_corpse = true;
					return sdk::info.world_items.push_back( world_item );
				}

				return sdk::info.world_items.push_back( world_item );
			}

			if ( config::visuals_entities_construction && distance < config::visuals_entities_construction_max_distance && 
				 sdk::entity_types_t::contains( sdk::constructions, hash, prefab_hash ) ) {
				
				if ( base_networkable->is_class( _( "Workbench" ) ) ) {
					const auto& workbench = mem::read_chain< sdk::c_workbench* >( base_networkable, { 0x10, 0x28 } );
					if ( !memory::is_valid( workbench ) )
						return;

					const auto& workbench_level = workbench->level( );
					if ( ( workbench_level == 1 && !config::visuals_entities_construction_workbench_1 ) ||
						 ( workbench_level == 2 && !config::visuals_entities_construction_workbench_2 ) ||
						 ( workbench_level == 3 && !config::visuals_entities_construction_workbench_3 ) )
						return;

					switch ( workbench_level ) {
					case 1:
						world_item.name = REPLACE_TEXT( "Workbench 1", "工作台1", "Первый верстак" );
						world_item.color = config::visuals_entities_construction_workbench_1_color;
						break;
					case 2:
						world_item.name = REPLACE_TEXT( "Workbench 2", "工作台2", "Второй верстак" );
						world_item.color = config::visuals_entities_construction_workbench_2_color;
						break;
					case 3:
						world_item.name = REPLACE_TEXT( "Workbench 3", "工作台3", "Третий верстак" );
						world_item.color = config::visuals_entities_construction_workbench_3_color;
						break;
					}

					return sdk::info.world_items.push_back( world_item );
				}

				return sdk::info.world_items.push_back( world_item );
			}

			if ( config::visuals_entities_dropped_item && distance < config::visuals_entities_dropped_item_max_distance && 
				 base_networkable->is_class( _( "DroppedItem" ) ) ) {
				const auto& dropped_item = mem::read_chain< sdk::c_world_item* >( base_networkable, { 0x10, 0x28 } );
				if ( !memory::is_valid( dropped_item ) )
					return;

				const auto& item = dropped_item->item( );
				if ( !memory::is_valid( item ) )
					return;

				const auto& info = item->info( );
				if ( !memory::is_valid( info ) )
					return;

				const auto& category = info->category( );

				const auto& dropped_entity = sdk::dropped_items[ category ];
				if ( !dropped_entity.state( ) )
					return;

				const auto& name = info->display_name( )->name( );
				if ( memory::is_valid( name ) )
					world_item.name = name->string( );

				world_item.color = dropped_entity.current_color( );
				world_item.amount = item->amount( );
				world_item.is_dropped_item = true;

				if ( config::visuals_entities_dropped_item_icons ) {
					const auto& sprite = item->icon_sprite( );
					if ( memory::is_valid( sprite ) ) {
						const auto& texture = sprite->texture( );
						if ( memory::is_valid( texture ) ) {
							const auto& srv = texture->to_srv( texture->texture_id( ) );
							if ( memory::is_valid( srv ) )
								world_item.image = srv;
						}
					}
				}

				if ( config::visuals_entities_dropped_item_chams ) {
					const auto& renderers = dropped_item->components_in_children< unity::c_renderer* >( 
						il2cpp::type_object( _( "UnityEngine" ), _( "Renderer" ) ) );
					if ( memory::is_valid( renderers ) ) {
						for ( auto i = 0; i < renderers->count( ); ++i ) {
							const auto& renderer = renderers->get( i );
							if ( !memory::is_valid( renderer ) )
								continue;

							const auto& material = renderer->material( );
							if ( !memory::is_valid( material ) )
								continue;

							const auto& shader = unity::c_shader::find( _( "Hidden/Internal-Colored" ) );
							if ( !memory::is_valid( shader ) )
								continue;

							if ( material->shader( ) != shader )
								material->set_shader( shader );

							material->set_int( _( "_ZTest" ), 8 );
							material->set_color( _( "_Color" ), unity::c_color( world_item.color.r * 255.f,
																				world_item.color.g * 255.f,
																				world_item.color.b * 255.f,
																				world_item.color.a * 255.f ) );
						}
					}
				}

				return sdk::info.world_items.push_back( world_item );
			}
		}

		inline void entities_loop( ) {
			const auto& client_entities = sdk::c_base_networkable::client_entities( );
			if ( !memory::is_valid( client_entities ) ) {
				sdk::clear( );
				return;
			}

			const auto& entity_list = client_entities->entity_list( );
			if ( !memory::is_valid( entity_list ) ) {
				sdk::clear( );
				return;
			}

			const auto& count = entity_list->count( );
			if ( count <= 0 ) {
				sdk::clear( );
				return;
			}

			bool update_world_cache = false;
			if ( execute_delay( 1000 ) ) {
				sdk::info.world_items.clear( );

				update_world_cache = true;
			}

			sdk::info.players.clear( );

			sdk::local_player = sdk::c_local_player::entity( );
			if ( !memory::is_valid( sdk::local_player ) )
				return;

			for ( auto i = 0; i < count; ++i ) {
				const auto& base_networkable = entity_list->get< sdk::c_base_entity* >( i );
				if ( !memory::is_valid( base_networkable ) )
					continue;

				const auto& base_object = mem::read< uintptr_t >( base_networkable + 0x10 );
				if ( !memory::is_valid( base_object ) )
					continue;

				const auto& object = mem::read< uintptr_t >( base_object + 0x30 );
				if ( !memory::is_valid( object ) )
					continue;

				const auto& tag = mem::read< uint16_t >( object + 0x54 );
				if ( tag == 6 ) {
					const auto& player = mem::read< sdk::c_base_player* >( base_object + 0x28 );
					if ( !memory::is_valid( player ) )
						continue;

					if ( player->lifestate( ) || player->is_local_player( ) )
						continue;

					sdk::info.players.push_back( player );
				} else if ( update_world_cache ) {
					world_cache( base_networkable );
				}
			}

			if ( update_world_cache )
				update_world_cache = false;
		}

		inline void loop( ) {
			entities_loop( );

			if ( !memory::is_valid( sdk::local_player ) )
				return;

			features::misc.on_some_misc( );

			if ( !sdk::c_map_interface::is_open( ) ) {

				if ( !sdk::c_ui_inventory::is_open( ) ) {
					render_screen( );
					render_hotbar( );
				}

				for ( auto&& player : sdk::info.players )
					render_player( player );

				for ( auto&& world_item : sdk::info.world_items )
					render_world_item( world_item );
			}/* else {
				render_map( );
			}*/
		}
	};

	auto& visuals = c_visuals::instance( );
}
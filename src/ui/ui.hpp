#pragma once
#include "framework.hpp"

namespace ui {
    static bool enabled = true;

    static float tab_add;
    static int active_tab = 0;
    static float tab_alpha = 0.f;

    unsigned long picker_flags = ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_DisplayHex;

    void draw_aimbot_tab( const ImVec2& spacing ) {
        ImGui::BeginGroup( );
        {
            edited::BeginChild( REPLACE_TEXT( "Main", "主要-木", "Основное" ), ImVec2( c::bg::size.x - ( 100 + spacing.x * 3 ), 0 ) / 2 );
            {
                edited::CheckboxClicked( REPLACE_TEXT( "Silent aim", "启用", "Тихий аимбот" ), &config::aimbot.value );
                edited::Keybind( REPLACE_TEXT( "Silent key", "自动射击", "Авто стрельба" ), &config::aimbot_key.value );

                edited::Checkbox( REPLACE_TEXT( "Silent melee", "静默追踪", "Тихий ближний бой" ), &config::aimbot_silent_melee.value );
                edited::Checkbox( REPLACE_TEXT( "Melee silent aim", "无声近战", "Ближний тихий аимбот" ), &config::aimbot_melee.value );
            }
            edited::EndChild( );

            edited::BeginChild( REPLACE_TEXT( "Rage", "瞄准-暴力", "Рейдж" ), ImVec2( c::bg::size.x - ( 100 + spacing.x * 3 ), 0 ) / 2 );
            {
                edited::Checkbox( REPLACE_TEXT( "No sway", "没有摇摆", "Без тряски" ), &config::aimbot_no_sway.value );

                edited::Checkbox( REPLACE_TEXT( "No recoil", "无后座力", "Нет отдачи" ), &config::aimbot_no_recoil.value );
                edited::SliderFloat( REPLACE_TEXT( "Recoil value", "无后座力", "Нет отдачи" ), &config::aimbot_no_recoil_value.value, 0.0f, 100.f, _( "%.1f" ) );

                edited::Checkbox( REPLACE_TEXT( "No spread", "子弹无扩散", "Нет разброса" ), &config::aimbot_no_spread.value );
                edited::SliderFloat( REPLACE_TEXT( "Spread value", "无后座力", "Нет отдачи" ), &config::aimbot_no_spread_value.value, 0.0f, 100.f, _( "%.1f" ) );

                edited::Checkbox( REPLACE_TEXT( "Big bullet", "大子弹", "Большая пуля" ), &config::aimbot_big_bullet.value );

                edited::Checkbox( REPLACE_TEXT( "Auto shoot", "自动射击", "Авто стрельба" ), &config::aimbot_autoshoot.value );
                edited::Keybind( REPLACE_TEXT( "Shoot key", "自动射击", "Авто стрельба" ), &config::aimbot_autoshoot_key.value );

                edited::Checkbox( REPLACE_TEXT( "Manipulation", "子弹拐弯", "Манипулятор" ), &config::aimbot_manipulation.value );
                edited::Keybind( REPLACE_TEXT( "Manipulation key", "自动射击", "Авто стрельба" ), &config::aimbot_manipulation_key.value );

                edited::Checkbox( REPLACE_TEXT( "Instant bullet", "瞬发", "Моментальная пуля" ), &config::aimbot_instant_bullet.value );

                edited::Checkbox( REPLACE_TEXT( "Thickness bullet", "厚子弹", "Толстая пуля" ), &config::aimbot_thickness.value );
                edited::SliderFloat( REPLACE_TEXT( "Thickness value", "无后座力", "Нет отдачи" ), &config::aimbot_thickness_value.value, 0.0f, 2.f, _( "%.1f" ) );

                edited::Checkbox( REPLACE_TEXT( "Fake hit distance", "假击距离", "Ложная дистация" ), &config::aimbot_fake_hit_distance.value );
                edited::SliderFloat( REPLACE_TEXT( "Fake distance", "无后座力", "Нет отдачи" ), &config::aimbot_hit_distance.value, 0.0f, 1000.f, _( "%.1f" ) );
            }
            edited::EndChild( );
        }
        ImGui::EndGroup( );

        ImGui::SameLine( );

        ImGui::BeginGroup( );
        {
            edited::BeginChild( REPLACE_TEXT( "Visuals", "视觉-木", "Визуалы" ), ImVec2( c::bg::size.x - ( 100 + spacing.x * 3 ), 0 ) / 2 );
            {
                edited::CheckboxPicker( REPLACE_TEXT( "Crosshair", "瞄准线", "Прицел" ), &config::aimbot_crosshair.value, config::aimbot_crosshair_color.value.to_array( ), picker_flags );
                edited::CheckboxPicker( REPLACE_TEXT( "Fov circle", "命中圈", "Круг обзора" ), &config::aimbot_fov.value, config::aimbot_fov_color.value.to_array( ), picker_flags );
                edited::Checkbox( REPLACE_TEXT( "Hit effects", "无法瞄准队友", "Эффекты попаданий" ), &config::aimbot_hit_effects.value );
            }
            edited::EndChild( );

            edited::BeginChild( REPLACE_TEXT( "Settings", "设置", "Настройки" ), ImVec2( c::bg::size.x - ( 100 + spacing.x * 3 ), 0 ) / 2 );
            {
                edited::Checkbox( REPLACE_TEXT( "Ignore team", "穿透队友", "Игнор тимейтов" ), &config::aimbot_ignore_team.value );
                edited::Checkbox( REPLACE_TEXT( "Visible check", "可见检查", "Проверка видимости" ), &config::aimbot_visible_check.value );

                edited::Checkbox( REPLACE_TEXT( "Hit chance", "命中机会", "Шанс попадания" ), &config::aimbot_hit_chance.value );
                edited::SliderInt( REPLACE_TEXT( "Hit chance value", "命中机会", "Шанс попадания" ), &config::aimbot_hit_chance_value.value, 0, 100 );

                edited::Checkbox( REPLACE_TEXT( "Hitbox override", "更改时间", "Hitbox override" ), &config::aimbot_hitbox_override.value );

                edited::Combo( REPLACE_TEXT( "Hitboxes", "更改时间", "Hitbox override" ), &config::aimbot_hitbox_override_value.value, {
                    REPLACE_TEXT( "Head", "头", "Голова" ),
                    REPLACE_TEXT( "Pelvis", "骨盆", "Pelvis" ),
                    REPLACE_TEXT( "Random", "随机的", "Случайная" )
                               }, 3 );

                edited::Combo( REPLACE_TEXT( "Bone", "骨", "Кость" ), &config::aimbot_bone.value, {
                    REPLACE_TEXT( "Head", "头", "Голова" ),
                    REPLACE_TEXT( "Neck", "脖子", "Шея" ),
                    REPLACE_TEXT( "Chest", "胸部", "Туловище" ),
                    REPLACE_TEXT( "Random", "随机的", "Случайная" )
                               }, 3 );
                edited::SliderFloat( REPLACE_TEXT( "Fov", "视场", "Обзор" ), &config::aimbot_fov_value.value, 0.f, 360.f, reinterpret_cast< const char* >( _( u8"%.f °" ) ) );
            }
            edited::EndChild( );
        }
        ImGui::EndGroup( );
    }

    void draw_visuals_tab( const ImVec2& spacing ) {
        ImGui::BeginGroup( );
        {
            edited::BeginChild( REPLACE_TEXT( "Sky", "视觉-天空", "Небо" ), ImVec2( c::bg::size.x - ( 100 + spacing.x * 3 ), 0 ) / 2 );
            {
                edited::Checkbox( REPLACE_TEXT( "Bright cave", "明亮洞穴", "Яркая пещера" ), &config::visuals_sky_bright_cave.value );
                edited::Checkbox( REPLACE_TEXT( "Bright night", "明亮夜晚", "Яркая ночь" ), &config::visuals_sky_bright_night.value );
            }
            edited::EndChild( );

            edited::BeginChild( REPLACE_TEXT( "Time", "视觉-时间", "Время" ), ImVec2( c::bg::size.x - ( 100 + spacing.x * 3 ), 0 ) / 2 );
            {
                edited::Checkbox( REPLACE_TEXT( "Change time", "更改时间", "Сменить время" ), &config::visuals_time_change_time.value );
                edited::SliderInt( REPLACE_TEXT( "Time", "更改时间", "Время" ), &config::visuals_time_change_time_value.value, 0, 24 );

                edited::Checkbox( REPLACE_TEXT( "Show server time", "显示服务器时间", "Показывать серверное время" ), &config::visuals_time_show_server_time.value );
            }
            edited::EndChild( );

            edited::BeginChild( REPLACE_TEXT( "Weather", "视觉-天气", "Погода" ), ImVec2( c::bg::size.x - ( 100 + spacing.x * 3 ), 0 ) / 2 );
            {
                edited::Checkbox( REPLACE_TEXT( "No sun", "无阳光", "Нет солнца" ), &config::visuals_weather_no_sun.value );
                edited::Checkbox( REPLACE_TEXT( "No fog", "无雾", "Нет тумана" ), &config::visuals_weather_no_fog.value );
                edited::Checkbox( REPLACE_TEXT( "No rain", "无雨", "Нет дождя" ), &config::visuals_weather_no_rain.value );
                edited::Checkbox( REPLACE_TEXT( "No wind", "无风", "Нет ветра" ), &config::visuals_weather_no_wind.value );
                edited::Checkbox( REPLACE_TEXT( "Rainbows", "没有彩虹", "Радуга" ), &config::visuals_weather_rainbows.value );
                edited::Checkbox( REPLACE_TEXT( "No clouds", "无云", "Нет облаков" ), &config::visuals_weather_no_clouds.value );
                edited::Checkbox( REPLACE_TEXT( "No thunder", "没有雷声", "Нет грома" ), &config::visuals_weather_no_thunder.value );
                edited::Checkbox( REPLACE_TEXT( "Atmosphere contrast", "无大气层", "Атмосферный контраст" ), &config::visuals_weather_atmosphere.value );
            }
            edited::EndChild( );
        }
        ImGui::EndGroup( );

        ImGui::SameLine( );

        ImGui::BeginGroup( );
        {
            edited::BeginChild( REPLACE_TEXT( "Players", "视觉-人物", "Игроки" ), ImVec2( c::bg::size.x - ( 100 + spacing.x * 3 ), 0 ) / 2 );
            {
                edited::Checkbox( REPLACE_TEXT( "On", "启用", "Вкл/выкл" ), &config::visuals_players.value );

                edited::CheckboxPicker( REPLACE_TEXT( "NPC", "机器人", "Боты" ), &config::visuals_players_npc.value, config::visuals_players_npc_color.value.to_array( ), picker_flags );

                edited::Checkbox( REPLACE_TEXT( "Chams", "颜色", "Чамсы" ), &config::visuals_players_chams.value );

                edited::Combo( REPLACE_TEXT( "Chams", "颜色", "Чамсы" ), &config::visuals_players_chams_value.value, {
                    REPLACE_TEXT( "Flat", "平坦的", "Плоский" ),
                    REPLACE_TEXT( "Glitter", "闪光", "Блеск" ),
                    REPLACE_TEXT( "Damascus", "大马士革", "Дамаск" ),
                    REPLACE_TEXT( "Lightning", "闪电", "Молния" ),
                    REPLACE_TEXT( "Wire frame", "线框", "Проволочный каркас" ),
                    REPLACE_TEXT( "Night fire [Red]", "夜火【红】", "Ночной огонь [Красный]" ),
                    REPLACE_TEXT( "Night fire [Blue]", "夜火【蓝色】", "Ночной огонь [Синий]" ),
                    REPLACE_TEXT( "Geometric dissolve", "几何溶解", "Геометрическое растворение" )
                               }, 3 );

                edited::CheckboxPicker( REPLACE_TEXT( "Teammate", "队友", "Тимейты" ), &config::visuals_players_teammate.value, config::visuals_players_teammate_color.value.to_array( ), picker_flags );
                edited::CheckboxPicker( REPLACE_TEXT( "Sleepers", "睡觉者", "Спящие" ), &config::visuals_players_sleepers.value, config::visuals_players_sleepers_color.value.to_array( ), picker_flags );
                edited::CheckboxPicker( REPLACE_TEXT( "Visible check", "可见检查", "Проверка видимости" ), &config::visuals_players_visible_check.value, config::visuals_players_visible_check_color.value.to_array( ), picker_flags );

                bool* flags_values[] = {
                    &config::visuals_players_box.value,
                    &config::visuals_players_name.value,
                    &config::visuals_players_hotbar.value,
                    &config::visuals_players_weapon.value,
                    &config::visuals_players_skeleton.value,
                    &config::visuals_players_distance.value
                };

                edited::MultiCombo( REPLACE_TEXT( "Flags", "旗帜", "Флаги" ), flags_values, {
                    REPLACE_TEXT( "Box", "盒子", "Квадрат" ),
                    REPLACE_TEXT( "Name", "姓名", "Имя" ),
                    REPLACE_TEXT( "Hotbar", "热键栏", "Панель слотов" ),
                    REPLACE_TEXT( "Weapon", "武器", "Оружие" ),
                    REPLACE_TEXT( "Skeleton", "骨骼", "Скелет" ),
                    REPLACE_TEXT( "Distance", "距离", "Дистанция" ),
                                    } );
                edited::SliderFloat( REPLACE_TEXT( "Max distance", "最大距离", "Максимальная дистанция" ), &config::visuals_players_max_distance.value, 0.f, 500.f, _( "%.f m" ) );
                edited::SliderFloat( REPLACE_TEXT( "Npc max distance", "Npc最大距离", "Максимальная дистанция ботов" ), &config::visuals_players_npc_max_distance.value, 0.f, 500.f, _( "%.f m" ) );
            }
            edited::EndChild( );

            edited::BeginChild( REPLACE_TEXT( "Entities", "世界", "Объекты" ), ImVec2( c::bg::size.x - ( 100 + spacing.x * 3 ), 0 ) / 2 );
            {
                static int type = 0;
                edited::Combo( REPLACE_TEXT( "Type", "类型", "Тип" ), &type, {
                    REPLACE_TEXT( "Ore", "矿石", "Руда" ),
                    REPLACE_TEXT( "Loot", "掠夺", "Лут" ),
                    REPLACE_TEXT( "Food", "食物", "Еда" ),
                    REPLACE_TEXT( "Trap", "陷阱", "Ловушки" ),
                    REPLACE_TEXT( "Berry", "莓果", "Ягоды" ),
                    REPLACE_TEXT( "Crate", "箱", "Ящики" ),
                    REPLACE_TEXT( "Barrel", "桶", "Бочки" ),
                    REPLACE_TEXT( "Animal", "动物", "Животные" ),
                    REPLACE_TEXT( "Vehicle", "车辆", "Транспорт" ),
                    REPLACE_TEXT( "TC", "领地柜", "Шкаф" ),
                    REPLACE_TEXT( "Collectible", "收藏品", "Собираемое" ),
                    REPLACE_TEXT( "Construction", "建造", "Конструкции" ),
                    REPLACE_TEXT( "Dropped item", "掉落的物品", "Предметы на земле" ),
                    REPLACE_TEXT( "Vehicle water", "车辆水", "Водный транспорт" ),
                               } );

                switch ( type ) {
                    case 0: {
                        edited::Checkbox( REPLACE_TEXT( "On", "启用", "Вкл/выкл" ), &config::visuals_entities_ore.value );

                        bool* ore_flags[ ] = {
                            &config::visuals_entities_ore_stone.value,
                            &config::visuals_entities_ore_metal.value,
                            &config::visuals_entities_ore_sulfur.value
                        };

                        edited::MultiCombo( REPLACE_TEXT( "Flags", "旗帜", "Флаги" ), ore_flags, {
                            REPLACE_TEXT( "Stone", "石头矿", "Камень" ),
                            REPLACE_TEXT( "Metal", "金属矿", "Металл" ),
                            REPLACE_TEXT( "Sulfur", "硫黄矿", "Сера" )
                                            } );
                        edited::SliderFloat( REPLACE_TEXT( "Max distance", "最大距离", "Максимальная дистанция" ), &config::visuals_entities_ore_max_distance.value, 0.f, 400.f, _( "%.f m" ) );
                        break;
                    }

                    case 1: {
                        edited::Checkbox( REPLACE_TEXT( "On", "启用", "Вкл/выкл" ), &config::visuals_entities_loot.value );

                        bool* loot_flags[ ] = {
                            &config::visuals_entities_loot_corpse.value,
                            &config::visuals_entities_loot_backpack.value,
                            &config::visuals_entities_loot_small_stash.value,
                            &config::visuals_entities_loot_supply_drop.value
                        };

                        edited::MultiCombo( REPLACE_TEXT( "Flags", "旗帜", "Флаги" ), loot_flags, {
                            REPLACE_TEXT( "Corpse", "尸体", "Труп" ),
                            REPLACE_TEXT( "Backpack", "背包", "Рюкзак" ),
                            REPLACE_TEXT( "Small stash", "小藏匿", "Тайник" ),
                            REPLACE_TEXT( "Supply drop", "掉落物", "Аирдроп" )
                                            } );
                        edited::SliderFloat( REPLACE_TEXT( "Max distance", "最大距离", "Максимальная дистанция" ), &config::visuals_entities_loot_max_distance.value, 0.f, 400.f, _( "%.f m" ) );
                        break;
                    }

                    case 2: {
                        edited::Checkbox( REPLACE_TEXT( "On", "启用", "Вкл/выкл" ), &config::visuals_entities_food.value );

                        bool* food_flags[ ] = {
                            &config::visuals_entities_food_corn.value,
                            &config::visuals_entities_food_potato.value,
                            &config::visuals_entities_food_pumpkin.value,
                            &config::visuals_entities_food_mushroom.value
                        };

                        edited::MultiCombo( REPLACE_TEXT( "Flags", "旗幟", "Флаги" ), food_flags, {
                            REPLACE_TEXT( "Corn", "玉米", "Кукуруза" ),
                            REPLACE_TEXT( "Potato", "土豆", "Картошка" ),
                            REPLACE_TEXT( "Pumpkin", "南瓜", "Тыква" ),
                            REPLACE_TEXT( "Mushroom", "蘑菇", "Гриб" ),
                                            } );
                        edited::SliderFloat( REPLACE_TEXT( "Max distance", "最大距离", "Максимальная дистанция" ), &config::visuals_entities_food_max_distance.value, 0.f, 400.f, _( "%.f m" ) );
                        break;
                    }

                    case 3: {
                        edited::Checkbox( REPLACE_TEXT( "On", "启用", "Вкл/выкл" ), &config::visuals_entities_trap.value );

                        bool* trap_flags[ ] = {
                            &config::visuals_entities_trap_gun.value,
                            &config::visuals_entities_trap_bear.value,
                            &config::visuals_entities_trap_landmine.value,
                            &config::visuals_entities_trap_sam_site.value,
                            &config::visuals_entities_trap_auto_turret.value,
                            &config::visuals_entities_trap_flame_turret.value,
                        };

                        edited::MultiCombo( REPLACE_TEXT( "Flags", "旗帜", "Флаги" ), trap_flags, {
                            REPLACE_TEXT( "GunTrap", "喷子炮台", "Ловушка с дробовиком" ),
                            REPLACE_TEXT( "Bear", "捕兽夹", "Капкан" ),
                            REPLACE_TEXT( "Landmine", "地雷", "Мина" ),
                            REPLACE_TEXT( "Sam site", "防空炮", "ПВО" ),
                            REPLACE_TEXT( "Auto turret", "自动炮台", "Автоматическая турель" ),
                            REPLACE_TEXT( "Flame turret", "火焰炮台", "Огненная турель" ),
                                            } );
                        edited::SliderFloat( REPLACE_TEXT( "Max distance", "最大距离", "Максимальная дистанция" ), &config::visuals_entities_trap_max_distance.value, 0.f, 400.f, _( "%.f m" ) );
                        break;
                    }

                    case 4: {
                        edited::Checkbox( REPLACE_TEXT( "On", "启用", "Вкл/выкл" ), &config::visuals_entities_berry.value );

                        bool* berry_flags[ ] = {
                            &config::visuals_entities_berry_red.value,
                            &config::visuals_entities_berry_blue.value,
                            &config::visuals_entities_berry_black.value,
                            &config::visuals_entities_berry_green.value,
                            &config::visuals_entities_berry_white.value,
                            &config::visuals_entities_berry_yellow.value,
                        };

                        edited::MultiCombo( REPLACE_TEXT( "Flags", "旗帜", "Флаги" ), berry_flags, {
                            REPLACE_TEXT( "Red", "紅色浆果", "Красная" ),
                            REPLACE_TEXT( "Blue", "藍色浆果", "Голубая" ),
                            REPLACE_TEXT( "Black", "黑色浆果", "Черная" ),
                            REPLACE_TEXT( "Green", "綠色浆果", "Зеленая" ),
                            REPLACE_TEXT( "White", "白色浆果", "Белая" ),
                            REPLACE_TEXT( "Yellow", "黃色浆果", "Желтая" ),
                                            } );
                        edited::SliderFloat( REPLACE_TEXT( "Max distance", "最大距离", "Максимальная дистанция" ), &config::visuals_entities_berry_max_distance.value, 0.f, 400.f, _( "%.f m" ) );
                        break;
                    }

                    case 5: {
                        edited::Checkbox( REPLACE_TEXT( "On", "启用", "Вкл/выкл" ), &config::visuals_entities_crate.value );

                        bool* crate_flags[ ] = {
                            &config::visuals_entities_crate_fuel.value,
                            &config::visuals_entities_crate_food.value,
                            &config::visuals_entities_crate_mine.value,
                            &config::visuals_entities_crate_tools.value,
                            &config::visuals_entities_crate_elite.value,
                            &config::visuals_entities_crate_basic.value,
                            &config::visuals_entities_crate_normal.value,
                            &config::visuals_entities_crate_medical.value,
                            &config::visuals_entities_crate_military.value,
                            &config::visuals_entities_crate_ammunition.value,
                            &config::visuals_entities_crate_underwater_basic.value,
                            &config::visuals_entities_crate_underwater_advanced.value,
                        };

                        edited::MultiCombo( REPLACE_TEXT( "Flags", "旗帜", "Флаги" ), crate_flags, {
                            REPLACE_TEXT( "Fue", "燃料箱子", "Топливо" ),
                            REPLACE_TEXT( "Food", "食物盒子", "Еда" ),
                            REPLACE_TEXT( "Mine", "Mine", "Шахта" ),
                            REPLACE_TEXT( "Tools", "工具箱", "Инструменты" ),
                            REPLACE_TEXT( "Elite", "精英箱子", "Элитный" ),
                            REPLACE_TEXT( "Basic", "基础板条箱", "Базовый" ),
                            REPLACE_TEXT( "Normal", "普通箱子", "Обычный" ),
                            REPLACE_TEXT( "Medical", "医疗箱子", "Медицинский" ),
                            REPLACE_TEXT( "Military", "军事箱", "Военный" ),
                            REPLACE_TEXT( "Ammunition", "弹药箱", "Боеприпасы" ),
                            REPLACE_TEXT( "Underwater basic", "水下小箱子", "Подводный базовый" ),
                            REPLACE_TEXT( "Underwater advanced", "水下大箱子", "Подводный продвинутый" ),
                                            } );
                        edited::SliderFloat( REPLACE_TEXT( "Max distance", "最大距离", "Максимальная дистанция" ), &config::visuals_entities_crate_max_distance.value, 0.f, 400.f, _( "%.f m" ) );
                        break;
                    }

                    case 6: {
                        edited::Checkbox( REPLACE_TEXT( "On", "启用", "Вкл/выкл" ), &config::visuals_entities_barrel.value );

                        bool* barrel_flags[ ] = {
                            &config::visuals_entities_barrel_oil.value,
                            &config::visuals_entities_barrel_blue.value,
                            &config::visuals_entities_barrel_basic.value
                        };

                        edited::MultiCombo( REPLACE_TEXT( "Flags", "旗帜", "Флаги" ), barrel_flags, {
                            REPLACE_TEXT( "Oil", "原油桶", "Нефть" ),
                            REPLACE_TEXT( "Blue", "藍色", "Голубой" ),
                            REPLACE_TEXT( "Basic", "普通桶", "Базовый" ),
                                            } );
                        edited::SliderFloat( REPLACE_TEXT( "Max distance", "最大距离", "Максимальная дистанция" ), &config::visuals_entities_barrel_max_distance.value, 0.f, 400.f, _( "%.f m" ) );
                        break;
                    }

                    case 7: {
                        edited::Checkbox( REPLACE_TEXT( "On", "启用", "Вкл/выкл" ), &config::visuals_entities_animal.value );

                        bool* animal_flags[ ] = {
                            &config::visuals_entities_animal_stag.value,
                            &config::visuals_entities_animal_boar.value,
                            &config::visuals_entities_animal_bear.value,
                            &config::visuals_entities_animal_wolf.value,
                            &config::visuals_entities_animal_polar_bear.value
                        };

                        edited::MultiCombo( REPLACE_TEXT( "Flags", "旗幟", "Флаги" ), animal_flags, {
                            REPLACE_TEXT( "Stag", "鹿", "Олень" ),
                            REPLACE_TEXT( "Boar", "猪", "Кабан" ),
                            REPLACE_TEXT( "Bear", "熊", "Медведь" ),
                            REPLACE_TEXT( "Wolf", "狼", "Волк" ),
                            REPLACE_TEXT( "Polar bear", "北极熊", "Полярный медведь" ),
                                            } );
                        edited::SliderFloat( REPLACE_TEXT( "Max distance", "最大距离", "Максимальная дистанция" ), &config::visuals_entities_animal_max_distance.value, 0.f, 400.f, _( "%.f m" ) );
                        break;
                    }

                    case 8: {
                        edited::Checkbox( REPLACE_TEXT( "On", "启用", "Вкл/выкл" ), &config::visuals_entities_vehicle.value );

                        bool* vehicle_flags[ ] = {
                            &config::visuals_entities_vehicle_base.value,
                            &config::visuals_entities_vehicle_horse.value,
                            &config::visuals_entities_vehicle_bradley_apc.value,
                            &config::visuals_entities_vehicle_scrap_heli.value,
                            &config::visuals_entities_vehicle_mini_copter.value,
                            &config::visuals_entities_vehicle_air_baloon.value
                        };

                        edited::MultiCombo( REPLACE_TEXT( "Flags", "旗帜", "Флаги" ), vehicle_flags, {
                            REPLACE_TEXT( "Base", "车辆", "Базовый" ),
                            REPLACE_TEXT( "Horse", "马", "Лошадь" ),
                            REPLACE_TEXT( "Bradley", "坦克", "Танк" ),
                            REPLACE_TEXT( "Scrap heli", "废料直升机", "Вертолет" ),
                            REPLACE_TEXT( "Helicopter", "直升机", "Боевой вертолет" ),
                            REPLACE_TEXT( "Minicopter", "迷你直升机", "Коптер" ),
                            REPLACE_TEXT( "Air balloon", "热气球", "Воздушный шар" ),
                                            } );
                        edited::SliderFloat( REPLACE_TEXT( "Max distance", "最大距离", "Максимальная дистанция" ), &config::visuals_entities_vehicle_max_distance.value, 0.f, 400.f, _( "%.f m" ) );
                        break;
                    }

                    case 9: {
                        edited::CheckboxPicker( REPLACE_TEXT( "On", "启用", "Вкл/выкл" ), &config::visuals_entities_cupboard.value, config::visuals_entities_cupboard_color.value.to_array( ), picker_flags );

                        edited::Checkbox( REPLACE_TEXT( "Authorized", "授权玩家", "Авторизованные" ), &config::visuals_entities_cupboard_authorized.value );
                        edited::SliderFloat( REPLACE_TEXT( "Max distance", "最大距离", "Максимальная дистанция" ), &config::visuals_entities_cupboard_max_distance.value, 0.f, 400.f, _( "%.f m" ) );
                        break;
                    }
                    case 10: {
                        edited::Checkbox( REPLACE_TEXT( "On", "启用", "Вкл/выкл" ), &config::visuals_entities_collectible.value );

                        bool* collectible_flags[ ] = {
                            &config::visuals_entities_collectible_hemp.value,
                            &config::visuals_entities_collectible_wood.value,
                            &config::visuals_entities_collectible_stone.value,
                            &config::visuals_entities_collectible_metal.value,
                            &config::visuals_entities_collectible_sulfur.value,
                            &config::visuals_entities_collectible_diesel.value
                        };

                        edited::MultiCombo( REPLACE_TEXT( "Flags", "旗幟", "Флаги" ), collectible_flags, {
                            REPLACE_TEXT( "Hemp", "布", "Конопля" ),
                            REPLACE_TEXT( "Wood", "小木头", "Дерево" ),
                            REPLACE_TEXT( "Stone", "小石头", "Камень" ),
                            REPLACE_TEXT( "Metal", "小金屬矿", "Металл" ),
                            REPLACE_TEXT( "Sulfur", "小硫磺矿", "Сера" ),
                            REPLACE_TEXT( "Diesel", "柴油", "Дизель" ),
                                            } );
                        edited::SliderFloat( REPLACE_TEXT( "Max distance", "最大距离", "Максимальная дистанция" ), &config::visuals_entities_collectible_max_distance.value, 0.f, 400.f, _( "%.f m" ) );
                        break;
                    }
                    case 11: {
                        edited::Checkbox( REPLACE_TEXT( "On", "启用", "Вкл/выкл" ), &config::visuals_entities_construction.value );

                        bool* construction_flags[ ] = {
                            &config::visuals_entities_construction_recycler.value,
                            &config::visuals_entities_construction_workbench_1.value,
                            &config::visuals_entities_construction_workbench_2.value,
                            &config::visuals_entities_construction_workbench_3.value,
                            &config::visuals_entities_construction_large_wood_box.value,
                            &config::visuals_entities_construction_wood_storage_box.value
                        };

                        edited::MultiCombo( REPLACE_TEXT( "Flags", "旗幟", "Флаги" ), construction_flags, {
                            REPLACE_TEXT( "Recycler", "回收商", "Переработчик" ),
                            REPLACE_TEXT( "Workbench 1", "工作台1", "Верстак 1" ),
                            REPLACE_TEXT( "Workbench 2", "工作台2", "Верстак 2" ),
                            REPLACE_TEXT( "Workbench 3", "工作台3", "Верстак 3" ),
                            REPLACE_TEXT( "Large Wood Box", "大箱子", "Большой ящик" ),
                            REPLACE_TEXT( "Wood Storage Box", "小箱子", "Маленький ящик" ),
                                            } );
                        edited::SliderFloat( REPLACE_TEXT( "Max distance", "最大距离", "Максимальная дистанция" ), &config::visuals_entities_construction_max_distance.value, 0.f, 400.f, _( "%.f m" ) );
                        break;
                    }

                    case 12: {
                        edited::Checkbox( REPLACE_TEXT( "On", "启用", "Вкл/выкл" ), &config::visuals_entities_dropped_item.value );
                        edited::Checkbox( REPLACE_TEXT( "Chams", "数量", "Chams" ), &config::visuals_entities_dropped_item_chams.value );
                        //edited::Checkbox( REPLACE_TEXT( "Icons", "数量", "Icons" ), &config::visuals_entities_dropped_item_icons.value, REPLACE_TEXT( "Showing icons of dropped items", "显示掉落物品的数量", "Показывать количество выброшенных вещей" ) );
                        edited::Checkbox( REPLACE_TEXT( "Amount", "数量", "Количество" ), &config::visuals_entities_dropped_item_amount.value );

                        bool* dropped_item_flags[ ] = {
                            &config::visuals_entities_dropped_item_all.value,
                            &config::visuals_entities_dropped_item_fun.value,
                            &config::visuals_entities_dropped_item_food.value,
                            &config::visuals_entities_dropped_item_tool.value,
                            &config::visuals_entities_dropped_item_misc.value,
                            &config::visuals_entities_dropped_item_items.value,
                            &config::visuals_entities_dropped_item_traps.value,
                            &config::visuals_entities_dropped_item_common.value,
                            &config::visuals_entities_dropped_item_weapon.value,
                            &config::visuals_entities_dropped_item_attire.value,
                            &config::visuals_entities_dropped_item_search.value,
                            &config::visuals_entities_dropped_item_medical.value,
                            &config::visuals_entities_dropped_item_resources.value,
                            &config::visuals_entities_dropped_item_component.value,
                            &config::visuals_entities_dropped_item_favourite.value,
                            &config::visuals_entities_dropped_item_electrical.value,
                            &config::visuals_entities_dropped_item_ammunition.value,
                            &config::visuals_entities_dropped_item_construction.value,
                        };

                        edited::MultiCombo( REPLACE_TEXT( "Flags", "旗帜", "Флаги" ), dropped_item_flags, {
                            REPLACE_TEXT( "Al", "全部", "Остальное" ),
                            REPLACE_TEXT( "Fun", "乐趣", "Фан" ),
                            REPLACE_TEXT( "Food", "食物", "Еда" ),
                            REPLACE_TEXT( "Tool", "工具", "Инструменты" ),
                            REPLACE_TEXT( "Misc", "杂质", "Разное" ),
                            REPLACE_TEXT( "Items", "事项", "Предметы" ),
                            REPLACE_TEXT( "Traps", "陷阱", "Ловушки" ),
                            REPLACE_TEXT( "Common", "寻常的", "Общие" ),
                            REPLACE_TEXT( "Weapon", "武器", "Оружие" ),
                            REPLACE_TEXT( "Attire", "服裝", "Наряды" ),
                            REPLACE_TEXT( "Search", "搜索", "Поисковые" ),
                            REPLACE_TEXT( "Medical", "医疗", "Медицинские" ),
                            REPLACE_TEXT( "Resources", "資源", "Ресурсы" ),
                            REPLACE_TEXT( "Component", "成分", "Компоненты" ),
                            REPLACE_TEXT( "Favourite", "喜欢的", "Избранное" ),
                            REPLACE_TEXT( "Electrical", "电力", "Электрика" ),
                            REPLACE_TEXT( "Ammunition", "弹药", "Боеприпасы" ),
                            REPLACE_TEXT( "Construction", "结构", "Конструкции" ),
                                            } );
                        edited::SliderFloat( REPLACE_TEXT( "Max distance", "最大距离", "Максимальная дистанция" ), &config::visuals_entities_dropped_item_max_distance.value, 0.f, 400.f, _( "%.f m" ) );
                        break;
                    }

                    case 13: {
                        edited::Checkbox( REPLACE_TEXT( "On", "启用", "Вкл/выкл" ), &config::visuals_entities_vehicle_water.value );

                        bool* vehicle_water_flags[ ] = {
                            &config::visuals_entities_vehicle_water_rhib.value,
                            &config::visuals_entities_vehicle_water_kayak.value,
                            &config::visuals_entities_vehicle_water_rowboat.value,
                            &config::visuals_entities_vehicle_water_tugboat.value,
                            &config::visuals_entities_vehicle_water_submarine.value,
                            &config::visuals_entities_vehicle_water_submarine_duo.value
                        };

                        edited::MultiCombo( REPLACE_TEXT( "Flags", "旗帜", "Флаги" ), vehicle_water_flags, {
                            REPLACE_TEXT( "Rhib", "快艇", "Большая лодка" ),
                            REPLACE_TEXT( "Kayak", "皮艇", "Самодельная лодка" ),
                            REPLACE_TEXT( "Rowboat", "划艇", "Лодка" ),
                            REPLACE_TEXT( "Tugboat", "拖船", "Буксир" ),
                            REPLACE_TEXT( "Submarine", "潛水艇", "Подводная лодка" ),
                            REPLACE_TEXT( "Submarine duo", "双人潜水艇", "Двойная подводная лодка" ),
                                            } );
                        edited::SliderFloat( REPLACE_TEXT( "Max distance", "最大距离", "Максимальная дистанция" ), &config::visuals_entities_vehicle_water_max_distance.value, 0.f, 400.f, _( "%.f m" ) );
                        break;
                    }
                    default:
                        break;
                }
            }
            edited::EndChild( );
        }
        ImGui::EndGroup( );
    }

    void draw_misc_tab( const ImVec2& spacing ) {
        ImGui::BeginGroup( );
        {
            edited::BeginChild( REPLACE_TEXT( "Fov", "杂项-视角", "Обзор" ), ImVec2( c::bg::size.x - ( 100 + spacing.x * 3 ), 0 ) / 2 );
            {
                edited::Checkbox( REPLACE_TEXT( "Zoom", "缩放", "Зум" ), &config::misc_fov_zoom_fov.value );
                edited::SliderFloat( REPLACE_TEXT( "Zoom amount", "视场变换器", "Изменение обзора" ), &config::misc_fov_zoom_fov_amount.value, 15.f, 85.f, reinterpret_cast< const char* >( _( u8"%.f °" ) ) );
                edited::Keybind( REPLACE_TEXT( "Zoom key", "视场变换器", "Изменение обзора" ), &config::misc_fov_zoom_fov_key.value );

                edited::Checkbox( REPLACE_TEXT( "Fov changer", "视场变换器", "Изменение обзора" ), &config::misc_fov_fov_changer.value );
                edited::SliderFloat( REPLACE_TEXT( "Fov amount", "视场变换器", "Обзор" ), &config::misc_fov_fov_changer_amount.value, 0.0f, 150.f, reinterpret_cast< const char* >( _( u8"%.f °" ) ) );
            }
            edited::EndChild( );

            edited::BeginChild( REPLACE_TEXT( "Weapon", "杂项-武器", "Оружие" ), ImVec2( c::bg::size.x - ( 100 + spacing.x * 3 ), 0 ) / 2 );
            {
                edited::Checkbox( REPLACE_TEXT( "Spam", "Spam", "Spam" ), &config::misc_weapon_spam.value );
                edited::SliderFloat( REPLACE_TEXT( "Spam delay", "Spam", "Spam" ), &config::misc_weapon_spam_delay.value, 0.f, 20.f );
                edited::Keybind( REPLACE_TEXT( "Spam key", "Spam", "Spam" ), &config::misc_weapon_spam_key.value );

                edited::Checkbox( REPLACE_TEXT( "Chams", "武器颜色", "Чамсы" ), &config::misc_weapon_chams.value );
                edited::Combo( REPLACE_TEXT( "Chams material", "武器颜色", "Чамсы" ), &config::misc_weapon_chams_value.value, {
                    REPLACE_TEXT( "Glitter", "闪光", "Блеск" ),
                    REPLACE_TEXT( "Damascus", "大马士革", "Дамаск" ),
                    REPLACE_TEXT( "Lightning", "闪电", "Молния" ),
                    REPLACE_TEXT( "Wire frame", "线框", "Проволочный каркас" ),
                    REPLACE_TEXT( "Night fire [Red]", "夜火【红】", "Ночной огонь [Красный]" ),
                    REPLACE_TEXT( "Night fire [Blue]", "夜火【蓝色】", "Ночной огонь [Синий]" ),
                    REPLACE_TEXT( "Geometric dissolve", "几何溶解", "Геометрическое растворение" )
                } );

                edited::Checkbox( REPLACE_TEXT( "Fast bow", "快弓", "Быстрый лук" ), &config::misc_weapon_fast_bow.value );

                edited::Checkbox( REPLACE_TEXT( "Fast eoka", "摇摇乐秒喷", "Быстрая еока" ), &config::misc_weapon_fast_eoka.value );
                edited::SliderFloat( REPLACE_TEXT( "Eoka chance", "摇摇乐秒喷", "Быстрая еока" ), &config::misc_weapon_fast_eoka_chance.value, 0.0f, 100.f );
                
                edited::Checkbox( REPLACE_TEXT( "Rapid fire", "快速射击", "Быстрый огонь" ), &config::misc_weapon_rapid_fire.value );
                edited::SliderFloat( REPLACE_TEXT( "Fire value", "快速射击", "Быстрый огонь" ), &config::misc_weapon_rapid_fire_speed.value, 0.0f, 20.f );

                edited::Checkbox( REPLACE_TEXT( "Automatic weapons", "自动武器", "Автоматические оружия" ), &config::misc_weapon_automatic.value );
                edited::Checkbox( REPLACE_TEXT( "No slow down on melee", "近战时不减速", "Нет замедления в ближнем бою" ), &config::misc_weapon_disable_slow_down_melee.value );
                edited::Checkbox( REPLACE_TEXT( "Instant charge compound", "复合弓能量瞬满", "Быстрый заряд блочки" ), &config::misc_weapon_instant_charge_compound.value );
            }
            edited::EndChild( );
        }
        ImGui::EndGroup( );

        ImGui::SameLine( );

        ImGui::BeginGroup( );
        {
            edited::BeginChild( REPLACE_TEXT( "Movement", "杂项-移动", "Мувменты" ), ImVec2( c::bg::size.x - ( 100 + spacing.x * 3 ), 0 ) / 2 );
            {
                edited::Checkbox( REPLACE_TEXT( "Suicide", "自杀", "Самоубийство" ), &config::misc_suicide.value );
                edited::Keybind( REPLACE_TEXT( "Suicide key", "自杀", "Самоубийство" ), &config::misc_suicide_key.value );

                edited::Checkbox( REPLACE_TEXT( "High jump", "高跳", "Высокий прыжок" ), &config::misc_high_jump.value );
                edited::SliderFloat( REPLACE_TEXT( "Jump height", "自杀", "Самоубийство" ), &config::misc_high_jump_height.value, 235.f, 100.f );

                edited::Checkbox( REPLACE_TEXT( "Debug camera", "灵魂出窍", "Дебаг камера" ), &config::misc_debug_camera.value );
                edited::Keybind( REPLACE_TEXT( "Debug camera key", "自杀", "Самоубийство" ), &config::misc_debug_camera_key.value );

                edited::Checkbox( REPLACE_TEXT( "Spiderman", "蜘蛛侠", "Спайдермен" ), &config::misc_climb_assist.value );
                edited::Checkbox( REPLACE_TEXT( "Farm assist", "自动农场", "Фарм ассист" ), &config::misc_farm_assist.value );
                edited::Checkbox( REPLACE_TEXT( "Always shoot", "总是射击", "Стрелять всегда" ), &config::misc_modify_can_attack.value );
                edited::Checkbox( REPLACE_TEXT( "Infinite jump", "无限跳跃", "Бесконечный прыжок" ), &config::misc_infinite_jump.value );
                edited::Checkbox( REPLACE_TEXT( "No fall damage", "掉落无伤", "Нет урона при падении" ), &config::misc_disable_land_damage.value );
                edited::Checkbox( REPLACE_TEXT( "Disarm landmines", "解除地雷", "Обезвредить мины" ), &config::misc_disarm_landmines.value );
            }
            edited::EndChild( );

            edited::BeginChild( REPLACE_TEXT( "Automatic", "杂项-自动", "Автоматическое" ), ImVec2( c::bg::size.x - ( 100 + spacing.x * 3 ), 0 ) / 2 );
            {
                edited::Checkbox( REPLACE_TEXT( "Auto heal", "自动回血", "Автоматический хил" ), &config::misc_automatic_auto_heal.value );
                edited::Checkbox( REPLACE_TEXT( "Auto trade", "自动交易", "Автоматический обмен" ), &config::misc_automatic_auto_trade.value );
                edited::Checkbox( REPLACE_TEXT( "Auto reload", "自动换弹", "Автоматическая перезарядка" ), &config::misc_automatic_auto_reload.value );

                edited::Checkbox( REPLACE_TEXT( "Auto upgrade", "自动升级", "Авто апгрейд" ), &config::misc_automatic_auto_upgrade.value );
                edited::Combo( REPLACE_TEXT( "Grade tier", "自动升级", "Авто апгрейд" ), &config::misc_automatic_auto_upgrade_tier.value, { REPLACE_TEXT( "Wood", "木头", "Дерево" ), REPLACE_TEXT( "Stone", "石头", "Камень" ), REPLACE_TEXT( "Metal", "铁", "Метал" ), REPLACE_TEXT( "HQM", "高金", "МВК" ) } );
            }
            edited::EndChild( );
        }
        ImGui::EndGroup( );
    }

    void draw_exploits_tab( const ImVec2& spacing ) {
        edited::BeginChild( REPLACE_TEXT( "Exploits", "漏洞", "Эксплоиты" ), ImVec2( c::bg::size.x - ( 100 + spacing.x * 3 ), 0 ) / 2 );
        {
            edited::Checkbox( REPLACE_TEXT( "Spinbot", "假管理", "Spinbot" ), &config::exploits_spinbot.value );
            edited::Checkbox( REPLACE_TEXT( "Fast loot", "快速掠夺", "Быстрое лутание" ), &config::exploits_fast_loot.value );

            edited::Checkbox( REPLACE_TEXT( "Speedhack", "移动速度", "Спидхак" ), &config::exploits_speedhack.value );
            edited::SliderFloat( REPLACE_TEXT( "Speedhack value", "移动速度", "Спидхак" ), &config::exploits_speedhack_value.value, 0.f, 10.f, _( "%.1f" ) );
            edited::Keybind( REPLACE_TEXT( "Speedhack key", "移动速度", "Спидхак" ), &config::exploits_speedhack_key.value );

            edited::Checkbox( REPLACE_TEXT( "Admin flag", "管理员标志", "Админ флаг" ), &config::exploits_admin_flag.value );
            edited::Checkbox( REPLACE_TEXT( "Omni sprint", "全方位奔跑", "Всесторонний спринт" ), &config::exploits_omni_sprint.value );

            edited::Checkbox( REPLACE_TEXT( "Name spoofer", "假名字", "Спуф имени" ), &config::exploits_name_spoofer.value );
            edited::Checkbox( REPLACE_TEXT( "Walk on water", "水上行走", "Ходить по воде" ), &config::exploits_walk_on_water.value );

            edited::Checkbox( REPLACE_TEXT( "Anti flyhack kick", "防飞踢", "Анти флай кик" ), &config::exploits_anti_fly_hack_kick.value );
            edited::Checkbox( REPLACE_TEXT( "Attack in vehicles", "无射击限制", "Стрельба с транспорта" ), &config::exploits_can_attack_in_vehicles.value );

            edited::Checkbox( REPLACE_TEXT( "No collision for trees", "树木无碰撞", "Нет колизии деревьев" ), &config::exploits_walk_through_trees.value );
            edited::Checkbox( REPLACE_TEXT( "No collision for players", "玩家无碰撞", "Нет колизии на игроков" ), &config::exploits_walk_through_players.value );
        }
        edited::EndChild( );
    }

    void draw_settings_tab( const ImVec2& spacing ) {
        ImGui::BeginGroup( );
        {
            edited::BeginChild( REPLACE_TEXT( "Main", "设置-主要的", "Основное" ), ImVec2( c::bg::size.x - ( 100 + spacing.x * 3 ), 0 ) / 2 );
            {
                edited::Combo( REPLACE_TEXT( "Language", "语言", "Язык" ), &config::settings_language.value, { _( "English" ), to_ansi( _( L"中国" ) ), to_ansi( _( L"Русский" ) ) } );

                if ( edited::Combo( REPLACE_TEXT( "Select config", "选择配置", "Выбор конфига" ), &config::manager::cfg_index, 
                     { REPLACE_TEXT( "Legit", "合法的", "Легит" ), REPLACE_TEXT( "Semi rage", "半狂怒", "Полу рейдж" ), REPLACE_TEXT( "Rage", "愤怒", "Рейдж" ) } 
                ) )
                    config::manager::init( );

                edited::Keybind( REPLACE_TEXT( "Panic key", "紧急键", "Кнопка выгрузки чита" ), &config::settings_panic_key.value );
            }
            edited::EndChild( );
        }
        ImGui::EndGroup( );

        ImGui::SameLine( );

        ImGui::BeginGroup( );
        {
            edited::BeginChild( REPLACE_TEXT( "Configs", "-设置-配置", "Конфиги" ), ImVec2( c::bg::size.x - ( 100 + spacing.x * 3 ), 0 ) / 2 );
            {
                if ( ImGui::Button( REPLACE_TEXT( "Load", "加载", "Загрузить" ), { ImGui::GetWindowWidth( ) - GImGui->Style.WindowPadding.x * 2, 49 } ) )
                    config::manager::init( );

                ImGui::SetCursorPosY( 59.f );

                if ( ImGui::Button( REPLACE_TEXT( "Save", "保存", "Сохранить" ), { ImGui::GetWindowWidth( ) - GImGui->Style.WindowPadding.x * 2, 49 } ) )
                    config::manager::save( );
            }
            edited::EndChild( );
        }
        ImGui::EndGroup( );
    }

    void draw( ) {

        if ( enabled && !config::settings_panic ) {
            ImGuiStyle* style = &ImGui::GetStyle( );

            static float color[ 4 ] = { 112 / 255.f, 109 / 255.f, 214 / 255.f, 1.f };
            c::accent_color = { color[ 0 ], color[ 1 ], color[ 2 ], 1.f };

            static float background[ 4 ] = { 21 / 255.f, 21 / 255.f, 21 / 255.f, 1.f };
            c::bg::background = { background[ 0 ], background[ 1 ], background[ 2 ], background[ 3 ] };

            static float border[ 4 ] = { 23 / 255.f, 24 / 255.f, 25 / 255.f, 1.f };
            c::bg::border = { border[ 0 ], border[ 1 ], border[ 2 ], border[ 3 ] };

            static float child[ 4 ] = { 23 / 255.f, 24 / 255.f, 25 / 255.f, 1.f };
            c::child::background = { child[ 0 ], child[ 1 ], child[ 2 ], child[ 3 ] };

            static float widget[ 4 ] = { 28 / 255.f, 28 / 255.f, 35 / 255.f, 1.f };
            c::widget::background = { widget[ 0 ], widget[ 1 ], widget[ 2 ], widget[ 3 ] };

            static float selectable[ 4 ] = { 37 / 255.f, 37 / 255.f, 47 / 255.f, 1.f };
            c::widget::selectable = { selectable[ 0 ], selectable[ 1 ], selectable[ 2 ], selectable[ 3 ] };

            static float popup[ 4 ] = { 21 / 255.f, 21 / 255.f, 22 / 255.f, 1.f };
            c::widget::popup = { popup[ 0 ], popup[ 1 ], popup[ 2 ], popup[ 3 ] };

            static float text_active[ 4 ] = { 255 / 255.f, 255 / 255.f, 255 / 255.f, 1.f };
            c::text::text_active = { text_active[ 0 ], text_active[ 1 ], text_active[ 2 ], text_active[ 3 ] };

            static float text_hovered[ 4 ] = { 89 / 255.f, 95 / 255.f, 105 / 255.f, 1.f };
            c::text::text_hov = { text_hovered[ 0 ], text_hovered[ 1 ], text_hovered[ 2 ], text_hovered[ 3 ] };

            static float text_default[ 4 ] = { 50 / 255.f, 54 / 255.f, 59 / 255.f, 1.f };
            c::text::text = { text_default[ 0 ], text_default[ 1 ], text_default[ 2 ], text_default[ 3 ] };

            style->WindowPadding = ImVec2( 0, 0 );
            style->ItemSpacing = ImVec2( 20, 0 );
            style->WindowBorderSize = 0;
            style->ScrollbarSize = 9.f;

            // ImGui::GetBackgroundDrawList()->AddImage(image::background_preview, ImVec2(0, 0), ImVec2(1920, 1080), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 255));

            ImGui::SetNextWindowSize( c::bg::size );

            ImGui::Begin( _( "WARRION" ), nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBringToFrontOnFocus );
            {
                const ImVec2& pos = ImGui::GetWindowPos( );
                const ImVec2& region = ImGui::GetContentRegionMax( );
                const ImVec2& spacing = style->ItemSpacing;

                ImGui::GetBackgroundDrawList( )->AddRectFilled( pos, pos + ImVec2( c::bg::size ), ImGui::GetColorU32( c::bg::background ), c::bg::rounding + 1 );
                ImGui::GetBackgroundDrawList( )->AddRectFilled( pos, pos + ImVec2( 100, c::bg::size.y ), ImGui::GetColorU32( c::bg::border ), c::bg::rounding, ImDrawFlags_RoundCornersLeft );

                ImGui::GetBackgroundDrawList( )->AddImage( image::logo, pos + ( ImVec2( 100, 100 ) - ImVec2( 38, 43 ) ) / 2, pos + ( ImVec2( 100, 100 ) + ImVec2( 38, 43 ) ) / 2, ImVec2( 0, 0 ), ImVec2( 1, 1 ), ImGui::GetColorU32( c::accent_color ) );

                ImGui::GetBackgroundDrawList( )->AddLine( pos + ImVec2( 0, 100 ), pos + ImVec2( 100, 100 ), ImGui::GetColorU32( c::widget::background ), 1.f );

                static int page = 0;

                ImGui::SetCursorPos( ImVec2( ( 100 - 47 ) / 2, 100 + ( 47 / 2 ) ) );
                ImGui::BeginGroup( );
                {
                    if ( edited::Tab( page == 0, 1, _( "c" ), ImVec2( 47, 47 ) ) )
                        page = 0;

                    if ( edited::Tab( page == 1, 2, _( "b" ), ImVec2( 47, 47 ) ) )
                        page = 1;

                    if ( edited::Tab( page == 2, 3, _( "o" ), ImVec2( 47, 47 ) ) )
                        page = 2;

                    if ( edited::Tab( page == 3, 4, _( "f" ), ImVec2( 47, 47 ) ) )
                        page = 3;

                    if ( edited::Tab( page == 4, 5, _( "e" ), ImVec2( 47, 47 ) ) )
                        page = 4;
                }
                ImGui::EndGroup( );

                ImGui::SetCursorPos( ImVec2( 100 + spacing.x, 0 ) );

                tab_alpha = ImClamp( tab_alpha + ( 4.f * ImGui::GetIO( ).DeltaTime * ( page == active_tab ? 1.f : -1.f ) ), 0.f, 1.f );
                if ( tab_alpha == 0.f && tab_add == 0.f ) 
                    active_tab = page;

                ImGui::PushStyleVar( ImGuiStyleVar_Alpha, tab_alpha * style->Alpha );

                ImGui::BeginChild( _( "CONTAINER" ), ImVec2( c::bg::size ) - ImVec2( 100 + spacing.x, 0 ) );
                {
                    switch ( active_tab ) {
                    case 0:
                        draw_aimbot_tab( spacing );
                        break;
                    case 1:
                        draw_visuals_tab( spacing );
                        break;
                    case 2:
                        draw_misc_tab( spacing );
                        break;
                    case 3:
                        draw_exploits_tab( spacing );
                        break;
                    case 4:
                        draw_settings_tab( spacing );
                        break;
                    }

                    ImGui::SetCursorPosY( ImGui::GetCursorPosY( ) + spacing.x );
                }
                ImGui::EndChild( );

                ImGui::PopStyleVar( );
            }
            ImGui::End( );
        }

        input_handler.handle( );

        if ( input_handler.is_down( config::settings_panic_key ) )
            config::settings_panic.value = true;

        if ( input_handler.is_down( VK_LMENU ) && input_handler.is_down( VK_INSERT ) && input_handler.is_down( VK_HOME ) )
            config::settings_panic.value = false;

        if ( input_handler.is_down( VK_INSERT ) )
            enabled = !enabled;
    }
}
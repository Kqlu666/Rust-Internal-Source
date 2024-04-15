#pragma once
#include <functional>
#include <unordered_map>

#include "unity.hpp"
#include "bundle_utils.hpp"
#include "../../options/config_mgr.hpp"

inline char* to_ansi(const wchar_t* unicode) {
	static char output[512] = { };
	ImTextStrToUtf8(
		output, sizeof(output), (const ImWchar*)unicode, (const ImWchar*)&unicode[wcslen(unicode)]);
	return output;
}

#define REPLACE_TEXT(eng, china, russian) \
   (config::settings_language == 0 ? _(eng) : \
    (config::settings_language == 1 ? to_ansi( _( L##china ) ) : \
    to_ansi( _( L##russian ) ) ) )

namespace sdk {
	class c_base_player;
	class c_map_interface;

	enum class e_tag : uint16_t {
		main_camera = 5,
		player = 6,
		terrain = 20001,
		corpse = 20009,
		misc = 20006,
		animal = 20008,
		skydome = 20012,
		rivermesh = 20014,
		monument = 20015
	};

	enum e_model_state_flags : uint32_t {
		in_duck = 1,
		has_jumped = 2,
		on_ground = 4,
		asleep = 8,
		in_sprint = 16,
		on_ladder = 32,
		in_fly = 64,
		in_aim = 128,
		has_pruned = 256,
		is_mounted = 512,
		model_is_relaxed = 1024,
		is_on_phone = 2048,
	};

	enum e_flag {
		unused_1 = ( 1 << 0 ),
		unused_2 = ( 1 << 1 ),
		is_admin = ( 1 << 2 ),
		receiving_snapshot = ( 1 << 3 ),
		sleeping = ( 1 << 4 ),
		spectating = ( 1 << 5 ),
		wounded = ( 1 << 6 ),
		is_developer = ( 1 << 7 ),
		connected = ( 1 << 8 ),
		third_person_viewmode = ( 1 << 10 ),
		eyes_viewmode = ( 1 << 11 ),
		chat_mute = ( 1 << 12 ),
		no_sprint = ( 1 << 13 ),
		aiming = ( 1 << 14 ),
		display_sash = ( 1 << 15 ),
		relaxed = ( 1 << 16 ),
		safe_zone = ( 1 << 17 ),
		server_fall = ( 1 << 18 ),
		workbench_1 = ( 1 << 20 ),
		workbench_2 = ( 1 << 21 ),
		workbench_3 = ( 1 << 22 ),
	};

	enum e_signal {
		attack,
		alt_attack,
		dry_fire,
		reload,
		deploy,
		flinch_head,
		flinch_chest,
		flinch_stomach,
		flinch_rear_head,
		flinch_rear_torso,
		_throw,
		relax,
		gesture,
		phys_impact,
		eat,
		startled
	};

	enum e_bone {
		pelvis = 0,
		l_hip = 1,
		l_knee = 2,
		l_foot = 3,
		l_toe = 4,
		l_ankle_scale = 5,
		penis = 6,
		GenitalCensor = 7,
		GenitalCensor_LOD0 = 8,
		Inner_LOD0 = 9,
		GenitalCensor_LOD1 = 11,
		GenitalCensor_LOD2 = 12,
		r_hip = 13,
		r_knee = 14,
		r_foot = 15,
		r_toe = 16,
		r_ankle_scale = 17,
		spine1 = 18,
		spine1_scale = 19,
		spine2 = 20,
		spine3 = 21,
		spine4 = 22,
		l_clavicle = 23,
		l_upperarm = 24,
		l_forearm = 25,
		l_hand = 26,
		l_index1 = 27,
		l_index2 = 28,
		l_index3 = 29,
		l_little1 = 30,
		l_little2 = 31,
		l_little3 = 32,
		l_middle1 = 33,
		l_middle2 = 34,
		l_middle3 = 35,
		l_prop = 36,
		l_ring1 = 37,
		l_ring2 = 38,
		l_ring3 = 39,
		l_thumb1 = 40,
		l_thumb2 = 41,
		l_thumb3 = 42,
		IKtarget_righthand_min = 43,
		IKtarget_righthand_max = 44,
		l_ulna = 45,
		neck = 46,
		head = 47,
		jaw = 48,
		eyeTranform = 49,
		l_eye = 50,
		l_Eyelid = 51,
		r_eye = 52,
		r_Eyelid = 53,
		r_clavicle = 54,
		r_upperarm = 55,
		r_forearm = 56,
		r_hand = 57,
		r_index1 = 58,
		r_index2 = 59,
		r_index3 = 60,
		r_little1 = 61,
		r_little2 = 62,
		r_little3 = 63,
		r_middle2 = 65,
		r_middle3 = 66,
		r_prop = 67,
		r_ring1 = 68,
		r_ring2 = 69,
		r_ring3 = 70,
		r_thumb1 = 71,
		r_thumb2 = 72,
		r_thumb3 = 73,
		IKtarget_lefthand_min = 74,
		IKtarget_lefthand_max = 75,
		r_ulna = 76,
		l_breast = 77,
		r_breast = 78,
		BoobCensor = 79,
		BreastCensor_LOD0 = 80,
		BreastCensor_LOD1 = 83,
		BreastCensor_LOD2 = 84
	};

	class c_translate_phase {
	public:
		inline auto name( ) {
			il2cpp_method( ptr, _( "" ), _( "TokenisedPhrase" ), _( "get_translated" ), -1, unity::c_string * ( * )( c_translate_phase* ) );
			return safe.call< unity::c_string* >( ptr, this );
		}

		inline auto english( ) {
			return mem::read< unity::c_string* >( this + 0x18 );
		}
	};

	static unity::c_camera* camera;

	static vec3_t local_position;
	static c_base_player* local_player;

	class c_sell_order;

	struct entity_type_t {
		std::function<std::string( )> name;

		bool* enabled;
		c_color* color;

		entity_type_t( ) { }
		entity_type_t( bool* _enabled, c_color* _color ) : enabled( _enabled ), color( _color ) { }
		entity_type_t( std::function<std::string( )> _name, bool* _enabled, c_color* _color ) : name( _name ), enabled( _enabled ), color( _color ) { }

		bool state( ) const {
			return *enabled;
		}

		c_color current_color( ) const {
			return *color;
		}
	};

	static std::unordered_map< hash_t, entity_type_t > ores = {
		{ HASH( "stone-ore" ), { [ ]( ) { return REPLACE_TEXT( "Stone ore", "石矿石", "Каменная руда" ); }, &config::visuals_entities_ore_stone.value,&config::visuals_entities_ore_stone_color.value } },
		{ HASH( "metal-ore" ), { [ ]( ) { return REPLACE_TEXT( "Metal ore", "金属矿石", "Металлическая руда" ); }, &config::visuals_entities_ore_metal.value, &config::visuals_entities_ore_metal_color.value } },
		{ HASH( "sulfur-ore" ), { [ ]( ) { return REPLACE_TEXT( "Sulfur ore", "硫磺矿石", "Серная руда" ); }, &config::visuals_entities_ore_sulfur.value, &config::visuals_entities_ore_sulfur_color.value } },
	};

	static std::unordered_map< hash_t, entity_type_t > foods = {
		{ HASH( "corn-collectable" ), { [ ]( ) { return REPLACE_TEXT( "Corn", "玉米", "Кукуруза" ); }, &config::visuals_entities_food_corn.value, &config::visuals_entities_food_corn_color.value } },
	    { HASH( "mushroom-cluster-5" ), { [ ]( ) { return REPLACE_TEXT( "Mushroom", "蘑菇", "Гриб" ); }, &config::visuals_entities_food_mushroom.value, &config::visuals_entities_food_mushroom_color.value } },
		{ HASH( "mushroom-cluster-6" ), { [ ]( ) { return REPLACE_TEXT( "Mushroom", "蘑菇", "Гриб" ); }, &config::visuals_entities_food_mushroom.value, &config::visuals_entities_food_mushroom_color.value } },
		{ HASH( "potato-collectable" ), { [ ]( ) { return REPLACE_TEXT( "Potato", "土豆", "Картошка" ); }, &config::visuals_entities_food_potato.value, &config::visuals_entities_food_potato_color.value } },
		{ HASH( "pumpkin-collectable" ), { [ ]( ) { return REPLACE_TEXT( "Pumpkin", "南瓜", "Тыква" ); }, &config::visuals_entities_food_pumpkin.value, &config::visuals_entities_food_pumpkin_color.value } },
	};

	static std::unordered_map< hash_t, entity_type_t > traps = {
		{ HASH( "GunTrap" ), { [ ]( ) { return REPLACE_TEXT( "Gun trap", "枪陷阱", "Ловушка" ); }, &config::visuals_entities_trap_gun.value, &config::visuals_entities_trap_gun_color.value } },
		{ HASH( "SamSite" ), { [ ]( ) { return REPLACE_TEXT( "Sam site", "山姆网站", "ПВО" ); }, &config::visuals_entities_trap_sam_site.value, &config::visuals_entities_trap_sam_site_color.value } },
		{ HASH( "BearTrap" ), { [ ]( ) { return REPLACE_TEXT( "Bear trap", "捕熊器", "Капкан" ); }, &config::visuals_entities_trap_bear.value, &config::visuals_entities_trap_bear_color.value } },
		{ HASH( "Landmine" ), { [ ]( ) { return REPLACE_TEXT( "Landmine", "地雷", "Мина" ); }, &config::visuals_entities_trap_landmine.value, &config::visuals_entities_trap_landmine_color.value } },
		{ HASH( "AutoTurret" ), { [ ]( ) { return REPLACE_TEXT( "Auto turret", "自动炮塔", "Автоматическая турель" ); }, &config::visuals_entities_trap_auto_turret.value, &config::visuals_entities_trap_auto_turret_color.value } },
		{ HASH( "FlameTurret" ), { [ ]( ) { return REPLACE_TEXT( "Flame turret", "火焰炮塔", "Огненная турель" ); }, &config::visuals_entities_trap_flame_turret.value, &config::visuals_entities_trap_flame_turret_color.value } },
	};

	static std::unordered_map< hash_t, entity_type_t > loots = {
		{ HASH( "SupplyDrop" ), { [ ]( ) { return REPLACE_TEXT( "Supply drop", "供应下降", "Аирдроп" ); }, &config::visuals_entities_loot_supply_drop.value, &config::visuals_entities_loot_supply_drop_color.value } },
		{ HASH( "PlayerCorpse" ), { [ ]( ) { return REPLACE_TEXT( "Corpse", "尸体", "Труп" ); }, &config::visuals_entities_loot_corpse.value, &config::visuals_entities_loot_corpse_color.value } },
		{ HASH( "StashContainer" ), { [ ]( ) { return REPLACE_TEXT( "Small stash", "小藏品", "Тайник" ); }, &config::visuals_entities_loot_small_stash.value, &config::visuals_entities_loot_small_stash_color.value } },
		{ HASH( "DroppedItemContainer" ), { [ ]( ) { return REPLACE_TEXT( "Backpack", "背包", "Рюкзак" ); }, &config::visuals_entities_loot_backpack.value, &config::visuals_entities_loot_backpack_color.value } }
	};

	static std::unordered_map< hash_t, entity_type_t > crates = {
		{ HASH( "crate_fuel" ), { [ ]( ) { return REPLACE_TEXT( "Fuel crate", "燃料箱", "Ящик для топлива" ); }, &config::visuals_entities_crate_fuel.value, &config::visuals_entities_crate_fuel_color.value } },
		{ HASH( "crate_mine" ), { [ ]( ) { return REPLACE_TEXT( "Mine crate", "矿井板条箱", "Ящик минёра" ); }, &config::visuals_entities_crate_mine.value, &config::visuals_entities_crate_mine_color.value } },
		{ HASH( "crate_tools" ), { [ ]( ) { return REPLACE_TEXT( "Tools crate", "工具箱", "Ящик для инструментов" ); }, &config::visuals_entities_crate_tools.value, &config::visuals_entities_crate_tools_color.value } },
		{ HASH( "crate_elite" ), { [ ]( ) { return REPLACE_TEXT( "Elite crate", "精英箱", "Элитный ящик" ); }, &config::visuals_entities_crate_elite.value, &config::visuals_entities_crate_elite_color.value } },
		{ HASH( "crate_basic" ), { [ ]( ) { return REPLACE_TEXT( "Basic crate", "基本板条箱", "Базовый ящик" ); }, &config::visuals_entities_crate_basic.value, &config::visuals_entities_crate_basic_color.value } },
		{ HASH( "crate_normal" ), { [ ]( ) { return REPLACE_TEXT( "Military crate", "军用板条箱", "Военный ящик" ); }, &config::visuals_entities_crate_military.value, &config::visuals_entities_crate_military_color.value } },
		{ HASH( "crate_food_1" ), { [ ]( ) { return REPLACE_TEXT( "Food crate", "食品箱", "Ящик для еды" ); }, &config::visuals_entities_crate_food.value, &config::visuals_entities_crate_food_color.value } },
		{ HASH( "crate_food_2" ), { [ ]( ) { return REPLACE_TEXT( "Food crate", "食品箱", "Ящик для еды" ); }, &config::visuals_entities_crate_food.value, &config::visuals_entities_crate_food_color.value } },
		{ HASH( "crate_medical" ), { [ ]( ) { return REPLACE_TEXT( "Medical crate", "医疗箱", "Медицинский ящик" ); }, &config::visuals_entities_crate_medical.value, &config::visuals_entities_crate_medical_color.value } },
		{ HASH( "crate_normal_2" ), { [ ]( ) { return REPLACE_TEXT( "Normal crate", "普通板条箱", "Обычный ящик" ); }, &config::visuals_entities_crate_normal.value, &config::visuals_entities_crate_normal_color.value } },
		{ HASH( "crate_ammunition" ), { [ ]( ) { return REPLACE_TEXT( "Ammunition crate", "弹药箱", "Ящик для патрон" ); }, &config::visuals_entities_crate_ammunition.value, &config::visuals_entities_crate_ammunition_color.value } },
		{ HASH( "crate_normal_2_food" ), { [ ]( ) { return REPLACE_TEXT( "Food crate", "食品箱", "Ящик для еды" ); }, &config::visuals_entities_crate_food.value, &config::visuals_entities_crate_food_color.value } },
		{ HASH( "crate_normal_2_medical" ), { [ ]( ) { return REPLACE_TEXT( "Medical crate", "医疗箱", "Медицинский ящик" ); }, &config::visuals_entities_crate_medical.value, &config::visuals_entities_crate_medical_color.value } },
		{ HASH( "crate_underwater_basic" ), { [ ]( ) { return REPLACE_TEXT( "Underwater basic crate", "水下基本板条箱", "Базовый подводный ящик" ); }, &config::visuals_entities_crate_underwater_basic.value, &config::visuals_entities_crate_underwater_basic_color.value } },
		{ HASH( "crate_underwater_advanced" ), { [ ]( ) { return REPLACE_TEXT( "Underwater advanced crate", "水下高级板条箱", "Усовершенствованный подводный ящик" ); }, &config::visuals_entities_crate_underwater_advanced.value, &config::visuals_entities_crate_underwater_advanced_color.value } },
	};

	static std::unordered_map< hash_t, entity_type_t > animals = {
		{ HASH( "Stag" ), { [ ]( ) { return REPLACE_TEXT( "Stag", "麈", "Олень" ); }, &config::visuals_entities_animal_stag.value, &config::visuals_entities_animal_stag_color.value } },
		{ HASH( "Boar" ), { [ ]( ) { return REPLACE_TEXT( "Boar", "公猪", "Кабан" ); }, &config::visuals_entities_animal_boar.value, &config::visuals_entities_animal_boar_color.value } },
		{ HASH( "Bear" ), { [ ]( ) { return REPLACE_TEXT( "Bear", "熊", "Медведь" ); }, &config::visuals_entities_animal_bear.value, &config::visuals_entities_animal_bear_color.value } },
		{ HASH( "Wolf" ), { [ ]( ) { return REPLACE_TEXT( "Wolf", "狼", "Волк" ); }, &config::visuals_entities_animal_wolf.value, &config::visuals_entities_animal_wolf_color.value } },
		{ HASH( "Polarbear" ), { [ ]( ) { return REPLACE_TEXT( "Polar bear", "北极熊", "Полярный медведь" ); }, &config::visuals_entities_animal_polar_bear.value, &config::visuals_entities_animal_polar_bear_color.value } },
	};

	static std::unordered_map< hash_t, entity_type_t > berries = {
		{ HASH( "berry-red-collectable" ), { [ ]( ) { return REPLACE_TEXT( "Red berry", "红莓", "Красная ягода" ); }, &config::visuals_entities_berry_red.value, &config::visuals_entities_berry_red_color.value } },
		{ HASH( "berry-blue-collectable" ), { [ ]( ) { return REPLACE_TEXT( "Blue berry", "蓝莓", "Голубая ягода" ); }, &config::visuals_entities_berry_blue.value, &config::visuals_entities_berry_blue_color.value } },
		{ HASH( "berry-black-collectable" ), { [ ]( ) { return REPLACE_TEXT( "Black berry", "黑莓", "Черная ягода" ); }, &config::visuals_entities_berry_black.value, &config::visuals_entities_berry_black_color.value } },
		{ HASH( "berry-green-collectable" ), { [ ]( ) { return REPLACE_TEXT( "Green berry", "绿色浆果", "Зеленая ягода" ); }, &config::visuals_entities_berry_green.value, &config::visuals_entities_berry_green_color.value } },
		{ HASH( "berry-white-collectable" ), { [ ]( ) { return REPLACE_TEXT( "White berry", "白浆果", "Белая ягода" ); }, &config::visuals_entities_berry_white.value, &config::visuals_entities_berry_white_color.value } },
		{ HASH( "berry-yellow-collectable" ), { [ ]( ) { return REPLACE_TEXT( "Yellow berry", "黄色浆果", "Желтая ягода" ); }, &config::visuals_entities_berry_yellow.value, &config::visuals_entities_berry_yellow_color.value } },
	};

	static std::unordered_map< hash_t, entity_type_t > barrels = {
		{ HASH( "oil_barrel" ), { [ ]( ) { return REPLACE_TEXT( "Oil barrel", "油桶", "Нефтяная бочка" ); }, &config::visuals_entities_barrel_oil.value, &config::visuals_entities_barrel_oil_color.value } },
		{ HASH( "loot_barrel_1" ), { [ ]( ) { return REPLACE_TEXT( "Blue barrel", "蓝桶", "Синяя бочка" ); }, &config::visuals_entities_barrel_blue.value, &config::visuals_entities_barrel_blue_color.value } },
		{ HASH( "loot_barrel_2" ), { [ ]( ) { return REPLACE_TEXT( "Basic barrel", "基本桶", "Базовая бочка" ); }, &config::visuals_entities_barrel_basic.value, &config::visuals_entities_barrel_basic_color.value } },
	};
	
	static std::unordered_map< hash_t, entity_type_t > vehicles = {
		{ HASH( "BradleyAPC" ), { [ ]( ) { return REPLACE_TEXT( "Bradley", "布拉德利", "Танк" ); }, &config::visuals_entities_vehicle_bradley_apc.value, &config::visuals_entities_vehicle_bradley_apc_color.value } },
		{ HASH( "Minicopter" ), { [ ]( ) { return REPLACE_TEXT( "Minicopter", "微型直升机", "Миникоптер" ); }, &config::visuals_entities_vehicle_mini_copter.value, &config::visuals_entities_vehicle_mini_copter_color.value } },
		{ HASH( "RidableHorse" ), { [ ]( ) { return REPLACE_TEXT( "Horse", "马", "Лошадь" ); }, &config::visuals_entities_vehicle_horse.value, &config::visuals_entities_vehicle_horse_color.value } },
		{ HASH( "GroundVehicle" ), { [ ]( ) { return REPLACE_TEXT( "Ground vehicle", "地面车辆", "Наземная машина" ); }, &config::visuals_entities_vehicle_base.value, &config::visuals_entities_vehicle_base_color.value } },
		{ HASH( "HotAirBalloon" ), { [ ]( ) { return REPLACE_TEXT( "Hot air balloon", "热气球", "Воздушный шар" ); }, &config::visuals_entities_vehicle_air_baloon.value, &config::visuals_entities_vehicle_air_baloon_color.value } },
		{ HASH( "PatrolHelicopter" ), { [ ]( ) { return REPLACE_TEXT( "Patrol helicopter", "巡逻直升机", "Патрульный вертолет" ); }, &config::visuals_entities_vehicle_helicopter.value, &config::visuals_entities_vehicle_helicopter_color.value } },
		{ HASH( "ScrapTransportHelicopter" ), { [ ]( ) { return REPLACE_TEXT( "Scrap helicopter", "报废直升机", "Корова" ); }, &config::visuals_entities_vehicle_scrap_heli.value, &config::visuals_entities_vehicle_scrap_heli_color.value } }
	};

	static std::unordered_map< hash_t, entity_type_t > collectibles = {
		{ HASH( "hemp-collectable" ), { [ ]( ) { return REPLACE_TEXT( "Hemp", "麻", "Конопля" ); }, &config::visuals_entities_collectible_hemp.value, &config::visuals_entities_collectible_hemp_color.value } },
		{ HASH( "wood-collectable" ), { [ ]( ) { return REPLACE_TEXT( "Wood", "木头", "Дерево" ); }, &config::visuals_entities_collectible_wood.value, &config::visuals_entities_collectible_wood_color.value } },
		{ HASH( "stone-collectable" ), { [ ]( ) { return REPLACE_TEXT( "Stone", "石头", "Камень" ); }, &config::visuals_entities_collectible_stone.value, &config::visuals_entities_collectible_stone_color.value } },
		{ HASH( "metal-collectable" ), { [ ]( ) { return REPLACE_TEXT( "Metal", "金属", "Металл" ); }, &config::visuals_entities_collectible_metal.value, &config::visuals_entities_collectible_metal_color.value } },
		{ HASH( "sulfur-collectable" ), { [ ]( ) { return REPLACE_TEXT( "Sulfur", "硫", "Сера" ); }, &config::visuals_entities_collectible_sulfur.value, &config::visuals_entities_collectible_sulfur_color.value } },
		{ HASH( "diesel_collectable" ), { [ ]( ) { return REPLACE_TEXT( "Дизель", "柴油机", "Дизель" ); }, &config::visuals_entities_collectible_diesel.value, &config::visuals_entities_collectible_diesel_color.value } },
		{ HASH( "diesel_barrel.item" ), { [ ]( ) { return REPLACE_TEXT( "Дизель", "柴油机", "Дизель" ); }, &config::visuals_entities_collectible_diesel.value, &config::visuals_entities_collectible_diesel_color.value } },
		{ HASH( "diesel_barrel_world" ), { [ ]( ) { return REPLACE_TEXT( "Дизель", "柴油机", "Дизель" ); }, &config::visuals_entities_collectible_diesel.value, &config::visuals_entities_collectible_diesel_color.value } }
	};

	static std::unordered_map< hash_t, entity_type_t > constructions = {
		{ HASH( "Recycler" ), { [ ]( ) { return REPLACE_TEXT( "Recycler", "回收商", "Переработчик" ); }, &config::visuals_entities_construction_recycler.value, &config::visuals_entities_construction_recycler_color.value } },
		{ HASH( "Workbench" ), { [ ]( ) { return REPLACE_TEXT( "Workbench", "工作台", "Верстак" ); }, &config::visuals_entities_construction_workbench_1.value, &config::visuals_entities_construction_workbench_1_color.value } },
		{ HASH( "box.wooden.large" ), { [ ]( ) { return REPLACE_TEXT( "Large wood storage", "工作台", "Большой ящик" ); }, &config::visuals_entities_construction_large_wood_box.value, &config::visuals_entities_construction_large_wood_box_color.value } },
		{ HASH( "woodbox_deployed" ), { [ ]( ) { return REPLACE_TEXT( "Wood storage box", "木质储物盒", "Маленький ящик" ); }, &config::visuals_entities_construction_wood_storage_box.value, &config::visuals_entities_construction_wood_storage_box_color.value } }
	};

	static std::unordered_map< hash_t, entity_type_t > vehicles_water = {
		{ HASH( "RHIB" ), { [ ]( ) { return REPLACE_TEXT( "RHIB", "RHIB", "Большая лодка" ); }, &config::visuals_entities_vehicle_water_rhib.value, &config::visuals_entities_vehicle_water_rhib_color.value } },
		{ HASH( "Kayak" ), { [ ]( ) { return REPLACE_TEXT( "Kayak", "皮艇", "Байдарка" ); }, &config::visuals_entities_vehicle_water_kayak.value, &config::visuals_entities_vehicle_water_kayak_color.value } },
		{ HASH( "Tugboat" ), { [ ]( ) { return REPLACE_TEXT( "Tugboat", "皮艇", "Буксир" ); }, &config::visuals_entities_vehicle_water_tugboat.value, &config::visuals_entities_vehicle_water_tugboat_color.value } },
		{ HASH( "MotorRowboat" ), { [ ]( ) { return REPLACE_TEXT( "Rowboat", "划艇", "Лодка" ); }, &config::visuals_entities_vehicle_water_rowboat.value, &config::visuals_entities_vehicle_water_rowboat_color.value } },
		{ HASH( "SubmarineDuo" ), { [ ]( ) { return REPLACE_TEXT( "Submarine duo", "潜艇二人组", "Подводная лодка дуо" ); }, &config::visuals_entities_vehicle_water_submarine_duo.value, &config::visuals_entities_vehicle_water_submarine_duo_color.value } },
		{ HASH( "BaseSubmarine" ), { [ ]( ) { return REPLACE_TEXT( "Submarine", "潜艇", "Подводная лодка" ); }, &config::visuals_entities_vehicle_water_submarine.value, &config::visuals_entities_vehicle_water_submarine_color.value } },
	};

	inline std::unordered_map< hash_t, entity_type_t > entity_types = { { HASH( "DroppedItem" ), { &config::visuals_entities_vehicle_water_rhib.value, &config::visuals_entities_vehicle_water_rhib_color.value } } };

	struct entity_types_t {
		static void initialize( ) {
			entity_types.insert( ores.begin( ), ores.end( ) );
			entity_types.insert( foods.begin( ), foods.end( ) );
			entity_types.insert( traps.begin( ), traps.end( ) );
			entity_types.insert( loots.begin( ), loots.end( ) );
			entity_types.insert( crates.begin( ), crates.end( ) );
			entity_types.insert( animals.begin( ), animals.end( ) );
			entity_types.insert( berries.begin( ), berries.end( ) );
			entity_types.insert( barrels.begin( ), barrels.end( ) );
			entity_types.insert( vehicles.begin( ), vehicles.end( ) );
			entity_types.insert( collectibles.begin( ), collectibles.end( ) );
			entity_types.insert( constructions.begin( ), constructions.end( ) );
			entity_types.insert( vehicles_water.begin( ), vehicles_water.end( ) );
		}

		static const entity_type_t& get( const hash_t& hash, const hash_t& prefab_hash ) {
			if ( entity_types.contains( hash ) )
				return entity_types[ hash ];
			if ( entity_types.contains( prefab_hash ) )
				return entity_types[ prefab_hash ];

			return { };
		}

		static auto has( const hash_t& hash, const hash_t& prefab_hash ) {
			return entity_types.contains( hash ) || 
				entity_types.contains( prefab_hash );
		}

		static auto contains( const std::unordered_map< hash_t, entity_type_t >& types, const hash_t& hash, const hash_t& prefab_hash ) {
			return types.contains( hash ) ||
				types.contains( prefab_hash );
		}
	};

	enum class e_item_category {
		weapon,
		construction,
		items,
		resources,
		attire,
		tool,
		medical,
		food,
		ammunition,
		traps,
		misc,
		all,
		common,
		component,
		search,
		favourite,
		electrical,
		fun
	};

	struct dropped_item_t {
		bool* enabled;
		c_color* color;

		bool state( ) const {
			return *enabled;
		}

		c_color current_color( ) const {
			return *color;
		}
	};

	static std::unordered_map< e_item_category, dropped_item_t > dropped_items = {
		{ e_item_category::all, { &config::visuals_entities_dropped_item_all.value, &config::visuals_entities_dropped_item_all_color.value } },
		{ e_item_category::fun, { &config::visuals_entities_dropped_item_fun.value, &config::visuals_entities_dropped_item_fun_color.value } },
		{ e_item_category::food, { &config::visuals_entities_dropped_item_food.value, &config::visuals_entities_dropped_item_food_color.value } },
		{ e_item_category::tool, { &config::visuals_entities_dropped_item_tool.value, &config::visuals_entities_dropped_item_tool_color.value } },
		{ e_item_category::misc, { &config::visuals_entities_dropped_item_misc.value, &config::visuals_entities_dropped_item_misc_color.value } },
		{ e_item_category::items, { &config::visuals_entities_dropped_item_items.value, &config::visuals_entities_dropped_item_items_color.value } },
		{ e_item_category::traps, { &config::visuals_entities_dropped_item_traps.value, &config::visuals_entities_dropped_item_traps_color.value } },
		{ e_item_category::common, { &config::visuals_entities_dropped_item_common.value, &config::visuals_entities_dropped_item_common_color.value } },
		{ e_item_category::weapon, { &config::visuals_entities_dropped_item_weapon.value, &config::visuals_entities_dropped_item_weapon_color.value } },
		{ e_item_category::attire, { &config::visuals_entities_dropped_item_attire.value, &config::visuals_entities_dropped_item_attire_color.value } },
		{ e_item_category::search, { &config::visuals_entities_dropped_item_search.value, &config::visuals_entities_dropped_item_search_color.value } },
		{ e_item_category::medical, { &config::visuals_entities_dropped_item_medical.value, &config::visuals_entities_dropped_item_medical_color.value } },
		{ e_item_category::resources, { &config::visuals_entities_dropped_item_resources.value, &config::visuals_entities_dropped_item_resources_color.value } },
		{ e_item_category::component, { &config::visuals_entities_dropped_item_component.value, &config::visuals_entities_dropped_item_component_color.value } },
		{ e_item_category::favourite, { &config::visuals_entities_dropped_item_favourite.value, &config::visuals_entities_dropped_item_favourite_color.value } },
		{ e_item_category::electrical, { &config::visuals_entities_dropped_item_electrical.value, &config::visuals_entities_dropped_item_electrical_color.value } },
		{ e_item_category::ammunition, { &config::visuals_entities_dropped_item_ammunition.value, &config::visuals_entities_dropped_item_ammunition_color.value } },
		{ e_item_category::construction, { &config::visuals_entities_dropped_item_construction.value, &config::visuals_entities_dropped_item_construction_color.value } }
	};

	struct world_item_t {
		std::string name;
		c_color color = c_color::white( );

		int amount = 1;
		ID3D11ShaderResourceView* image{ };
		bool is_dropped_item = false;

		std::string player_name;
		bool is_corpse = false;

		vec3_t position{ };
		quat_t rotation{ };
		unity::bounds_t bounds{ };

		std::pair<vec2_t, vec2_t> find_bb_minmax( vec3_t origin, vec3_t center, vec3_t extents, quat_t rotation ) {
			vec3_t points[ 8 ] =
			{
				vec3_t( center.x - extents.x, center.y + extents.y, center.z - extents.z ),
				vec3_t( center.x + extents.x, center.y + extents.y, center.z - extents.z ),
				vec3_t( center.x - extents.x, center.y - extents.y, center.z - extents.z ),
				vec3_t( center.x + extents.x, center.y - extents.y, center.z - extents.z ),
				vec3_t( center.x - extents.x, center.y + extents.y, center.z + extents.z ),
				vec3_t( center.x + extents.x, center.y + extents.y, center.z + extents.z ),
				vec3_t( center.x - extents.x, center.y - extents.y, center.z + extents.z ),
				vec3_t( center.x + extents.x, center.y - extents.y, center.z + extents.z ),
			};

			vec2_t screen_points[ 8 ];

			for ( size_t i = 0; i < ARRAYSIZE( points ); i++ ) {
				points[ i ] = origin + rotation * points[ i ];
				if ( !sdk::camera->world_to_screen_point( points[ i ], screen_points[ i ] ) )
					return std::make_pair( vec2_t{ }, vec2_t{ } );
			}

			vec2_t min = screen_points[ 0 ], max = screen_points[ 0 ];
			for ( size_t i = 0; i < ARRAYSIZE( screen_points ); i++ ) {
				const auto& point = screen_points[ i ];

				min = vec2_t{ std::min( point.x, min.x ), std::min( point.y, min.y ) };
				max = vec2_t{ std::max( point.x, max.x ), std::max( point.y, max.y ) };
			}

			return std::make_pair( min, max );
		}
	};

	enum class e_player_state {
		inside,
		outside
	};

	inline struct {
		std::vector< c_base_player* > players = { };
		std::vector< world_item_t > world_items = { };

		std::unordered_map< c_base_player*, bool > visible_states = { };
		std::unordered_map< c_base_player*, e_player_state > player_states = { };
		std::unordered_map< c_base_player*, std::unordered_map< e_bone, vec3_t > > bone_positions = { };
	} info;

	inline void clear( ) {
		camera = nullptr;
		local_player = nullptr;

		info = { };

		bundles::destroy_all( );
	}
} // namespace sdk

#include "convar.hpp"
#include "model_state.hpp"
#include "base_movement.hpp"
#include "player_walk_movement.hpp"
#include "base_networkable.hpp"
#include "model.hpp"
#include "base_entity.hpp"
#include "base_combat_entity.hpp"
#include "player_model.hpp"
#include "item_definition.hpp"
#include "magazine.hpp"
#include "view_model.hpp"
#include "attack.hpp"
#include "environment.hpp"
#include "entity.hpp"
#include "player_attack.hpp"
#include "player_projectile_attack.hpp"
#include "hit_test.hpp"
#include "projectile.hpp"
#include "projectile_shoot.hpp"
#include "held_entity.hpp"
#include "item_mod_projectile.hpp"
#include "item.hpp"
#include "item_container.hpp"
#include "player_inventory.hpp"
#include "input_state.hpp"
#include "player_input.hpp"
#include "player_eyes.hpp"
#include "base_mountable.hpp"
#include "player_tick.hpp"
#include "game_physics.hpp"
#include "player_team.hpp"
#include "player_metabolism.hpp"
#include "player_blueprints.hpp"
#include "player_projectile_update.hpp"
#include "patrol_helicopter.hpp"
#include "base_player.hpp"
#include "world_item.hpp"
#include "player_name_id.hpp"
#include "building_privlidge.hpp"
#include "console_gen.hpp"
#include "game_manager.hpp"
#include "item_icon.hpp"
#include "stash_container.hpp"
#include "vending_machine.hpp"
#include "main_camera.hpp"
#include "building_block.hpp"
#include "workbench.hpp"
#include "hit_info.hpp"
#include "water_level.hpp"
#include "hackable_locked_crate.hpp"
#include "tod_sky.hpp"
#include "loot_panel_vending_machine.hpp"
#include "trigger_safe_zone.hpp"
#include "rust_application.hpp"
#include "facepunch.hpp"
#include "map_interface.hpp"
#include "console_system.hpp"
#include "game_trace.hpp"
#include "map_marker.hpp"
#include "buttons.hpp"
#include "camera_man.hpp"
#include "terrain_meta.hpp"
#include "lootable_corpse.hpp"
#include "dropped_item_container.hpp"
#include "console_ui.hpp"
#include "patrol_helicopter_ai.hpp"
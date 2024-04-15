#pragma once

namespace sdk {
	class c_inventory_def_id {
	public:
		il2cpp_field_set( int, set_value, _( "Steamworks.Data" ), _( "InventoryDefId" ), _( "Value" ) );
	};

	class c_inventory_item_id {
	public:
		il2cpp_field_set( unsigned int, set_value, _( "Steamworks.Data" ), _( "InventoryItemId" ), _( "Value" ) );
	};

	class c_inventory_item {
	public:
		il2cpp_field_set( c_inventory_item_id*, set_id, _( "Steamworks" ), _( "InventoryItem" ), _( "_id" ) );
		il2cpp_field_set( c_inventory_def_id*, set_def, _( "Steamworks" ), _( "InventoryItem" ), _( "_def" ) );

		il2cpp_field_get( c_inventory_def_id*, def, _( "Steamworks" ), _( "InventoryItem" ), _( "_def" ) );
	};

	class c_steam_item {
	public:
		il2cpp_field_set( c_inventory_item*, set_value, _( "Rust.Platform.Steam" ), _( "SteamItem" ), _( "Value" ) );

		il2cpp_field_get( c_inventory_item*, value, _( "Rust.Platform.Steam" ), _( "SteamItem" ), _( "Value" ) );
	};

	class c_steam_inventory {
	public:
		il2cpp_field_get( unity::c_array< c_steam_item* >*, items, _( "" ), _( "SteamInventory" ), _( "Items" ) );
	};

	class c_player_blueprints {
	public:
		il2cpp_field_get( c_steam_inventory*, steam_inventory, _( "" ), _( "PlayerBlueprints" ), _( "steamInventory" ) );
	};
}
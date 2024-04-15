#pragma once

namespace sdk {
	class c_loot_panel_vending_machine : public unity::c_behaviour {
	public:
		il2cpp_static_field_get( c_loot_panel_vending_machine*, instance, _( "" ), _( "LootPanelVendingMachine" ), _( "Instance" ) );

		inline auto vending_machine( ) {
			il2cpp_method( ptr, _( "" ), _( "LootPanelVendingMachine" ), _( "GetVendingMachine" ), -1, c_invisible_vending_machine * ( * )( c_loot_panel_vending_machine* ) );
			return safe.call< c_invisible_vending_machine* >( ptr, this );
		}

		inline auto buy_button_clicked( int index, int multiplier = 1 ) {
			il2cpp_method( ptr, _( "" ), _( "LootPanelVendingMachine" ), _( "BuyButtonClicked" ), 2, void ( * )( c_loot_panel_vending_machine*, int, int ) );
			safe.call< void >( ptr, this, index, multiplier );
		}
	};
}
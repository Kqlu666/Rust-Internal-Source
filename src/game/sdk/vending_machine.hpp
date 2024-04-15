#pragma once

namespace sdk {
	class c_sell_order {
	public:
		inline auto in_stock( ) {
			return mem::read< int >( this + 0x20 );
		}
	};

	class c_sell_order_container {
	public:
		inline auto sell_orders( ) {
			return mem::read< unity::c_list< c_sell_order* >* >( this + 0x10 );
		}
	};

	class c_ui_market_terminal {
	public:
		inline auto buy_button_clicked( int index, int multiplier = 1 ) {
			il2cpp_method( ptr, _( "" ), _( "UIMarketTerminal" ), _( "BuyButtonClicked" ), 2, void ( * )( c_ui_market_terminal*, int, int ) );
			safe.call< void >( ptr, this, index, multiplier );
		}
	};

	class c_invisible_vending_machine : public c_base_combat_entity {
	public:
		il2cpp_field_get( c_sell_order_container*, sell_order_container, _( "" ), _( "VendingMachine" ), _( "sellOrders" ) );
	};
}
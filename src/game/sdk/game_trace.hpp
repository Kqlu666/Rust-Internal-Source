#pragma once

namespace sdk {
	class c_game_trace {
	public:
		static auto trace_all( sdk::c_hit_test* hit_test, unity::c_list< trace_info_t >* list, int layer_mask ) {
			il2cpp_method( ptr, _( "" ), _( "GameTrace" ), _( "TraceAll" ), 3, void( * )( c_hit_test*, unity::c_list< trace_info_t >*, int ) );
			safe.call< void >( ptr, hit_test, list, layer_mask );
		}
	};
}
#pragma once

namespace sdk {
	class c_item_icon {
	public:
		il2cpp_field_get( bool, queued_for_looting, _( "" ), _( "ItemIcon" ), _( "queuedForLooting" ) );

		inline auto run_timed_action( ) {
			il2cpp_method( ptr, _( "" ), _( "ItemIcon" ), _( "RunTimedAction" ), -1, void( * )( c_item_icon* ) );
			safe.call< void >( ptr, this );
		}
	};
}
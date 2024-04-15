#pragma once

#include "aimbot.hpp"

#include "misc.hpp"
#include "visuals.hpp"

namespace features {
	inline void loop( ) {
		if ( config::settings_panic )
			return;

		visuals.loop( );
	}
}
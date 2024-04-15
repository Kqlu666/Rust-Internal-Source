#pragma once

namespace sdk {

	enum class e_building_grade : int {
		none = -1,
		twigs,
		wood,
		stone,
		metal,
		top_tier,
		count
	};

	class c_building_block {
	public:
		inline auto upgrade( int grade, unsigned int skin ) {
			il2cpp_method( ptr, _( "" ), _( "BuildingBlock" ), _( "UpgradeToGrade" ), -1, bool( * )( c_building_block*, int, unsigned int, c_base_player* ) );
			safe.call< void >( ptr, this, grade, skin, local_player );
		}

		inline auto can_afford( int grade, unsigned int skin ) {
			il2cpp_method( ptr, _( "" ), _( "BuildingBlock" ), _( "CanAffordUpgrade" ), -1, bool( * )( c_building_block*, int, unsigned int, c_base_player* ) );
			return safe.call< bool >( ptr, this, grade, skin, local_player );
		}

		inline auto can_upgrade( int grade, unsigned int skin ) {
			il2cpp_method( ptr, _( "" ), _( "BuildingBlock" ), _( "CanChangeToGrade" ), -1, bool( * )( c_building_block*, int, unsigned int, c_base_player* ) );
			return safe.call< bool >( ptr, this, grade, skin, local_player );
		}
	};
}
#pragma once

namespace sdk {
	class c_item_definition : public unity::c_component {
	public:
		il2cpp_field_get( unsigned int, itemid, _( "" ), _( "ItemDefinition" ), _( "itemid" ) );
		il2cpp_field_get( bool, quick_despawn, _( "" ), _( "ItemDefinition" ), _( "quickDespawn" ) );
		il2cpp_field_get( e_item_category, category, _( "" ), _( "ItemDefinition" ), _( "category" ) );
		il2cpp_field_get( unity::c_string*, shortname, _( "" ), _( "ItemDefinition" ), _( "shortname" ) );
		il2cpp_field_get( c_translate_phase*, display_name, _( "" ), _( "ItemDefinition" ), _( "displayName" ) );
	};
}
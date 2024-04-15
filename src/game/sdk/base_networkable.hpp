#pragma once

namespace sdk {
	class c_base_entity;

	class c_entity_realm {
	public:
		inline auto entity_list( ) {
			return mem::read< unity::c_list_dictionary* >( this + 0x10 );
		}
	};

	class c_base_networkable : public unity::c_component {
	public:
		il2cpp_static_field_get( c_entity_realm*, client_entities, _( "" ), _( "BaseNetworkable" ), _( "clientEntities" ) );

		inline auto prefab( ) {
			il2cpp_method( ptr, _( "" ), _( "BaseNetworkable" ), _( "get_ShortPrefabName" ), -1, unity::c_string* ( * )( c_base_networkable* ) );
			return safe.call< unity::c_string* >( ptr, this );
		}

		inline auto has_parent( ) {
			il2cpp_method( ptr, _( "" ), _( "BaseNetworkable" ), _( "HasParent" ), -1, bool( * )( c_base_networkable* ) );
			return safe.call< bool >( ptr, this );
		}
	};
}
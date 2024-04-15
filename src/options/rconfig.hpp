#pragma once
#include <string>
#include <vector>

class rconfig {
public:
	rconfig( ) = default;
	rconfig( std::string file_path );

	void read( );
	void write( );

	bool is_stale( );

	template<class T>
	T get( std::string name, T def );

	template<class T>
	void set( std::string name, T value );

private:
	std::string get_record( uint32_t hash );
	void set_record( uint32_t hash, std::string value );

	uint32_t get_config_hash( );

	std::string path;
	std::string cfg;
	uint32_t last_hash;
};


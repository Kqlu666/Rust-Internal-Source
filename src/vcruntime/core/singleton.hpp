#pragma once

class no_copy {
public:
    no_copy( ) = default;
    no_copy( const no_copy& ) = delete;

protected:
    no_copy& operator=( const no_copy& ) = delete;
};

class no_move {
public:
    no_move( ) = default;
    no_move( no_move&& ) = delete;

protected:
    no_move& operator=( no_move&& ) = delete;
};

template < class T >
class singleton : public no_copy, public no_move {
public:
    static T& instance( ) {
        static T instance = { };
        return instance;
    }
};
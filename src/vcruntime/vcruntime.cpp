#include "include.hpp"

extern "C" {
    BOOL _Init_thread_epoch = 0;

    void _Init_thread_header( int32_t* const parameter ) noexcept { }
    void _Init_thread_abort( int32_t* const parameter ) noexcept { }
    void _Init_thread_footer( int32_t* const parameter ) noexcept { }
};

extern "C" {
    std::int32_t _fltused = 0x9875;
}; // extern "C"

using _PVFV = void ( * )( );

extern "C" {
    int atexit( _PVFV ) { return 0; }

    int _purecall( ) { return 0; }

}; // extern "C"

type_info::~type_info( ) { }

__declspec( noreturn ) void _invalid_parameter_noinfo_noreturn( ) { }

__declspec( noreturn ) void __cdecl terminate( ) { }

extern "C" {
    void __security_check_cookie( std::uintptr_t cookie ) { }

    void __chkstk( ) { }

    void __GSHandlerCheck( ) { }

    void __cdecl __std_terminate( ) { __debugbreak( ); }
}

extern "C" {

    EXCEPTION_DISPOSITION __C_specific_handler( PEXCEPTION_RECORD ExceptionRecord, PVOID EstablisherFrame,
                                                PCONTEXT ContextRecord, PDISPATCHER_CONTEXT DispatcherContext ) {
        return ExceptionContinueExecution;
    }

    EXCEPTION_DISPOSITION __CxxFrameHandler3( PEXCEPTION_RECORD ExceptionRecord, PVOID EstablisherFrame,
                                              PCONTEXT ContextRecord, PDISPATCHER_CONTEXT DispatcherContext ) {
        return ExceptionContinueExecution;
    }

    EXCEPTION_DISPOSITION __CxxFrameHandler4( PEXCEPTION_RECORD ExceptionRecord, PVOID EstablisherFrame,
                                              PCONTEXT ContextRecord, PDISPATCHER_CONTEXT DispatcherContext ) {
        return ExceptionContinueExecution;
    }

    void _CxxThrowException( void*, _ThrowInfo* ) { }

    void** __cdecl __current_exception( ) { return nullptr; }

    void** __cdecl __current_exception_context( ) { return nullptr; }

    __declspec( noreturn ) void __cdecl _invoke_watson( wchar_t const* const expression, wchar_t const* const function_name,
                                                        wchar_t const* const file_name, unsigned int const line_number, uintptr_t const reserved ) { }

    void _Xlength_error( const char* ) { }

    void _Xout_of_range( const char* ) { }


    void _guard_xfg_dispatch_icall_nop( ) { }

    void __std_exception_copy( struct __std_exception_data const*, struct __std_exception_data* ) { }

    void __std_exception_destroy( struct __std_exception_data* ) { }
}; // extern "C"

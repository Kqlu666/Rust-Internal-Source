#pragma once
#include "../safe/safe.hpp"

#define IMPORT_SYSTEM_CALL( SYSTEM_CALL_TYPE, SYSTEM_CALL_NAME, SYSTEM_CALL_PARAMS, ... ) \
                                                                                          \
  inline SYSTEM_CALL_TYPE SYSTEM_CALL_NAME##SYSTEM_CALL_PARAMS {                          \
    static auto system_call_number = safe.get_system_index( HASH( #SYSTEM_CALL_NAME ) );  \
                                                                                          \
    using SystemCallFn = SYSTEM_CALL_TYPE( * ) SYSTEM_CALL_PARAMS;                        \
    const auto procedure = safe._syscall< SystemCallFn >( system_call_number );           \
    return procedure( __VA_ARGS__ );                                                      \
  }

namespace nt {

    IMPORT_SYSTEM_CALL( NTSTATUS, NtQuerySystemInformation,
                        ( SYSTEM_INFORMATION_CLASS system_information_class, void* system_information, ULONG system_information_size,
                          PULONG size ),
                        system_information_class, system_information, system_information_size, size );
    IMPORT_SYSTEM_CALL( NTSTATUS, NtUserBuildHwndList,
                        ( HDESK desktop, ::HWND parent, BOOLEAN children, BOOLEAN unknown, ULONG thread_id, ULONG lparam, ::HWND* window,
                          ULONG* size ),
                        desktop, parent, children, unknown, thread_id, lparam, window, size );
    IMPORT_SYSTEM_CALL(
        NTSTATUS, NtDelayExecution, ( ::BOOLEAN alertable, PLARGE_INTEGER interval ), alertable, interval );
    IMPORT_SYSTEM_CALL(
        NTSTATUS, NtQueryPerformanceCounter, ( PLARGE_INTEGER counter, PLARGE_INTEGER frequency ), counter, frequency );

    IMPORT_SYSTEM_CALL( NTSTATUS, NtWriteFile,
                        ( HANDLE file_handle, HANDLE file_event, PIO_APC_ROUTINE apc_routine, void* apc_context,
                          PIO_STATUS_BLOCK io_status_block, void* buffer, ULONG size, PLARGE_INTEGER large, ULONG* key ),
                        file_handle, file_event, apc_routine, apc_context, io_status_block, buffer, size, large, key );
    IMPORT_SYSTEM_CALL( NTSTATUS, NtReadFile,
                        ( HANDLE file_handle, HANDLE file_event, PIO_APC_ROUTINE apc_routine, void* apc_context,
                          PIO_STATUS_BLOCK io_status_block, void* buffer, ULONG size, PLARGE_INTEGER large, ULONG* key ),
                        file_handle, file_event, apc_routine, apc_context, io_status_block, buffer, size, large, key );
    IMPORT_SYSTEM_CALL( NTSTATUS, NtQueryDirectoryFile,
                        ( HANDLE file_handle, HANDLE file_event, PIO_APC_ROUTINE apc_routine, void* apc_context,
                          PIO_STATUS_BLOCK io_status_block, void* buffer, std::uint32_t size, FILE_INFORMATION_CLASS information,
                          BOOLEAN return_single_entry, UNICODE_STRING* file_name, BOOLEAN restart_scan ),
                        file_handle, file_event, apc_routine, apc_context, io_status_block, buffer, size, information,
                        return_single_entry, file_name, restart_scan );

    IMPORT_SYSTEM_CALL( NTSTATUS, NtCreateFile,
                        ( HANDLE file_handle, ACCESS_MASK access, ::OBJECT_ATTRIBUTES* object_attributes,
                          IO_STATUS_BLOCK* io_status_block, LARGE_INTEGER* allocation_size, ULONG attributes, ULONG share,
                          ULONG disposition, ULONG options, void* buffer, ULONG size ),
                        file_handle, access, object_attributes, io_status_block, allocation_size, attributes, share, disposition, options,
                        buffer, size );
    IMPORT_SYSTEM_CALL( NTSTATUS, NtClose, ( HANDLE handle ), handle );

    IMPORT_SYSTEM_CALL( NTSTATUS, NtQueryInformationFile,
                        ( HANDLE handle, ::IO_STATUS_BLOCK* io_status_block, void* buffer, ULONG size,
                          FILE_INFORMATION_CLASS information_class ),
                        handle, io_status_block, buffer, size, information_class );
    IMPORT_SYSTEM_CALL( NTSTATUS, NtSetInformationFile,
                        ( HANDLE handle, ::IO_STATUS_BLOCK* io_status_block, void* buffer, ULONG size,
                          FILE_INFORMATION_CLASS information_class ),
                        handle, io_status_block, buffer, size, information_class );

    IMPORT_SYSTEM_CALL( NTSTATUS, NtReadVirtualMemory,
                        ( HANDLE process, void* address, void* buffer, ULONG size, ULONG* read ), process, address, buffer, size, read );
    IMPORT_SYSTEM_CALL( NTSTATUS, NtWriteVirtualMemory,
                        ( HANDLE process, void* address, void* buffer, ULONG size, ULONG* write ), process, address, buffer, size,
                        write );
    IMPORT_SYSTEM_CALL( NTSTATUS, NtProtectVirtualMemory,
                        ( HANDLE process, void** address, std::size_t* size, ULONG protect, ULONG* restore ), process, address, size,
                        protect, restore );
    IMPORT_SYSTEM_CALL( NTSTATUS, NtAllocateVirtualMemory,
                        ( HANDLE process, void** address, std::uintptr_t zero, std::size_t* size, ULONG allocation_type, ULONG protect ),
                        process, address, zero, size, allocation_type, protect );
    IMPORT_SYSTEM_CALL( NTSTATUS, NtFreeVirtualMemory, ( HANDLE process, void** address, std::size_t* size, ULONG type ),
                        process, address, size, type );
    IMPORT_SYSTEM_CALL( NTSTATUS, NtQueryVirtualMemory,
                        ( HANDLE process, void* address, DWORD i, void* buffer, std::size_t size, std::size_t* output_size ), process,
                        address, i, buffer, size, output_size );

    IMPORT_SYSTEM_CALL( NTSTATUS, NtSetInformationProcess,
                        ( HANDLE process, DWORD information_class, void* buffer, ULONG size ), process, information_class, buffer, size );
    IMPORT_SYSTEM_CALL( NTSTATUS, NtRaiseHardError,
                        ( NTSTATUS error, ULONG number_parameters, ::PUNICODE_STRING unicode, PVOID* parameters, DWORD option,
                          ULONG* response ),
                        error, number_parameters, unicode, parameters, option, response );
    IMPORT_SYSTEM_CALL( NTSTATUS, NtAdjustPrivilegesToken,
                        ( HANDLE process, BOOLEAN disable, ::PTOKEN_PRIVILEGES privileges, ULONG size, ::PTOKEN_PRIVILEGES previous,
                          ULONG* required ),
                        process, disable, privileges, size, previous, required );
    IMPORT_SYSTEM_CALL( NTSTATUS, NtOpenProcessToken, ( HANDLE process, DWORD access, HANDLE* process_token ), process,
                        access, process_token );
    IMPORT_SYSTEM_CALL( NTSTATUS, NtOpenProcess,
                        ( HANDLE process, ACCESS_MASK access, ::OBJECT_ATTRIBUTES* object_attributes, ::CLIENT_ID* client_id ), process,
                        access, object_attributes, client_id );
    IMPORT_SYSTEM_CALL( NTSTATUS, NtGetContextThread, ( HANDLE process, PCONTEXT context ), process, context );

    IMPORT_SYSTEM_CALL( NTSTATUS, NtQueryAttributesFile,
                        ( IN POBJECT_ATTRIBUTES ObjectAttributes, OUT PCHAR FileAttributes ), ObjectAttributes, FileAttributes )
        IMPORT_SYSTEM_CALL(
            NTSTATUS, NtQueryDefaultLocale, ( BOOLEAN UserProfile, PLCID DefaultLocaleId ), UserProfile, DefaultLocaleId );
    IMPORT_SYSTEM_CALL( NTSTATUS, NtCreateEvent,
                        ( _Out_ PHANDLE EventHandle, _In_ ACCESS_MASK DesiredAccess, _In_opt_ POBJECT_ATTRIBUTES ObjectAttributes,
                          _In_ DWORD EventType, _In_ BOOLEAN InitialState ),
                        EventHandle, DesiredAccess, ObjectAttributes, EventType, InitialState );

    IMPORT_SYSTEM_CALL( std::uintptr_t, NtUserCallTwoParam,
                        ( std::uintptr_t Param1, std::uintptr_t Param2, DWORD Routine ), Param1, Param2, Routine );
    IMPORT_SYSTEM_CALL( std::uintptr_t, NtUserCallOneParam, ( std::uintptr_t param, DWORD routine ), param, routine );
    IMPORT_SYSTEM_CALL( std::uintptr_t, NtUserGetThreadState, ( std::uintptr_t mode ), mode );

    IMPORT_SYSTEM_CALL( std::uintptr_t, NtUserQueryWindow, ( ::HWND window, DWORD i ), window, i );
    IMPORT_SYSTEM_CALL(
        std::uintptr_t, NtUserInjectMouseInput, ( ::MOUSEINPUT* mouse_input, std::int32_t size ), mouse_input, size );
    IMPORT_SYSTEM_CALL(
        std::uintptr_t, NtUserInjectKeyboardInput, ( ::KEYBDINPUT* keybd_input, std::int32_t size ), keybd_input, size );


    IMPORT_SYSTEM_CALL(
        std::uint32_t, NtUserSendInput, ( std::uint32_t count, ::LPINPUT input, std::int32_t size ), count, input, size );
    IMPORT_SYSTEM_CALL( std::uint32_t, NtUserGetKeyNameText, ( std::int32_t lparam, LPWSTR string, std::int32_t size ),
                        lparam, string, size );
    IMPORT_SYSTEM_CALL( std::uint32_t, NtUserMapVirtualKeyEx,
                        ( std::uint32_t code, std::uint32_t type, DWORD keyboard_id, ::HKL hkl ), code, type, keyboard_id, hkl );

    IMPORT_SYSTEM_CALL( std::int32_t, NtUserUnhookWindowsHookEx, ( ::HHOOK windows_hook ), windows_hook );
    IMPORT_SYSTEM_CALL( std::int32_t, NtUserGetWindowPlacement, ( ::HWND window, WINDOWPLACEMENT* window_placement ),
                        window, window_placement );
    IMPORT_SYSTEM_CALL( std::int32_t, NtUserGetCursorInfo, ( ::PCURSORINFO cursor_info ), cursor_info );
    IMPORT_SYSTEM_CALL( std::int32_t, NtUserGetClassName, ( ::HWND window, ::BOOL real, UNICODE_STRING* class_name ),
                        window, real, class_name );
    IMPORT_SYSTEM_CALL( std::int32_t, NtGdiGetDeviceCaps, ( ::HDC hdc, std::int32_t i ), hdc, i );

    IMPORT_SYSTEM_CALL( std::int16_t, NtUserGetAsyncKeyState, ( int key ), key );
    IMPORT_SYSTEM_CALL( std::int16_t, NtUserGetKeyState, ( int key ), key );
    IMPORT_SYSTEM_CALL( std::int16_t, NtUserGetDpiForCurrentProcess, ( ) );

    IMPORT_SYSTEM_CALL( ::LRESULT, NtUserCallNextHookEx, ( int code, WPARAM wparam, LPARAM lparam, ::BOOL ansi ), code,
                        wparam, lparam, ansi );

    IMPORT_SYSTEM_CALL( ::HHOOK, NtUserSetWindowsHookEx,
                        ( ::HMODULE image, UNICODE_STRING* image_name, DWORD thread_id, int hooked_id, ::HOOKPROC hooked_procedure,
                          ::BOOL ansi ),
                        image, image_name, thread_id, hooked_id, hooked_procedure, ansi );
    IMPORT_SYSTEM_CALL( ::HWND, NtUserGetForegroundWindow, ( ) );
    IMPORT_SYSTEM_CALL( ::HCURSOR, NtUserGetCursor, ( ::HCURSOR cursor ), cursor );
    IMPORT_SYSTEM_CALL( ::HCURSOR, NtUserSetCursor, ( ::HCURSOR cursor ), cursor );
    IMPORT_SYSTEM_CALL( ::HDC, NtUserGetDC, ( ::HWND window ), window );
    IMPORT_SYSTEM_CALL( ::UINT64, NtUserGetCPD, ( ::HWND window, int a2, unsigned long long a3 ), window, a2, a3 );

    IMPORT_SYSTEM_CALL( ::LONG_PTR, NtUserSetWindowLongPtr,
                        ( ::HWND window, unsigned long long id, unsigned long long ptr, unsigned int unk ), window, id, ptr, unk );

} // namespace nt

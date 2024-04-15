#include "../nt/nt.hpp"
#include "../nt/functions.hpp"

namespace proc {

	HANDLE NtCurrentProcess( ) noexcept;
	HANDLE NtCurrentThread( ) noexcept;
	HANDLE NtCurrentSession( ) noexcept;

	nt::TEB* NtCurrentTeb( ) noexcept;
	nt::PEB* NtCurrentPeb( ) noexcept;

	uint32_t NtCurrentProcessId( ) noexcept;
	uint32_t NtCurrentThreadId( ) noexcept;

	int32_t GetLastStatus( ) noexcept;
	void SetLastStatus( int32_t last_status ) noexcept;

	uint32_t GetLastError( ) noexcept;
	void SetLastError( uint32_t last_error ) noexcept;

	void* GetProcessHeap( ) noexcept;
} // namespace proc
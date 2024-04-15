#pragma once

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif // !_CRT_SECURE_NO_WARNINGS

#ifndef _CRT_NON_CONFORMING_SWPRINTFS
#define _CRT_NON_CONFORMING_SWPRINTFS
#endif // _CRT_NON_CONFORMING_SWPRINTFS

#pragma warning( push )
#pragma warning( disable : 4530 )

#include <memory>
#include <chrono>
#include <thread>
#include <limits>
#include <string>
#include <vector>
#include <algorithm>

#pragma warning( pop )

#pragma warning( disable : 26812 )

#define INLINE      inline
#define FORCEINLINE __forceinline

#define JOIN_IMPL( A, B ) A##B
#define JOIN( A, B )      JOIN_IMPL( A, B )

#define FIELD_PAD( Size ) uint8_t JOIN( __pad, __COUNTER__ )[Size] = { }

#define FUNCTION _( __FUNCTION__ )
#define FN       _( __FUNCTION__ )

#define PAGE_SIZE 0x1000

#ifndef NOMINMAX
#define NOMINMAX
#endif

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winternl.h>
#include <winioctl.h>

#include <intrin.h>

#include <heapapi.h>

#pragma warning( push )
#pragma warning( disable : 4005 )
#include <ntstatus.h>

#include <d3d.h>
#include <d3d11.h>

#pragma warning( pop )

#undef LOBYTE
#undef HIBYTE
#undef LOWORD
#undef HIWORD
#undef LODWORD
#undef HIDWORD

#undef BYTE1
#undef BYTE2
#undef WORD1
#undef WORD2

//
// unsigned
//

#define BYTE_ELEMENT( Data, Index )  ( *( (std::uint8_t*)&( Data ) + Index ) )
#define WORD_ELEMENT( Data, Index )  ( *( (std::uint16_t*)&( Data ) + Index ) )
#define DWORD_ELEMENT( Data, Index ) ( *( (std::uint32_t*)&( Data ) + Index ) )

#define LOBYTE( Data ) BYTE_ELEMENT( Data, 0 )
#define HIBYTE( Data ) BYTE_ELEMENT( Data, 1 )

#define LOWORD( Data ) WORD_ELEMENT( Data, 0 )
#define HIWORD( Data ) WORD_ELEMENT( Data, 1 )

#define LODWORD( Data ) DWORD_ELEMENT( Data, 0 )
#define HIDWORD( Data ) DWORD_ELEMENT( Data, 1 )

#define BYTE1( Data ) BYTE_ELEMENT( Data, 1 )
#define BYTE2( Data ) BYTE_ELEMENT( Data, 2 )

#define WORD1( Data ) WORD_ELEMENT( Data, 1 )
#define WORD2( Data ) WORD_ELEMENT( Data, 2 )

#define DWORD1( Data ) DWORD_ELEMENT( Data, 1 )
#define DWORD2( Data ) DWORD_ELEMENT( Data, 2 )

//
// signed
//

#define SBYTE_ELEMENT( Data, Index )  ( *( (std::int8_t*)&( Data ) + Index ) )
#define SWORD_ELEMENT( Data, Index )  ( *( (std::int16_t*)&( Data ) + Index ) )
#define SDWORD_ELEMENT( Data, Index ) ( *( (std::int32_t*)&( Data ) + Index ) )

#define SLOBYTE( Data ) SBYTE_ELEMENT( Data, 0 )
#define SHIBYTE( Data ) SBYTE_ELEMENT( Data, 1 )

#define SLOWORD( Data ) SWORD_ELEMENT( Data, 0 )
#define SHIWORD( Data ) SWORD_ELEMENT( Data, 1 )

#define SLODWORD( Data ) SDWORD_ELEMENT( Data, 0 )
#define SHIDWORD( Data ) SDWORD_ELEMENT( Data, 1 )

#define SBYTE1( Data ) SBYTE_ELEMENT( Data, 1 )
#define SBYTE2( Data ) SBYTE_ELEMENT( Data, 2 )

#define SWORD1( Data ) SWORD_ELEMENT( Data, 1 )
#define SWORD2( Data ) SWORD_ELEMENT( Data, 2 )

#define SDWORD1( Data ) SDWORD_ELEMENT( Data, 1 )
#define SDWORD2( Data ) SDWORD_ELEMENT( Data, 2 )

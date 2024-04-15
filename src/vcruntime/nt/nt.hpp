#pragma once

#include "../include.hpp"

namespace nt {

    struct ACTIVATION_CONTEXT;
    struct PEB;
    struct TEB;

    struct RTL_ACTIVATION_CONTEXT_STACK_FRAME {
        RTL_ACTIVATION_CONTEXT_STACK_FRAME* Previous = nullptr; // 0x0000
        ACTIVATION_CONTEXT* ActivationContext = nullptr;        // 0x0008
        std::uint32_t Flags = 0;                                // 0x0010
        FIELD_PAD( 0x0004 );                                    // 0x0014
    };

    struct ACTIVATION_CONTEXT_STACK {
        RTL_ACTIVATION_CONTEXT_STACK_FRAME* ActiveFrame = nullptr; // 0x0000
        LIST_ENTRY FrameListCache = { };                           // 0x0008
        std::uint32_t Flags = 0;                                   // 0x0018
        std::uint32_t NextCookieSequenceNumber = 0;                // 0x001C
        std::uint32_t StackId = 0;                                 // 0x0020
        FIELD_PAD( 0x0004 );                                       // 0x0024
    };

    struct GDI_TEB_BATCH {
        struct {
            std::uint32_t Offset : 31;
            std::uint32_t HasRenderingCommand : 1;
        };
        std::int32_t Padding_73 = 0;
        std::uint64_t HDC = 0;
        std::uint32_t Buffer[ 310 ] = { };
    };

    struct TEB {
        NT_TIB NtTib = { };
        void* EnvironmentPointer = nullptr;
        CLIENT_ID ClientId = { };
        void* ActiveRpcHandle = nullptr;
        void* ThreadLocalStoragePointer = nullptr;
        PEB* ProcessEnvironmentBlock = nullptr;
        std::uint32_t LastErrorValue = 0;
        std::uint32_t CountOfOwnedCriticalSections = 0;
        void* CsrClientThread = nullptr;
        void* Win32ThreadInfo = nullptr;
        std::uint32_t User32Reserved[ 26 ] = { };
        std::uint32_t UserReserved[ 5 ] = { };
        FIELD_PAD( 0x0004 );
        void* WOW32Reserved = nullptr;
        std::uint32_t CurrentLocale = 0;
        std::uint32_t FpSoftwareStatusRegister = 0;
        void* ReservedForDebuggerInstrumentation[ 16 ] = { };
        void* SystemReserved1[ 30 ] = { };
        std::int8_t PlaceholderCompatibilityMode = 0;
        std::uint8_t PlaceholderHydrationAlwaysExplicit = 0;
        std::int8_t PlaceholderReserved[ 10 ] = { };
        std::uint32_t ProxiedProcessId = 0;
        ACTIVATION_CONTEXT_STACK _ActivationStack = { };
        std::uint8_t WorkingOnBehalfTicket[ 8 ] = { };
        std::int32_t ExceptionCode = 0;
        std::uint8_t Padding0[ 4 ] = { };
        ACTIVATION_CONTEXT_STACK* ActivationContextStackPointer = nullptr;
        std::uint64_t InstrumentationCallbackSp = 0;
        std::uint64_t InstrumentationCallbackPreviousPc = 0;
        std::uint64_t InstrumentationCallbackPreviousSp = 0;
        std::uint32_t TxFsContext = 0;
        std::uint8_t InstrumentationCallbackDisabled = 0;
        std::uint8_t UnalignedLoadStoreExceptions = 0;
        std::uint8_t Padding1[ 2 ] = { };
        GDI_TEB_BATCH GdiTebBatch = { };
        CLIENT_ID RealClientId = { };
        void* GdiCachedProcessHandle = nullptr;
        std::uint32_t GdiClientPID = 0;
        std::uint32_t GdiClientTID = 0;
        void* GdiThreadLocalInfo = nullptr;
        std::uint64_t Win32ClientInfo[ 62 ] = { };
        void* glDispatchTable[ 233 ] = { };
        std::uint64_t glReserved1[ 29 ] = { };
        void* glReserved2 = nullptr;
        void* glSectionInfo = nullptr;
        void* glSection = nullptr;
        void* glTable = nullptr;
        void* glCurrentRC = nullptr;
        void* glContext = nullptr;
        std::uint32_t LastStatusValue = 0;
        std::uint8_t Padding2[ 4 ] = { };
        UNICODE_STRING StaticUnicodeString = { };
        wchar_t StaticUnicodeBuffer[ 261 ] = { };
        std::uint8_t Padding3[ 6 ] = { };
        void* DeallocationStack = nullptr;
        void* TlsSlots[ 64 ] = { };
        LIST_ENTRY TlsLinks = { };
        void* Vdm = nullptr;
        void* ReservedForNtRpc = nullptr;
        void* DbgSsReserved[ 2 ] = { };
        std::uint32_t HardErrorMode = 0;
        std::uint8_t Padding4[ 4 ] = { };
        void* Instrumentation[ 11 ] = { };
        GUID ActivityId = { };
        void* SubProcessTag = nullptr;
        void* PerflibData = nullptr;
        void* EtwTraceData = nullptr;
        void* WinSockData = nullptr;
        std::uint32_t GdiBatchCount = 0;
        union {
            PROCESSOR_NUMBER CurrentIdealProcessor = { };
            std::uint32_t IdealProcessorValue;
            struct {
                std::uint8_t ReservedPad0;
                std::uint8_t ReservedPad1;
                std::uint8_t ReservedPad2;
                std::uint8_t IdealProcessor;
            };
        };
        std::uint32_t GuaranteedStackBytes = 0;
        std::uint8_t Padding5[ 4 ] = { };
        void* ReservedForPerf = nullptr;
        void* ReservedForOle = nullptr;
        std::uint32_t WaitingOnLoaderLock = 0;
        std::uint8_t Padding6[ 4 ] = { };
        void* SavedPriorityState = nullptr;
        std::uint64_t ReservedForCodeCoverage = 0;
        void* ThreadPoolData = nullptr;
        void** TlsExpansionSlots = nullptr;
        void* DeallocationBStore = nullptr;
        void* BStoreLimit = nullptr;
        std::uint32_t MuiGeneration = 0;
        std::uint32_t IsImpersonating = 0;
        void* NlsCache = nullptr;
        void* pShimData = nullptr;
        std::uint32_t HeapData = 0;
        std::uint8_t Padding7[ 4 ] = { };
        void* CurrentTransactionHandle = nullptr;
        void* ActiveFrame = nullptr;
        void* FlsData = nullptr;
        void* PreferredLanguages = nullptr;
        void* UserPrefLanguages = nullptr;
        void* MergedPrefLanguages = nullptr;
        std::uint32_t MuiImpersonation = 0;
        union {
            volatile std::uint16_t CrossTebFlags = 0;
            std::uint16_t SpareCrossTebBits : 16;
        };
        union {
            std::uint16_t SameTebFlags = 0;
            struct {
                std::uint16_t SafeThunkCall : 1;
                std::uint16_t InDebugPrint : 1;
                std::uint16_t HasFiberData : 1;
                std::uint16_t SkipThreadAttach : 1;
                std::uint16_t WerInShipAssertCode : 1;
                std::uint16_t RanProcessInit : 1;
                std::uint16_t ClonedThread : 1;
                std::uint16_t SupressDebugMsg : 1;
                std::uint16_t DisableUserStackWalk : 1;
                std::uint16_t RtlExceptionAttached : 1;
                std::uint16_t InitialThread : 1;
                std::uint16_t SessionAware : 1;
                std::uint16_t LoadOwner : 1;
                std::uint16_t LoaderWorker : 1;
                std::uint16_t SkipLoaderInit : 1;
                std::uint16_t SpareSameTebBits : 1;
            };
        };
        void* TxnScopeEnterCallback = nullptr;
        void* TxnScopeExitCallback = nullptr;
        void* TxnScopeContext = nullptr;
        std::uint32_t LockCount = 0;
        std::int32_t WowTebOffset = 0;
        void* ResourceRetValue = nullptr;
        void* ReservedForWdf = nullptr;
        std::uint64_t ReservedForCrt = 0;
        GUID EffectiveContainerId = { };
    };

    struct PEB_LDR_DATA {
        std::uint32_t Length = 0;                         // 0x0000
        std::uint8_t Initialized = 0;                     // 0x0004
        FIELD_PAD( 0x0003 );                              // 0x0005
        void* SsHandle = nullptr;                         // 0x0008
        LIST_ENTRY InLoadOrderModuleList = { };           // 0x0010
        LIST_ENTRY InMemoryOrderModuleList = { };         // 0x0020
        LIST_ENTRY InInitializationOrderModuleList = { }; // 0x0030
        void* EntryInProgress = nullptr;                  // 0x0040
        std::uint8_t ShutdownInProgress = 0;              // 0x0048
        FIELD_PAD( 0x0007 );                              // 0x0049
        void* ShutdownThreadId = nullptr;                 // 0x0050
    };

    struct PEB {
        std::uint8_t InheritedAddressSpace = 0;
        std::uint8_t ReadImageFileExecOptions = 0;
        std::uint8_t BeingDebugged = 0;
        union {
            std::uint8_t BitField = 0;
            struct {
                std::uint8_t ImageUsesLargePages : 1;
                std::uint8_t IsProtectedProcess : 1;
                std::uint8_t IsImageDynamicallyRelocated : 1;
                std::uint8_t SkipPatchingUser32Forwarders : 1;
                std::uint8_t IsPackagedProcess : 1;
                std::uint8_t IsAppContainer : 1;
                std::uint8_t IsProtectedProcessLight : 1;
                std::uint8_t IsLongPathAwareProcess : 1;
            };
        };
        FIELD_PAD( 0x0004 );
        void* Mutant = nullptr;
        void* ImageBaseAddress = nullptr;
        PEB_LDR_DATA* Ldr = nullptr;
        RTL_USER_PROCESS_PARAMETERS* ProcessParameters = nullptr;
        void* SubSystemData = nullptr;
        void* ProcessHeap = nullptr;
        void* FastPebLock = nullptr;
        void* volatile AtlThunkSListPtr = nullptr;
        void* IFEOKey = nullptr;
    };

    enum LDR_DLL_LOAD_REASON {
        LoadReasonStaticDependency = 0,
        LoadReasonStaticForwarderDependency = 1,
        LoadReasonDynamicForwarderDependency = 2,
        LoadReasonDelayloadDependency = 3,
        LoadReasonDynamicLoad = 4,
        LoadReasonAsImageLoad = 5,
        LoadReasonAsDataLoad = 6,
        LoadReasonEnclavePrimary = 7,
        LoadReasonEnclaveDependency = 8,
        LoadReasonUnknown = -1,
    };

    struct RTL_BALANCED_NODE {
        union {
            RTL_BALANCED_NODE* Children[ 2 ] = { };
            struct {
                RTL_BALANCED_NODE* Left;
                RTL_BALANCED_NODE* Right;
            };
        };
        union {
            std::uint8_t Red : 1;
            std::uint8_t Balance : 2;
            std::uint64_t ParentValue = 0;
        };
    };

    struct LDR_DATA_TABLE_ENTRY {
        LIST_ENTRY InLoadOrderLinks = { };
        LIST_ENTRY InMemoryOrderLinks = { };
        LIST_ENTRY InInitializationOrderLinks = { };
        void* DllBase = nullptr;
        void* EntryPoint = nullptr;
        std::uint32_t SizeOfImage = 0;
        FIELD_PAD( 0x0004 );
        UNICODE_STRING FullDllName = { };
        UNICODE_STRING BaseDllName = { };
        union {
            std::uint8_t FlagGroup[ 4 ] = { };
            std::uint32_t Flags;
            struct {
                std::uint32_t PackagedBinary : 1;
                std::uint32_t MarkedForRemoval : 1;
                std::uint32_t ImageDll : 1;
                std::uint32_t LoadNotificationsSent : 1;
                std::uint32_t TelemetryEntryProcessed : 1;
                std::uint32_t ProcessStaticImport : 1;
                std::uint32_t InLegacyLists : 1;
                std::uint32_t InIndexes : 1;
                std::uint32_t ShimDll : 1;
                std::uint32_t InExceptionTable : 1;
                std::uint32_t ReservedFlags1 : 2;
                std::uint32_t LoadInProgress : 1;
                std::uint32_t LoadConfigProcessed : 1;
                std::uint32_t EntryProcessed : 1;
                std::uint32_t ProtectDelayLoad : 1;
                std::uint32_t ReservedFlags3 : 2;
                std::uint32_t DontCallForThreads : 1;
                std::uint32_t ProcessAttachCalled : 1;
                std::uint32_t ProcessAttachFailed : 1;
                std::uint32_t CorDeferredValidate : 1;
                std::uint32_t CorImage : 1;
                std::uint32_t DontRelocate : 1;
                std::uint32_t CorILOnly : 1;
                std::uint32_t ReservedFlags5 : 2;
                std::uint32_t Redirected : 1;
                std::uint32_t ReservedFlags6 : 2;
                std::uint32_t CompatDatabaseProcessed : 1;
            };
        };
        std::uint16_t ObsoleteLoadCount = 0;
        std::uint16_t TlsIndex = 0;
        LIST_ENTRY HashLinks = { };
        std::uint32_t TimeDateStamp = 0;
        FIELD_PAD( 0x0004 );
        void* EntryPointActivationContext = nullptr;
        void* Lock = nullptr;
        void* DdagNode = nullptr;
        LIST_ENTRY NodeModuleLink = { };
        void* LoadContext = nullptr;
        void* ParentDllBase = nullptr;
        void* SwitchBackContext = nullptr;
        RTL_BALANCED_NODE BaseAddressIndexNode = { };
        RTL_BALANCED_NODE MappingInfoIndexNode = { };
        std::uint64_t OriginalBase = 0;
        LARGE_INTEGER LoadTime = { };
        std::uint32_t BaseNameHashValue = 0;
        LDR_DLL_LOAD_REASON LoadReason = LoadReasonUnknown;
        std::uint32_t ImplicitPathOptions = 0;
        std::uint32_t ReferenceCount = 0;
        std::uint32_t DependentLoadFlags = 0;
        std::uint8_t SigningLevel = 0;
        FIELD_PAD( 0x0003 );
    };

    __forceinline static PTEB get_teb( ) noexcept {
        return reinterpret_cast< PTEB >( __readgsqword( FIELD_OFFSET( NT_TIB, Self ) ) );
    }
    __forceinline static PEB* get_peb( ) noexcept {
        return reinterpret_cast< PEB* >( get_teb( )->ProcessEnvironmentBlock );
    }

} // namespace nt

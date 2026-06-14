#pragma once
#include <ntifs.h>
#include<guiddef.h>
#include <ntimage.h>
#define MAX_SHADOW_PAGE 128
#define NO_SHADOW_PAGE (UINT32)-1
#define JMP_POOL 'JpOl'
#define INT_3 0xCC
#define DEF_PTR(type,baseAddress,offset) (*(type*)(((UCHAR*)(baseAddress))+((__int64)(offset))))
#define PTR_ADD(type,baseAddress,offset) ((type)(((UCHAR*)(baseAddress))+((__int64)(offset))))
#define GET_4KB_PAGE_BASE(VirtualAddress) (((((UINT64)VirtualAddress) >> 12) << 12))
#define GET_PAGE_ALIGN_LENGTH(MapSize) (((MapSize)+0xFFFULL) &~0xFFFULL)
#define GET_PAGE_OFFSET(VirtualAddress) ((DWORD_PTR)(VirtualAddress) & 0x0FFF)
#define GET_2MB_PAGE_BASE(VirtualAddress) (((((UINT64)(VirtualAddress)) >> 21) << 21))
typedef struct _MEMORY_INFO
{
    PVOID VirtualAddress;
    PHYSICAL_ADDRESS PhysicalAddress;
    SIZE_T Size;
}MEMORY_INFO, * PMEMORY_INFO;
typedef struct _CPU_CONTEXT
CPU_CONTEXT, * PCPU_CONTEXT;
typedef struct _HOOK_FUNC_INFO
{
    LIST_ENTRY HookFuncList;
    PVOID OriginalFuncAddress;
    PVOID HookFuncAddress;
    PVOID JumpTrampolineAddress;
    SIZE_T JumpTrampolineOffset;
    SIZE_T JumpTrampolineSize;
    SIZE_T FuncLength;
    UINT64 HookCount;
}HOOK_FUNC_INFO, * PHOOK_FUNC_INFO;
typedef struct _HOOK_PAGE_INFO
{
    MEMORY_INFO PageBaseInfo;
    MEMORY_INFO ShadowPageBaseInfo[MAX_SHADOW_PAGE];
}HOOK_PAGE_INFO, * PHOOK_PAGE_INFO;
typedef struct _HOOK_INFO
{
    LIST_ENTRY HookList;
    SIZE_T CbSize;
    CHAR TagStr[64];
    GUID HookId;
    SIZE_T PageBaseCount;
    BOOLEAN DataPage;
    PHOOK_PAGE_INFO HookPageInfo;
    struct
    {
        LIST_ENTRY HookFuncListHead;
        KSPIN_LOCK HookFuncListLock;
    }HookFuncList;
    volatile UINT64 RefCount;
}HOOK_INFO, * PHOOK_INFO;
#pragma pack(push,1)
typedef struct _JMP_CODE_INFO
{
    struct
    {
        UINT8 Reserved[6];
    }Execute;
    struct
    {
        UINT8 JmpAddress[8];
    }Data;
}JMP_CODE_INFO, * PJMP_CODE_INFO;
typedef struct _JMP_FUNC_TRAMPOLINE
{
    struct
    {
        UINT8 HookFuncAddress[8];
        UINT8 CallbackAddress[8];
        UINT8 ReturnAddress[8];
    }Data;
    struct
    {
        UINT8 Reserved1[0x109];
        UINT8 OriginalCode[90];
        UINT8 Reserved2[0x21];
    }Execute;
} JMP_FUNC_TRAMPOLINE, * PJMP_FUNC_TRAMPOLINE;
#pragma pack(pop)
typedef struct _HOOK_REGS
{
    UINT64 Rcx;
    UINT64 Rdx;
    UINT64 R8;
    UINT64 R9;
    UINT64 Param5;
    UINT64 Param6;
    UINT64 Param7;
}HOOK_REGS, * PHOOK_REGS;
EXTERN_C _declspec(dllimport) PCPU_CONTEXT g_CpuContexts;
EXTERN_C _declspec(dllimport) BOOLEAN g_VmStart;
EXTERN_C _declspec(dllimport) BOOLEAN g_Test;
EXTERN_C _declspec(dllimport) BOOLEAN g_Test1;
EXTERN_C _declspec(dllimport) BOOLEAN g_Unload;
EXTERN_C _declspec(dllimport) BOOLEAN g_bDebug;
EXTERN_C _declspec(dllimport) volatile ULONG CpuCount;
EXTERN_C _declspec(dllimport) UINT8 GetInstructionLength(PVOID CodeAddr, UINT8 MaxLength);
EXTERN_C _declspec(dllimport) void SvmGetJmpCodeBuffer(PVOID Buffer, size_t Length);
EXTERN_C _declspec(dllimport) UINT64 SvmGetJmpCodeBufferLength();
EXTERN_C _declspec(dllimport) void SvmGetJmpCodeFuncBuffer(PVOID Buffer, size_t Length);
EXTERN_C _declspec(dllimport) UINT64 SvmGetJmpCodeFuncBufferLength();
EXTERN_C _declspec(dllimport) PVOID SvmAllocateJmpTrampoline(PHOOK_FUNC_INFO HookFuncInfo, SIZE_T Length);
EXTERN_C _declspec(dllimport) void SvmFreeJmpTrampoline(PHOOK_FUNC_INFO HookFuncInfo);
EXTERN_C _declspec(dllimport) PHOOK_INFO SvmAddHookInfo(PCSTR TagStr, UINT64 VirtualAddress, SIZE_T MapSize);
EXTERN_C _declspec(dllimport) PHOOK_INFO SvmFindHookInfoTag(PCSTR TagStr);
EXTERN_C _declspec(dllimport) PHOOK_INFO SvmFindHookInfoGuid(PGUID HookId);
EXTERN_C _declspec(dllimport) BOOLEAN SvmCreateShadowPage(PHOOK_INFO HookInfo, UINT32 ShadowPageIndex);
EXTERN_C _declspec(dllimport) BOOLEAN SvmSetGuestShadowPage(PCPU_CONTEXT CpuContext, PHOOK_INFO HookInfo, UINT32 ShadowPageIndex, BOOLEAN NoExecute, BOOLEAN Write);
EXTERN_C _declspec(dllimport) BOOLEAN SvmShadowCopyMemory(PHOOK_INFO HookInfo, UINT32 ShadowPageIndex, UINT64 VirtualAddress, PVOID Data, SIZE_T DataSize);
EXTERN_C _declspec(dllimport) PHOOK_FUNC_INFO SvmAddHookFuncInfo(PHOOK_INFO hookInfo, UINT64 OriginalFuncAddress, UINT64 HookFuncAddress, SIZE_T FuncLength);
EXTERN_C _declspec(dllimport) PHOOK_FUNC_INFO SvmFindHookFuncInfo(PHOOK_INFO hookInfo, UINT64 RipAddress, BOOLEAN Lock);
EXTERN_C _declspec(dllimport) void SvmFreeGuestShadowPage(PHOOK_INFO HookInfo, UINT32 ShadowPageIndex);
EXTERN_C _declspec(dllimport) void SvmHookReference(PHOOK_INFO HookInfo);
EXTERN_C _declspec(dllimport) void SvmHookDereference(PHOOK_INFO HookInfo);
EXTERN_C _declspec(dllimport) PHOOK_INFO SvmFindHookInfoPageBase(PLIST_ENTRY ListHead, UINT64 VirtualAddress);
EXTERN_C _declspec(dllimport) PHOOK_FUNC_INFO SvmFindHookFuncInfoByJmpTrampoline(PHOOK_INFO hookInfo, UINT64 JmpTrampolineRipAddress, BOOLEAN Lock);
EXTERN_C _declspec(dllimport) BOOLEAN SvmIsHookRefCountZero(PHOOK_INFO HookInfo);
EXTERN_C _declspec(dllimport) PHOOK_INFO SvmEnumNextHookInfo(PHOOK_INFO CurrentHookInfo);
EXTERN_C _declspec(dllimport) void SvmRemoveHookFuncInfo(PHOOK_INFO hookInfo, PHOOK_FUNC_INFO funcInfo, BOOLEAN Lock);
EXTERN_C _declspec(dllimport) BOOLEAN SvmProtectDriverSection(UINT64 VirtualAddress, SIZE_T Size, BOOLEAN NoExecute, PBOOLEAN CoreStatus, UINT32 CoreCount);
EXTERN_C _declspec(dllimport) UINT64 SvmGetReturnOffset();
EXTERN_C _declspec(dllimport) NTSTATUS __stdcall test();
EXTERN_C _declspec(dllimport) NTSTATUS __stdcall test2();
#pragma once
#include <ntifs.h>
#include "PTE.h"
#include<guiddef.h>
#include <ntimage.h>
#define MAX_SHADOW_PAGE 128
#define NO_SHADOW_PAGE (UINT32)-1
#define JMP_POOL 'JpOl'
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
}HOOK_PAGE_INFO,*PHOOK_PAGE_INFO;
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
}JMP_CODE_INFO,*PJMP_CODE_INFO;
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
PHOOK_INFO AddHookInfo(PLIST_ENTRY ListHead, PCSTR TagStr, UINT64 VirtualAddress, SIZE_T MapSize, PKSPIN_LOCK HookListLock);
PHOOK_INFO FindHookInfoTag(PLIST_ENTRY ListHead, PCSTR TagStr, PKSPIN_LOCK HookListLock);
PHOOK_INFO FindHookInfoGuid(PLIST_ENTRY ListHead, PGUID HookId, PKSPIN_LOCK HookListLock);
BOOLEAN CreateShadowPage(PHOOK_INFO HookInfo, UINT32 ShadowPageIndex);
BOOLEAN ShadowCopyMemory(PHOOK_INFO HookInfo, UINT32 ShadowPageIndex, UINT64 VirtualAddress, PVOID Data, SIZE_T DataSize);
BOOLEAN SetGuestShadowPage(PCPU_CONTEXT CpuContext, PHOOK_INFO HookInfo, UINT32 ShadowPageIndex, BOOLEAN NoExecute, BOOLEAN Write, PUINT32 TlbControl);
void FreeGuestShadowPage(PHOOK_INFO HookInfo, UINT32 ShadowPageIndex);
PHOOK_FUNC_INFO AddHookFuncInfo(PHOOK_INFO hookInfo, UINT64 OriginalFuncAddress, UINT64 HookFuncAddress, SIZE_T FuncLength);
PHOOK_FUNC_INFO FindHookFuncInfo(PHOOK_INFO hookInfo, UINT64 RipAddress, BOOLEAN Lock);
void HookReference(PHOOK_INFO HookInfo);
void HookDereference(PHOOK_INFO HookInfo);
PHOOK_INFO FindHookInfoPageBase(PLIST_ENTRY ListHead, UINT64 VirtualAddress, PKSPIN_LOCK HookListLock);
PHOOK_FUNC_INFO FindHookFuncInfoByJmpTrampoline(PHOOK_INFO hookInfo, UINT64 JmpTrampolineRipAddress, BOOLEAN Lock);
BOOLEAN IsHookRefCountZero(PHOOK_INFO HookInfo);
PHOOK_INFO EnumNextHookInfo(PLIST_ENTRY ListHead, PHOOK_INFO CurrentHookInfo, PKSPIN_LOCK HookListLock);
PVOID AllocateJmpTrampoline(PHOOK_FUNC_INFO HookFuncInfo, SIZE_T Length);
void FreeJmpTrampoline(PHOOK_FUNC_INFO HookFuncInfo);
void RemoveHookFuncInfo(PHOOK_INFO hookInfo, PHOOK_FUNC_INFO funcInfo, BOOLEAN Lock);
void RemoveAllHookFuncInfo(PHOOK_INFO hookInfo, BOOLEAN Lock);
void RemoveAllHookInfo(PLIST_ENTRY ListHead, PKSPIN_LOCK HookListLock);

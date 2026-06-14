#pragma once
#include <ntifs.h>
#include "Hook.h"
#include "asmfunc.h"
EXTERN_C LIST_ENTRY HookListHead;
EXTERN_C KSPIN_LOCK HookListLock;
EXTERN_C _declspec(dllexport) BOOLEAN g_Test;
EXTERN_C _declspec(dllexport) BOOLEAN g_Test1;
EXTERN_C _declspec(dllexport) BOOLEAN g_bDebug;
EXTERN_C _declspec(dllexport) BOOLEAN g_Unload;
EXTERN_C _declspec(dllexport) void SvmGetJmpCodeBuffer(PVOID Buffer,size_t Length);
EXTERN_C _declspec(dllexport) UINT64 SvmGetJmpCodeBufferLength();
EXTERN_C _declspec(dllexport) void SvmGetJmpCodeFuncBuffer(PVOID Buffer,size_t Length);
EXTERN_C _declspec(dllexport) UINT64 SvmGetJmpCodeFuncBufferLength();
EXTERN_C _declspec(dllexport) PVOID SvmAllocateJmpTrampoline(PHOOK_FUNC_INFO HookFuncInfo, SIZE_T Length);
EXTERN_C _declspec(dllexport) void SvmFreeJmpTrampoline(PHOOK_FUNC_INFO HookFuncInfo);
EXTERN_C _declspec(dllexport) PHOOK_INFO SvmAddHookInfo(PCSTR TagStr, UINT64 VirtualAddress, SIZE_T MapSize);
EXTERN_C _declspec(dllexport) PHOOK_INFO SvmFindHookInfoTag(PCSTR TagStr);
EXTERN_C _declspec(dllexport) PHOOK_INFO SvmFindHookInfoGuid(PGUID HookId);
EXTERN_C _declspec(dllexport) BOOLEAN SvmCreateShadowPage(PHOOK_INFO HookInfo, UINT32 ShadowPageIndex);
EXTERN_C _declspec(dllexport) BOOLEAN SvmSetGuestShadowPage(PCPU_CONTEXT CpuContext, PHOOK_INFO HookInfo, UINT32 ShadowPageIndex, BOOLEAN NoExecute, BOOLEAN Write);
EXTERN_C _declspec(dllexport) BOOLEAN SvmShadowCopyMemory(PHOOK_INFO HookInfo, UINT32 ShadowPageIndex, UINT64 VirtualAddress, PVOID Data, SIZE_T DataSize);
EXTERN_C _declspec(dllexport) PHOOK_FUNC_INFO SvmAddHookFuncInfo(PHOOK_INFO hookInfo, UINT64 OriginalFuncAddress, UINT64 HookFuncAddress, SIZE_T FuncLength);
EXTERN_C _declspec(dllexport) PHOOK_FUNC_INFO SvmFindHookFuncInfo(PHOOK_INFO hookInfo, UINT64 RipAddress, BOOLEAN Lock);
EXTERN_C _declspec(dllexport) void SvmFreeGuestShadowPage(PHOOK_INFO HookInfo, UINT32 ShadowPageIndex);
EXTERN_C _declspec(dllexport) void SvmHookReference(PHOOK_INFO HookInfo);
EXTERN_C _declspec(dllexport) void SvmHookDereference(PHOOK_INFO HookInfo);
EXTERN_C _declspec(dllexport) PHOOK_INFO SvmFindHookInfoPageBase(PLIST_ENTRY ListHead, UINT64 VirtualAddress);
EXTERN_C _declspec(dllexport) PHOOK_FUNC_INFO SvmFindHookFuncInfoByJmpTrampoline(PHOOK_INFO hookInfo, UINT64 JmpTrampolineRipAddress, BOOLEAN Lock);
EXTERN_C _declspec(dllexport) BOOLEAN SvmIsHookRefCountZero(PHOOK_INFO HookInfo);
EXTERN_C _declspec(dllexport) PHOOK_INFO SvmEnumNextHookInfo(PHOOK_INFO CurrentHookInfo);
EXTERN_C _declspec(dllexport) void SvmRemoveHookFuncInfo(PHOOK_INFO hookInfo, PHOOK_FUNC_INFO funcInfo, BOOLEAN Lock);
EXTERN_C _declspec(dllexport) BOOLEAN SvmProtectDriverSection(UINT64 VirtualAddress, SIZE_T Size, BOOLEAN NoExecute, PBOOLEAN CoreStatus, UINT32 CoreCount);
EXTERN_C _declspec(dllexport) UINT64 SvmGetReturnOffset();
EXTERN_C _declspec(dllexport) NTSTATUS __stdcall test();
EXTERN_C _declspec(dllexport) NTSTATUS __stdcall test2();
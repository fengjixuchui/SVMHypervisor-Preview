#pragma once
#include <ntifs.h>
#define DEF_PTR(type,baseAddress,offset) (*(type*)(((UCHAR*)(baseAddress))+((__int64)(offset))))
#define PTR_ADD(type,baseAddress,offset) ((type)(((UCHAR*)(baseAddress))+((__int64)(offset))))
EXTERN_C _declspec(dllexport) UINT8 GetInstructionLength(PVOID CodeAddr, UINT8 MaxLength);
#pragma once
#include "PTE.h"
#pragma pack(push,1)
typedef struct vmcb_section
{
    UINT16 Selector;
    UINT16 Attrib;
    UINT32 limit;
    UINT64 base;
}VMCB_SECTION, * PVMCB_SECTION;
#pragma pack(pop)
typedef union _EVENTINJ
{
    UINT64 Uo64;
    struct
    {
        UINT64 Vector : 8;
        UINT64 Type : 3;
        UINT64 ErrorCodeValid : 1;
        UINT64 Reserved1 : 19;
        UINT64 Valid : 1;
        UINT64 ErrorCode : 32;
    }Bits;
}EVENTINJ,*PEVENTINJ;
#pragma pack(push, 1)
typedef struct _DESCRIPTOR_TABLE_REGISTER {
    UINT16 Limit;
    ULONG64 Base;
} DESCRIPTOR_TABLE_REGISTER, * PDESCRIPTOR_TABLE_REGISTER;
typedef union _SEGMENT_ATTRIBUTES
{
    unsigned short Uo16;
    struct
    {
        unsigned short Type : 4;
        unsigned short S : 1;
        unsigned short Dpl : 2;
        unsigned short P : 1;
        unsigned short Avl : 1;
        unsigned short L : 1; 
        unsigned short Db : 1;
        unsigned short G : 1;
        unsigned short Reserved : 4;
    } Fields;
} SEGMENT_ATTRIBUTES,*PSEGMENT_ATTRIBUTES;
typedef struct _SEGMENT_DESCRIPTOR
{
    union
    {
        UINT64 AsUInt64;
        struct
        {
            UINT16 LimitLow;        // [0:15]
            UINT16 BaseLow;         // [16:31]
            UINT32 BaseMiddle : 8;  // [32:39]
            UINT32 Type : 4;        // [40:43]
            UINT32 System : 1;      // [44]
            UINT32 Dpl : 2;         // [45:46]
            UINT32 Present : 1;     // [47]
            UINT32 LimitHigh : 4;   // [48:51]
            UINT32 Avl : 1;         // [52]
            UINT32 LongMode : 1;    // [53]
            UINT32 DefaultBit : 1;  // [54]
            UINT32 Granularity : 1; // [55]
            UINT32 BaseHigh : 8;    // [56:63]
        } Fields;
    };
} SEGMENT_DESCRIPTOR, * PSEGMENT_DESCRIPTOR;
#pragma pack(pop)
#pragma pack(push,1)
typedef struct _VMCB_CONTROL_AREA
{
	UINT16 InterceptCrRead;
	UINT16 InterceptCrWrite;
	UINT16 InterceptDrRead;
	UINT16 InterceptDrWrite;
	UINT32 InterceptException; 
	UINT32 InterceptMisc1; 
	UINT32 InterceptMisc2;
	UINT8 Reserved1[0x03c - 0x014];
    UINT16 PauseFilterThreshold;
    UINT16 PauseFilterCount;
    UINT64 IopmBasePa;
    UINT64 MsrpmBasePa;
    UINT64 TscOffset;
    UINT32 GuestAsid;
    UINT32 TlbControl;
    UINT64 VIntr;
    UINT64 InterruptShadow;
    UINT64 ExitCode;
    UINT64 ExitInfo1;
    UINT64 ExitInfo2;
    UINT64 ExitIntInfo;
    UINT64 NpEnable;
    UINT64 AvicApicBar; 
    UINT64 GuestPaOfGhcb;
    EVENTINJ EventInj;
    UINT64 NCr3;
    UINT64 LbrVirtualizationEnable;
    UINT64 VmcbClean;
    UINT64 NRip;
    UINT8 NumOfBytesFetched;
    UINT8 GuestInstructionBytes[15];
    UINT64 AvicApicBackingPagePointer;
    UINT64 Reserved2;
    UINT64 AvicLogicalTablePointer;
    UINT64 AvicPhysicalTablePointer;
    UINT64 Reserved3;
    UINT64 VmcbSaveStatePointer;
    UINT8 Reserved4[0x400 - 0x110];
}VMCB_CONTROL_AREA,*PVMCB_CONTROL_AREA;
typedef struct _VMCB_STATE_SAVE_AREA
{
    VMCB_SECTION es;
    VMCB_SECTION cs;
    VMCB_SECTION ss;
    VMCB_SECTION ds;
    VMCB_SECTION fs;
    VMCB_SECTION gs;
    VMCB_SECTION gdtr;
    VMCB_SECTION ldtr;
    VMCB_SECTION idtr;
    VMCB_SECTION tr;
    UINT8 Reserved1[0x0cb - 0x0a0];
    UINT8 Cpl;
    UINT32 Reserved2;
    UINT64 Efer;
    UINT8 Reserved3[0x148 - 0x0d8];
    UINT64 Cr4;
    UINT64 Cr3;
    UINT64 Cr0;
    UINT64 Dr7;
    UINT64 Dr6;
    UINT64 Rflags;
    UINT64 Rip;
    UINT8 Reserved4[0x1d8 - 0x180];
    UINT64 Rsp;
    UINT8 Reserved5[0x1f8 - 0x1e0];
    UINT64 Rax;
    UINT64 Star;
    UINT64 LStar;
    UINT64 CStar;
    UINT64 SfMask;
    UINT64 KernelGsBase;
    UINT64 SysenterCs;
    UINT64 SysenterEsp;
    UINT64 SysenterEip;
    UINT64 Cr2;
    UINT8 Reserved6[0x268 - 0x248];
    UINT64 GPat;
    UINT64 DbgCtl;
    UINT64 BrFrom;
    UINT64 BrTo;
    UINT64 LastExcepFrom; 
    UINT64 LastExcepTo; 
} VMCB_STATE_SAVE_AREA, * PVMCB_STATE_SAVE_AREA;
#pragma pack(pop)
typedef struct _VMCB
{
    VMCB_CONTROL_AREA ControlArea;
    VMCB_STATE_SAVE_AREA StateSaveArea;
    UINT8 Reserved1[0x1000 - sizeof(VMCB_CONTROL_AREA) - sizeof(VMCB_STATE_SAVE_AREA)];
} VMCB, * PVMCB;
typedef struct _GUEST_REGS {
    unsigned __int64 rax;
    unsigned __int64 rcx;
    unsigned __int64 rdx;
    unsigned __int64 rbx;
    unsigned __int64 rsp;
    unsigned __int64 rbp;
    unsigned __int64 rsi;
    unsigned __int64 rdi;
    unsigned __int64 r8;
    unsigned __int64 r9;
    unsigned __int64 r10;
    unsigned __int64 r11;
    unsigned __int64 r12;
    unsigned __int64 r13;
    unsigned __int64 r14;
    unsigned __int64 r15;
} GUEST_REGS, * PGUEST_REGS;
extern volatile BOOLEAN g_bSvmRunning;
extern volatile BOOLEAN g_SuspendGuest;
EXTERN_C _declspec(dllexport) volatile ULONG CpuCount;
#define MAX_SVM_THREADS 2048
extern BOOLEAN IsVirtualCpu[MAX_SVM_THREADS];
BOOLEAN AllocVMCB(PCPU_CONTEXT context);
BOOLEAN InitVMCB(PCPU_CONTEXT context);
BOOLEAN StartSVM(PCPU_CONTEXT context);
BOOLEAN CheckIsVirtualCpu(UINT32 Index);
void FreeVMCB(PCPU_CONTEXT context);
void VmExitHandler(PCPU_CONTEXT context, PGUEST_REGS Regs);
__forceinline void SuspendAllGuest();
__forceinline void ResumeAllGuest();
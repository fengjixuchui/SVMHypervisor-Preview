#pragma once
#define MSR_TSC 0x10
#define MSR_APIC_BASE 0x1B
#define MSR_SPEC_CTRL 0x48
#define MSR_PRED_CMD 0x49
#define MSR_MPERF 0xE7
#define MSR_APERF 0xE8
#define	 MSR_MTRR_CAP 0xFE
#define MSR_SYSENTER_CS 0x174
#define MSR_SYSENTER_ESP 0x175
#define MSR_SYSENTER_EIP 0x176

#define MSR_PAT 0x277
#define MSR_EFER 0xC0000080
#define MSR_STAR 0xC0000081
#define MSR_LSTAR 0xC0000082
#define MSR_CSTAR 0xC0000083
#define MSR_SF_MASK 0xC0000084
#define MSR_FSBASE 0xC0000100
#define MSR_GSBASE  0xC0000101
#define MSR_KERNEL_GSBASE 0xC0000102
#define MSR_VM_HSAVE_PA 0xC0010117
#define CR0_PE (1ULL << 0)   // 0: Protection Enable 
#define CR0_MP (1ULL << 1)   // 1: Monitor Coprocessor
#define CR0_EM (1ULL << 2)   // 2: Emulation
#define CR0_TS (1ULL << 3)   // 3: Task Switched
#define CR0_ET (1ULL << 4)   // 4: Extension Type
#define CR0_NE (1ULL << 5)   // 5: Numeric Error
#define CR0_WP (1ULL << 16)  // 16: Write Protect
#define CR0_AM (1ULL << 18)  // 18: Alignment Mask
#define CR0_NW (1ULL << 29)  // 29: Not Write-through
#define CR0_CD (1ULL << 30)  // 30: Cache Disable
#define CR0_PG (1ULL << 31)  // 31: Paging 
#define CR4_PAE (1ULL<<5)
#define CR4_SMEP (1ULL<<20)
#define CR4_SMAP (1ULL<<21)
#define EFER_SVME (1ULL << 12)
#define VMEXIT_CPUID 0x72
#define VMEXIT_MSR 0x7C
#define VMEXIT_VMMCALL 0x81
#define VMEXIT_EXCP_BP 0x43
#define VMEXIT_EXCP_GP 0x4D
#define VMEXIT_EXCP_PF 0x4E
#define VMEXIT_INVALID -1
#define VMEXIT_SHUTDOWN 0x7F
#define VMEXIT_IOIO 0x7B
#define INTERCEPT_EXCP_BP (1UL << 3)
#define INTERCEPT_EXCP_GP (1UL<<13)
#define INTERCEPT_EXCP_PF (1UL<<14)
#define INTERCEPT_MISC1_CPUID (1 << 18)
#define INTERCEPT_MISC1_INTR  (1 << 0)
#define INTERCEPT_MISC1_SMI (1 << 2)
#define INTERCEPT_MISC1_IOIO (1 << 27)
#define INTERCEPT_MISC1_MSR (1 << 28)
#define INTERCEPT_MISC1_SHUTDOWN (1UL << 31)
#define INTERCEPT_MISC2_VMMCALL (1UL << 1)
#define INTERCEPT_MISC2_VMRUN (1UL << 0)
#define IOPM_ENABLE_PORT(iopm, port) (((PUCHAR)(iopm))[(port) / 8] |= (1 << ((port) % 8)))
#define IOPM_DISABLE_PORT(iopm, port) (((PUCHAR)(iopm))[(port) / 8] &= ~(1 << ((port) % 8)))
#define VMMCALL_SUSPEND 0x57DE
#define VMMCALL_SUSTEND_CONTROL 0x57FE
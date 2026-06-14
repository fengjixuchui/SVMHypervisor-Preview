#include "CPUID.h"
#pragma code_seg(".entry$002")
BOOLEAN CheckSvmSupport()
{
	int cpuInfo[4] = { 0 };
	__cpuid(cpuInfo, 0);
	if (cpuInfo[0] < 0)
		return FALSE;
	DbgPrintEx(77, 0, "id1=0x%x\nid2=0x%x\nid3=0x%x\n", cpuInfo[1], cpuInfo[2], cpuInfo[3]);
	if (cpuInfo[1] != 0x68747541 || cpuInfo[2] != 0x444d4163 || cpuInfo[3] != 0x69746e65)
	{
		DbgPrintEx(77, 0, "不是AMD处理器\n");
		return FALSE;
	}
	RtlZeroMemory(cpuInfo, sizeof(cpuInfo));
	__cpuid(cpuInfo, 0x80000001);
	if ((cpuInfo[2] & (1 << 2)) == 0) {
		DbgPrintEx(77, 0, "CPU 不支持 SVM 或 BIOS 中未开启\n");
		return FALSE;
	}
	ULONG64 vmCr = __readmsr(0xC0010114);
	if ((vmCr & (1 << 4)) != 0) {
		DbgPrintEx(77,0,"SVM 被 BIOS 禁用且锁定\n");
		return FALSE;
	}
	RtlZeroMemory(cpuInfo, sizeof(cpuInfo));
	__cpuid(cpuInfo, 0x8000000a);
	if ((cpuInfo[3] & (1 << 0)) == 0) {
		DbgPrintEx(77, 0, "CPU 不支持 NPT 嵌套页表\n");
		return FALSE;
	}
	DbgPrintEx(77, 0, "设备符合要求\n");
	return TRUE;
}
#pragma code_seg()
#pragma once
#include <ntifs.h>
#include <intrin.h>
EXTERN_C __declspec(dllexport) BOOLEAN g_VmStart;
BOOLEAN CheckSvmSupport();
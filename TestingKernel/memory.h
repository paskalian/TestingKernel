#pragma once

#include <ntifs.h>

NTSTATUS NTAPI MmCopyVirtualMemory
(
	PEPROCESS SourceProcess,
	PVOID SourceAddress,
	PEPROCESS TargetProcess,
	PVOID TargetAddress,
	SIZE_T BufferSize,
	KPROCESSOR_MODE PreviousMode,
	PSIZE_T ReturnSize
);

typedef struct _KERNEL_RW_BUFFER
{
	PVOID SourceAddress;
	PVOID TargetAddress;
	SIZE_T BufferSize;
} KERNEL_RW_BUFFER, * PKERNEL_RW_BUFFER;

NTSTATUS NTAPI KernelReadMem(PVOID SourceAddress,PVOID TargetAddress, SIZE_T BufferSize);
NTSTATUS NTAPI KernelWriteMem(PVOID SourceAddress, PVOID TargetAddress, SIZE_T BufferSize);



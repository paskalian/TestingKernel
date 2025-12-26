#include "memory.h"

extern HANDLE g_AcClientPID;
// MmCopyVirtualMemory uses from-to logic, so when reading we will read INTO our KERNEL DRIVER thus using PsGetCurrentProcess() as target process.
// and when writing we will write INTO the TARGET PROCESS thus getting ac_client.exe's PEPROCESS as target process.
NTSTATUS NTAPI KernelReadMem(PVOID SourceAddress, PVOID TargetAddress, SIZE_T BufferSize)
{
	SIZE_T ReturnBytes = 0;
	PEPROCESS SrcProc;
	PsLookupProcessByProcessId(g_AcClientPID, &SrcProc);

	return MmCopyVirtualMemory(SrcProc, SourceAddress, PsGetCurrentProcess(), TargetAddress, BufferSize, KernelMode, &ReturnBytes);
}

NTSTATUS NTAPI KernelWriteMem(PVOID SourceAddress, PVOID TargetAddress, SIZE_T BufferSize)
{
	SIZE_T ReturnBytes = 0;
	PEPROCESS SrcProc;
	PsLookupProcessByProcessId(g_AcClientPID, &SrcProc);

	return MmCopyVirtualMemory(PsGetCurrentProcess(), SourceAddress, SrcProc, TargetAddress, BufferSize, KernelMode, &ReturnBytes);
}
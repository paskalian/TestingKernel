#include "irp.h"
#include "memory.h"

extern ULONGLONG g_AcClientBase;
extern PDEVICE_OBJECT g_pDevice;
extern UNICODE_STRING g_DeviceName;
extern UNICODE_STRING g_DosDeviceName;

NTSTATUS IrpCreateClose(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	UNREFERENCED_PARAMETER(DeviceObject);

	DbgPrintEx(0, 0, "Irp create close called.");

	NTSTATUS Status = STATUS_SUCCESS;

	Irp->IoStatus.Information = 0;
	Irp->IoStatus.Status = Status;

	IoCompleteRequest(Irp, IO_NO_INCREMENT);

	return Status;
}

NTSTATUS IrpControl(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	UNREFERENCED_PARAMETER(DeviceObject);

	DbgPrintEx(0, 0, "Irp control called.");

	NTSTATUS Status = STATUS_UNSUCCESSFUL;
	Irp->IoStatus.Information = 0;
	Irp->IoStatus.Status = Status;

	PIO_STACK_LOCATION StackLocation = IoGetCurrentIrpStackLocation(Irp);
	switch (StackLocation->Parameters.DeviceIoControl.IoControlCode)
	{
	case KRNL_GET_BASEADDR:
	{
		Status = STATUS_SUCCESS;

		Irp->IoStatus.Information = sizeof(ULONGLONG);
		Irp->IoStatus.Status = Status;

		PULONGLONG pBuffer = Irp->AssociatedIrp.SystemBuffer;
		*pBuffer = g_AcClientBase;
		break;
	}
	case KRNL_READ_MEM:
	{
		Status = STATUS_SUCCESS;

		Irp->IoStatus.Information = 0;
		Irp->IoStatus.Status = Status;

		DbgPrintEx(0, 0, "Kernel read called.");

		PKERNEL_RW_BUFFER pRwBuffer = Irp->AssociatedIrp.SystemBuffer;

		DbgPrintEx(0, 0, "SRC: %p\n", pRwBuffer->SourceAddress);
		DbgPrintEx(0, 0, "TRGT: %p\n", pRwBuffer->TargetAddress);
		DbgPrintEx(0, 0, "SIZE: %p\n", pRwBuffer->BufferSize);

		KernelReadMem(pRwBuffer->SourceAddress, pRwBuffer->TargetAddress, pRwBuffer->BufferSize);
		break;
	}
	case KRNL_WRITE_MEM:
	{
		Status = STATUS_SUCCESS;

		Irp->IoStatus.Information = 0;
		Irp->IoStatus.Status = Status;

		DbgPrintEx(0, 0, "Kernel write called.");

		PKERNEL_RW_BUFFER pRwBuffer = Irp->AssociatedIrp.SystemBuffer;

		DbgPrintEx(0, 0, "SRC: %p\n", pRwBuffer->SourceAddress);
		DbgPrintEx(0, 0, "TRGT: %p\n", pRwBuffer->TargetAddress);
		DbgPrintEx(0, 0, "SIZE: %p\n", pRwBuffer->BufferSize);

		KernelWriteMem(pRwBuffer->SourceAddress, pRwBuffer->TargetAddress, pRwBuffer->BufferSize);
		break;
	}
	}

	IoCompleteRequest(Irp, IO_NO_INCREMENT);

	return Status;
}
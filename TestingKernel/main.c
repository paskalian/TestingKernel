#include <ntifs.h>

#define GET_BASEADDR CTL_CODE( FILE_DEVICE_UNKNOWN, 0x801, METHOD_BUFFERED, FILE_ANY_ACCESS  )

ULONGLONG g_AcClientBase = 0;
PDEVICE_OBJECT g_pDevice;
UNICODE_STRING g_DeviceName;
UNICODE_STRING g_DosDeviceName;

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
	case GET_BASEADDR:
		Status = STATUS_SUCCESS;

		Irp->IoStatus.Information = sizeof(ULONGLONG);
		Irp->IoStatus.Status = Status;

		PULONGLONG pBuffer = Irp->AssociatedIrp.SystemBuffer;
		*pBuffer = g_AcClientBase;
		break;
	}

	IoCompleteRequest(Irp, IO_NO_INCREMENT);

	return Status;
}

VOID ImageNotifyRoutine(PUNICODE_STRING FullImageName, HANDLE ProcessId, PIMAGE_INFO ImageInfo)
{
	const wchar_t* ImageName = FullImageName->Buffer;

	if (wcsstr(ImageName, L"\\bin_win32\\ac_client.exe"))
	{
		DbgPrintEx(0, 0, "ac_client.exe [%p] addr: %p\n", ProcessId, ImageInfo->ImageBase);

		g_AcClientBase = (ULONGLONG)ImageInfo->ImageBase;
	}
}

NTSTATUS UnloadDriver(PDRIVER_OBJECT DriverObject)
{
	UNREFERENCED_PARAMETER(DriverObject);

	DbgPrintEx(0, 0, "Goodbye world from kernel.");
	PsRemoveLoadImageNotifyRoutine(ImageNotifyRoutine);
	IoDeleteSymbolicLink(&g_DosDeviceName);
	IoDeleteDevice(g_pDevice);

	return STATUS_SUCCESS;
}

NTSTATUS DriverEntry(
	_In_ PDRIVER_OBJECT  DriverObject,
	_In_ PUNICODE_STRING RegistryPath
)
{
	UNREFERENCED_PARAMETER(DriverObject);
	UNREFERENCED_PARAMETER(RegistryPath);


	DriverObject->DriverUnload = UnloadDriver;
	PsSetLoadImageNotifyRoutine(ImageNotifyRoutine);
	
	DriverObject->MajorFunction[IRP_MJ_CREATE] = IrpCreateClose;
	DriverObject->MajorFunction[IRP_MJ_CLOSE] = IrpCreateClose;
	DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = IrpControl;

	RtlInitUnicodeString(&g_DeviceName, L"\\Device\\KernelTestDevice");
	RtlInitUnicodeString(&g_DosDeviceName, L"\\DosDevices\\KernelTestDosDevice");
	IoCreateSymbolicLink(&g_DosDeviceName, &g_DeviceName);
	
	IoCreateDevice(DriverObject, 0, &g_DeviceName, FILE_DEVICE_UNKNOWN, FILE_DEVICE_SECURE_OPEN, FALSE, &g_pDevice);

	DbgPrintEx(0, 0, "Hello world from kernel.");

	return STATUS_SUCCESS;
}
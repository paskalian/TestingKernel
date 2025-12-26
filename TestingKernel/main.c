#include <ntifs.h>

#include "irp.h"

ULONGLONG g_AcClientBase = 0;
HANDLE g_AcClientPID = 0;
PDEVICE_OBJECT g_pDevice;
UNICODE_STRING g_DeviceName;
UNICODE_STRING g_DosDeviceName;


VOID ImageNotifyRoutine(PUNICODE_STRING FullImageName, HANDLE ProcessId, PIMAGE_INFO ImageInfo)
{
	const wchar_t* ImageName = FullImageName->Buffer;

	if (wcsstr(ImageName, L"\\bin_win32\\ac_client.exe"))
	{
		DbgPrintEx(0, 0, "ac_client.exe [%p] addr: %p\n", ProcessId, ImageInfo->ImageBase);

		g_AcClientBase = (ULONGLONG)ImageInfo->ImageBase;
		g_AcClientPID = ProcessId;
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
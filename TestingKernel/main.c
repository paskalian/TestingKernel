#include <ntifs.h>

NTSTATUS UnloadDriver(PDRIVER_OBJECT DriverObject)
{
	UNREFERENCED_PARAMETER(DriverObject);

	DbgPrintEx(0, 0, "Goodbye world from kernel.");

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

	DbgPrintEx(0, 0, "Hello world from kernel.");

	return STATUS_SUCCESS;
}
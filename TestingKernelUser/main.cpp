#include <Windows.h>
#include <iostream>

#define KRNL_GET_BASEADDR CTL_CODE( FILE_DEVICE_UNKNOWN, 0x801, METHOD_BUFFERED, FILE_ANY_ACCESS  )
#define KRNL_READ_MEM CTL_CODE( FILE_DEVICE_UNKNOWN, 0x802, METHOD_BUFFERED, FILE_ANY_ACCESS  )
#define KRNL_WRITE_MEM CTL_CODE( FILE_DEVICE_UNKNOWN, 0x803, METHOD_BUFFERED, FILE_ANY_ACCESS  )

// Dynamic address, update manually.
#define HEALTH_VIRTUAL_ADDRESS 0x0065CB40

typedef struct _KERNEL_RW_BUFFER
{
    PVOID SourceAddress;
    PVOID TargetAddress;
    SIZE_T BufferSize;
} KERNEL_RW_BUFFER, * PKERNEL_RW_BUFFER;

int main()
{
    HANDLE hDriver = CreateFileW(L"\\\\.\\KernelTestDosDevice", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

    ULONG Buffer = 0;
    DWORD BytesReturned = 0;
    DeviceIoControl(hDriver, KRNL_GET_BASEADDR, NULL, NULL, &Buffer, sizeof(ULONG), nullptr, nullptr);

    printf("ac_client.exe base addr: 0x%X\n", Buffer);

    system("pause");

    ULONG ReadHealth = 0;
    KERNEL_RW_BUFFER ReadBuffer = {};
    ReadBuffer.SourceAddress = (PVOID)HEALTH_VIRTUAL_ADDRESS;
    ReadBuffer.TargetAddress = &ReadHealth;
    ReadBuffer.BufferSize = sizeof(ReadHealth);
    DeviceIoControl(hDriver, KRNL_READ_MEM, &ReadBuffer, sizeof(KERNEL_RW_BUFFER), NULL, NULL, nullptr, nullptr);

    printf("Health value: %i\n", ReadHealth);

    system("pause");

    ULONG WriteHealth = 1000;
    KERNEL_RW_BUFFER WriteBuffer = {};
    WriteBuffer.SourceAddress = &WriteHealth;
    WriteBuffer.TargetAddress = (PVOID)HEALTH_VIRTUAL_ADDRESS;
    WriteBuffer.BufferSize = sizeof(WriteHealth);
    DeviceIoControl(hDriver, KRNL_WRITE_MEM, &WriteBuffer, sizeof(KERNEL_RW_BUFFER), NULL, NULL, nullptr, nullptr);

    printf("1000 written into health.\n");

    system("pause");

    CloseHandle(hDriver);
}
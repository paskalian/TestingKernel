#include <Windows.h>
#include <iostream>

#define GET_BASEADDR CTL_CODE( FILE_DEVICE_UNKNOWN, 0x801, METHOD_BUFFERED, FILE_ANY_ACCESS  )

int main()
{
    HANDLE hDriver = CreateFileW(L"\\\\.\\KernelTestDosDevice", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

    ULONG Buffer = 0;
    DWORD BytesReturned = 0;
    DeviceIoControl(hDriver, GET_BASEADDR, NULL, NULL, &Buffer, sizeof(ULONG), nullptr, nullptr);

    printf("ac_client.exe base addr: 0x%X\n", Buffer);

    system("pause");

    CloseHandle(hDriver);
}
#include <windows.h>
#include <iostream>

#include "EasyHook\easyhook.h"

int wmain(int argc, wchar_t **argv);
int wmain(int argc, wchar_t **argv)
{
    UNREFERENCED_PARAMETER(argc);
    UNREFERENCED_PARAMETER(argv);

    
    // Create the process, and inject the fault library...
    std::wcout << L"Creating process, and injecting fault library... ";
    ULONG               pid                 = 0;
    NTSTATUS            cai_status          = 0;
    #ifdef _M_X64
        cai_status = RhCreateAndInject(argv[1], NULL, CREATE_SUSPENDED, EASYHOOK_INJECT_DEFAULT, NULL, L"faultron64.dll", NULL, 0, &pid);
    #else
        cai_status = RhCreateAndInject(argv[1], NULL, CREATE_SUSPENDED, EASYHOOK_INJECT_DEFAULT, L"faultron32.dll", NULL, NULL, 0, &pid);
    #endif
    if (NT_SUCCESS(cai_status)) {
        std::wcout << L"Done (PID:0x" << std::hex << std::uppercase << pid << ")." << std::endl;
    } else {
        std::wcout << L"Failed (NTSTATUS:0x" << std::hex << std::uppercase << cai_status << ")." << std::endl;
    }
    

    return 0;
}
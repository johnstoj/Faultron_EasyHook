#include <windows.h>

#include <vector>
#include <memory>

#include "EasyHook\easyhook.h"

HMODULE                                                 g_hModule           = NULL;

std::vector<TRACED_HOOK_HANDLE>                         g_vApiHookHandles;

void RemoveAllApiHooks(void);
void RemoveAllApiHooks(void)
{
    LhUninstallAllHooks();
    LhWaitForPendingRemovals();

    for (std::vector<TRACED_HOOK_HANDLE>::const_iterator it = g_vApiHookHandles.begin(); it != g_vApiHookHandles.end(); it++) {
        delete *it;
    }
}


PVOID NTAPI MyRtlAllocateHeap(PVOID HeapHandle, ULONG Flags, SIZE_T Size);
PVOID NTAPI MyRtlAllocateHeap(PVOID HeapHandle, ULONG Flags, SIZE_T Size)
{
    ::MessageBoxW(NULL, L"MyRtlAllocateHeap", L"MyRtlAllocateHeap", MB_OK);
    return ::RtlAllocateHeap(HeapHandle, Flags, Size);
}


extern "C" __declspec(dllexport) void __stdcall NativeInjectionEntryPoint(REMOTE_ENTRY_INFO*);
extern "C" __declspec(dllexport) void __stdcall NativeInjectionEntryPoint(REMOTE_ENTRY_INFO*)
{
    try {
        TRACED_HOOK_HANDLE globalallochook(new HOOK_TRACE_INFO());
        LhInstallHook(GetProcAddress(GetModuleHandleW(L"ntdll"), "RtlAllocateHeap"), MyRtlAllocateHeap, NULL, globalallochook);
    
        ULONG ulTidList[1] = {};
        LhSetExclusiveACL(ulTidList, 0, globalallochook);
        
        g_vApiHookHandles.push_back(globalallochook);
    
        // Wakeup the suspended process...
        RhWakeUpProcess();
    } catch (...) {
        ::OutputDebugStringW(L"Faultron: NativeInjectionEntryPoint() exception.");
        RemoveAllApiHooks();
    }
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD fdwReason, LPVOID);
BOOL WINAPI DllMain(HMODULE hModule, DWORD fdwReason, LPVOID)
{

    switch (fdwReason) {
        case DLL_PROCESS_ATTACH:
            g_hModule = hModule;

            ::DisableThreadLibraryCalls(hModule);
            
            

            ::OutputDebugStringW(L"Faultron: DLL_PROCESS_ATTACH.");

            
            break;
        
        case DLL_PROCESS_DETACH:
            ::OutputDebugStringW(L"Faultron: DLL_PROCESS_DETACH.");
            
            RemoveAllApiHooks();

            break;
    }

    return TRUE;
}

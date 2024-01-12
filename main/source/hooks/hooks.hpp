#ifndef _HOOKS_HPP
#define _HOOKS_HPP

#include <common.hpp>
#include "../../utility/rage/rage.hpp"

class VTSwap {
public:
    static VTSwap* Instance();
public:
    void Create(void* address, int index, void* detour, void** original, const char* name);
    void Destroy();
public:
    std::vector<std::tuple<void*, int, void**>> swapped;
};

class Hooks {
private:
    void Detour(uintptr_t address, void* detour, void** original, const char* name);
public:
    static Hooks* Instance();
    void Initialize();
    void Uninitialize();
public:
    void* originalConvertThreadToFiber;
    static LPVOID ConvertThreadToFiberDetour(
        LPVOID param
    );

    void* originalGetPlayerPed;
    static __int64 GetPlayerPed(
        __int64 a1,
        __int64 a2
    );

    WNDPROC originalWndProc;
    void* originalPresent;
    static HRESULT __stdcall Present(
        IDXGISwapChain3* swapChain,
        UINT syncInterval, 
        UINT flags
    );
};

#endif
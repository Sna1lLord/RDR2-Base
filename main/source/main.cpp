#include "main.hpp"

#include "../utility/logger/logger.hpp"
#include "../utility/memory/signatures/signatures.hpp"
#include "../utility/invoker/natives.hpp"
#include "../utility/invoker/queue.hpp"
#include "hooks/hooks.hpp"
#include "ui/ui.hpp"
#include "script/script.hpp"
#include "script/manager.hpp"
#include "features/features.hpp"

static HANDLE coreThread;
static DWORD lpThreadId = 0;

void FeaturesScript() {
    while (true) {
        Queue::Instance()->Tick();
        Features::Instance()->Tick();

        Script::Current()->ScriptYield();
    }
}

DWORD WINAPI Core(LPVOID lpParam) {
    Logger::Instance()->Initialize();
    Logger::Instance()->Log(xorstr_("Welcome To The Base! \n"));

    Signatures::Instance()->Initialize();
    Hooks::Instance()->Initialize();

    ScriptManager::Instance()->Add(std::make_unique<Script>(&FeaturesScript), xorstr_("Features"));

    while (g_Main.running) {
        if (GetAsyncKeyState(VK_END) & 1)
            g_Main.running = false;

        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }

    Logger::Instance()->Log(xorstr_("Goodbye! \n"));

    Hooks::Instance()->Uninitialize();
    Ui::Instance()->Uninitialize();
    Features::Instance()->Uninitialize();
    Queue::Instance()->Uninitialize();
    ScriptManager::Instance()->Uninitialize();
    Invoker::Instance()->Uninitialize();
    Signatures::Instance()->Uninitialize();
    Logger::Instance()->Uninitialize();

    CloseHandle(coreThread);
    FreeLibraryAndExitThread((HMODULE)lpParam, 0);
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    UNREFERENCED_PARAMETER(lpvReserved);

    if (fdwReason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hinstDLL);

        coreThread = CreateThread(
            nullptr, 
            0, 
            Core, 
            hinstDLL, 
            0, 
            &lpThreadId
        );
    }

    return TRUE;
}
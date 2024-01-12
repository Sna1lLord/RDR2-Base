#include "../hooks.hpp"
#include "../../../utility/logger/logger.hpp"
#include "../../../utility/invoker/natives.hpp"
#include "../../../utility/invoker/queue.hpp"
#include "../../../utility/memory/signatures/signatures.hpp"
#include "../../ui/ui.hpp"
#include "../../script/manager.hpp"

__int64 Hooks::GetPlayerPed(__int64 a1,__int64 a2) {
    ScriptManager::Instance()->Tick();

    return static_cast<decltype(&GetPlayerPed)>(Hooks::Instance()->originalGetPlayerPed)(a1, a2);
}
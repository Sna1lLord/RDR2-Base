#include "script.hpp"

Script::Script(std::function<void()> scriptAction) {
    action = scriptAction;
    mainFiber = nullptr;

    scriptFiber = CreateFiber(0, [](void* param) {
        Script* thisScript = (Script*)(param);
        thisScript->FiberAction();
    }, this);
}

Script* Script::Current() {
    return (Script*)(GetFiberData());
}

void Script::ScriptYield(std::optional<std::chrono::high_resolution_clock::duration> time) {
    if (time.has_value())
        wake = std::chrono::high_resolution_clock::now() + time.value();
    else
        wake = std::nullopt;

    SwitchToFiber(mainFiber);
}

void Script::FiberAction() {
    std::invoke(action);

    while (true) {
        ScriptYield();
    }
}

void Script::Tick() {
    mainFiber = GetCurrentFiber();
    if (!wake.has_value() || wake.value() <= std::chrono::high_resolution_clock::now())
        SwitchToFiber(scriptFiber);
}
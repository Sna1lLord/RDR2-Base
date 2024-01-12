#include "manager.hpp"
#include "../../utility/logger/logger.hpp"
#include "../ui/ui.hpp"

static ScriptManager* scriptManagerInstance = nullptr;

ScriptManager* ScriptManager::Instance() {
	if (!scriptManagerInstance)
		scriptManagerInstance = new ScriptManager();

	return scriptManagerInstance;
}

void ScriptManager::Uninitialize() {
    scriptManagerInstance = nullptr;
    delete this;
}

void ScriptManager::Add(std::unique_ptr<Script> script, const char* scriptName) {
    Logger::Instance()->Log(xorstr_("Created %s Script \n"), scriptName);

    std::lock_guard lock(mutex);
	scripts.push_back(std::move(script));
}

void ScriptManager::Clear() {
    std::lock_guard lock(mutex);
	scripts.clear();
}

void ScriptManager::Tick() {
    bool ensureMainFiber = (ConvertThreadToFiber(nullptr), true);
    if (!ensureMainFiber)
        return;

    std::lock_guard lock(mutex);
    for (auto const &script : scripts)
        script->Tick();
}
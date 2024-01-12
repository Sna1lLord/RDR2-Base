#ifndef _SCRIPT_HPP
#define _SCRIPT_HPP

#include <common.hpp>

class Script {
private:
    void* scriptFiber;
    void* mainFiber;
    std::function<void()> action;
    std::optional<std::chrono::high_resolution_clock::time_point> wake;
public:
    Script(std::function<void()> scriptAction);
public:
    static Script* Current();
    void ScriptYield(std::optional<std::chrono::high_resolution_clock::duration> time = std::nullopt);
    void FiberAction();
    void Tick();
};

#endif
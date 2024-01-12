#include "../hooks.hpp"

LPVOID Hooks::ConvertThreadToFiberDetour(LPVOID param) {
    if (IsThreadAFiber())
		  return GetCurrentFiber();

    return static_cast<decltype(&ConvertThreadToFiberDetour)>(Hooks::Instance()->originalConvertThreadToFiber)(param);
}
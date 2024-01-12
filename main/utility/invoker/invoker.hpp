#ifndef _INVOKER_HPP
#define _INVOKER_HPP

#include <common.hpp>
#include "../rage/rage.hpp"

class NativeCallContext : public scrNativeCallContext {
public:
	NativeCallContext();
private:
	std::uint64_t returnStack[10];
	std::uint64_t argumentStack[100];
};

typedef void(__cdecl* Handler)(NativeCallContext* context);

// If you want to dump the entrypoints:
// https://github.com/Sna1lLord/RDR2-Entrypoint-Dumper/

class Invoker {
public:
    static Invoker* Instance();
    void Uninitialize();
public:
    Handler GetHandler(scrNativeHash hash);

    void Begin();
    void End(scrNativeHash hash);

    template <typename T>
    void Push(T&& value) {
        callContext.Push(std::forward<T>(value));
    }

    template <typename T>
    T& Get() {
        return *callContext.GetReturn<T>();
    }
private:
    bool handlersCached = false;
    NativeCallContext callContext;
    std::unordered_map<scrNativeHash, scrNativeHandler> handlerCache;
};

#endif
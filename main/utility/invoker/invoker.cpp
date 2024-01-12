#include "invoker.hpp"
#include "../memory/signatures/signatures.hpp"

extern "C" void	_call_asm(void* context, void* function, void* ret);
static Invoker* invokerInstance = nullptr;

Invoker* Invoker::Instance() {
    if (!invokerInstance) {
        invokerInstance = new Invoker();
    }

    return invokerInstance;
}

void Invoker::Uninitialize() {
    invokerInstance = nullptr;
    delete this;
}

NativeCallContext::NativeCallContext() {
	returnValue = &returnStack[0];
	arguments = &argumentStack[0];
}

Handler Invoker::GetHandler(scrNativeHash hash) {
	return (Handler)(Signatures::Instance()->definitions.getNativeAddressFromHash(hash));
}

void Invoker::Begin() {
    callContext.Reset();
}

void Invoker::End(scrNativeHash hash) {
    Handler handler = GetHandler(hash);
    handler(&callContext);
}
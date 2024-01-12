#ifndef _RAGE_HPP
#define _RAGE_HPP

#include <common.hpp>

class scrNativeCallContext;
class scrNativeRegistration;

using scrNativeHash = uint64_t;
using scrNativeMapping = std::pair<scrNativeHash, scrNativeHash>;
using scrNativeHandler = void(*)(scrNativeCallContext*);

class scrNativeCallContext {
public:
    void Reset() {
        argumentCount = 0;
        dataCount = 0;
    }

    template <typename T>
    void Push(T&& value) {
        static_assert(sizeof(T) <= sizeof(uint64_t));
        *reinterpret_cast<std::remove_cv_t<std::remove_reference_t<T>>*>(reinterpret_cast<uint64_t*>(arguments) + argumentCount++) = std::forward<T>(value);
    }

    template <typename T>
    T& Get(size_t index) {
        static_assert(sizeof(T) <= sizeof(uint64_t));
        return *reinterpret_cast<T*>(reinterpret_cast<uint64_t*>(arguments) + index);
    }

    template <typename T>
    void Set(size_t index, T&& value) {
        static_assert(sizeof(T) <= sizeof(uint64_t));
        *reinterpret_cast<std::remove_cv_t<remove_reference_t<T>>*>(reinterpret_cast<uint64_t*>(arguments) + index) = std::forward<T>(value);
    }

    template <typename T>
    T* GetReturn() {
        return reinterpret_cast<T*>(returnValue);
    }

    template <typename T>
    void SetReturn(T&& value) {
        *reinterpret_cast<std::remove_cv_t<std::remove_reference_t<T>>*>(returnValue) = std::forward<T>(value);
    }

    template <typename T>
    void SetReturn(T& value) {
        *reinterpret_cast<std::remove_cv_t<std::remove_reference_t<T>>*>(returnValue) = std::forward<T>(value);
    }

protected:
    void* returnValue;
    uint32_t argumentCount;
    void* arguments;
    int32_t dataCount;
    uint32_t data[48];
};

class scrNativeRegistrationTable {
    scrNativeRegistration* entries[0xFF];
    uint32_t unk;
    bool initialized;
};

#endif
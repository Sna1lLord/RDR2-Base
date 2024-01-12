#ifndef _FUNCTIONS_HPP
#define _FUNCTIONS_HPP

#include <common.hpp>
#include "../../definitions.hpp"

class datBitBuffer;

class Functions {
public:
    typedef uintptr_t(__fastcall* GetNativeAddressFromHashFn)(uintptr_t a1);
};

#endif
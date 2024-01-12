#ifndef _FEATURES_HPP
#define _FEATURES_HPP

#include <common.hpp>
#include "../../utility/definitions.hpp"
#include "../../utility/config.hpp"

class Features {
public:
    static Features* Instance();
    void Initialize();
    void Uninitialize();
    void Tick();
public:
    int localPlayer;
    int localPed;
    int localHorse;
public:
    NativeVector RotationToDirection(NativeVector rotation);
    NativeVector LoadGround(NativeVector location);
};

#endif
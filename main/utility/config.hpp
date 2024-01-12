#ifndef _CONFIG_HPP
#define _CONFIG_HPP

#include <common.hpp>

class Config;

static Config* configInstance = nullptr;

struct self_t {
    bool godmode = false;
    bool demiGodmode = false;

    bool noClip = false;
    float noClipSpeed = 1.f;
};

class Config {
public:
    static Config* Instance() {
        if (!configInstance)
            configInstance = new Config();

        return configInstance;
    }
public:
    self_t self;
};

#endif
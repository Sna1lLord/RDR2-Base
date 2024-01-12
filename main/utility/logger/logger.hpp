#ifndef _LOGGER_HPP
#define _LOGGER_HPP

#include <common.hpp>

class Logger {
public:
    static Logger* Instance();
    void Initialize();
    void Uninitialize();

    template <typename... T>
    void Log(T... args) {
        printf(args...);
    }
};

#endif
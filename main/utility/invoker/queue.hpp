#ifndef _QUEUE_HPP
#define _QUEUE_HPP

#include <common.hpp>

class Queue;

static Queue* queueInstance = nullptr;

class Queue {
public:
    static Queue* Instance() {
        if (!queueInstance)
            queueInstance = new Queue();

        return queueInstance;
    }
    void Uninitialize() {
        queueInstance = nullptr;
        delete this;
    }
public:
    void Add(std::function<void()> function) {
        std::lock_guard lock(mutex);
        jobs.push(std::move(function));
    }

    void Tick() {
        std::unique_lock lock(mutex);
        if (!jobs.empty()) {
            std::function<void()> job = std::move(jobs.top());
            jobs.pop();
            lock.unlock();

            std::invoke(std::move(job));
        }
    }
public:
    std::stack<std::function<void()>> jobs;
    std::recursive_mutex mutex;
};

#endif
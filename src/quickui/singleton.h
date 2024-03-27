#pragma once

#include <mutex>

template<typename T>
class Singleton
{
public:
    static T *getInstance();

    Singleton(const Singleton &other)               = delete;
    Singleton<T> &operator=(const Singleton &other) = delete;

private:
    static std::mutex mutex_;
    static T         *instance_;
};

template<typename T>
std::mutex Singleton<T>::mutex_;
template<typename T>
T *Singleton<T>::instance_;

template<typename T>
T *Singleton<T>::getInstance()
{
    if (instance_ == nullptr)
    {
        std::lock_guard<std::mutex> locker(mutex_);
        if (instance_ == nullptr)
        {
            instance_ = new T();
        }
    }
    return instance_;
}

#define SINGLETONG(Class)                       \
private:                                        \
    friend class Singleton<Class>;              \
    friend struct QScopedPointerDeleter<Class>; \
                                                \
public:                                         \
    static Class *getInstance()                 \
    {                                           \
        return Singleton<Class>::getInstance(); \
    }

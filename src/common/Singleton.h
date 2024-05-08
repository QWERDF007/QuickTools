#pragma once

namespace quicktools::common {

/**
 * @brief The Singleton class
 */
template<typename T>
class Singleton
{
public:
    static T *getInstance();
};

template<typename T>
T *Singleton<T>::getInstance()
{
    static T *instance = new T();
    return instance;
}

} // namespace quicktools::common

#define SINGLETON(Class)                                            \
private:                                                            \
    friend class quicktools::common::Singleton<Class>;              \
                                                                    \
public:                                                             \
    static Class *getInstance()                                     \
    {                                                               \
        return quicktools::common::Singleton<Class>::getInstance(); \
    }

#define QT_QML_SINGLETON(Class)                                      \
private:                                                             \
    friend class quicktools::common::Singleton<Class>;               \
                                                                     \
public:                                                              \
    static Class *getInstance()                                      \
    {                                                                \
        return quicktools::common::Singleton<Class>::getInstance();  \
    }                                                                \
                                                                     \
    static Class *create(QQmlEngine *qmlEngine, QJSEngine *jsEngine) \
    {                                                                \
        Q_UNUSED(qmlEngine)                                          \
        Q_UNUSED(jsEngine)                                           \
        return getInstance();                                        \
    }

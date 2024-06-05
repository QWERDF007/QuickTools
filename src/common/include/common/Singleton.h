#pragma once

namespace quicktools::common {

/**
 * @brief The Singleton class
 */
template<typename T>
class Singleton
{
public:
    /**
     * @brief 返回单例指针
     * @note: 不能使用返回静态局部变量的指针, 否则结束时会报错 _CrtlsValidHeapPointer(block),
     *        应该是 Qt 对指针进行了 delete, 然后结束时静态变量又自己 delete, 导致 double delete.
     * @return
     */
    static T *getInstance();

    /**
     * @brief 返回单例指针, 需要使用静态局部变量的指针, 否则最终不会释放资源
     * @return
     */
    static T *getInstanceCPP();
};

template<typename T>
T *Singleton<T>::getInstance()
{
    static T *instance = new T();
    return instance;
}

template<typename T>
T *Singleton<T>::getInstanceCPP()
{
    static T instance = T();
    return &instance;
}

} // namespace quicktools::common

/**
 * @brief c++ 单例
 */
#define SINGLETON(Class)                                               \
private:                                                               \
    friend class quicktools::common::Singleton<Class>;                 \
                                                                       \
public:                                                                \
    static Class *getInstance()                                        \
    {                                                                  \
        return quicktools::common::Singleton<Class>::getInstanceCPP(); \
    }

/**
 * @brief Qt QML 单例
 */
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

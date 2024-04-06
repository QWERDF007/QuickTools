#ifndef SINGLETON_H
#define SINGLETON_H

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

#define SINGLETON(Class)                                             \
private:                                                             \
    friend class Singleton<Class>;                                   \
                                                                     \
public:                                                              \
    static Class *getInstance()                                      \
    {                                                                \
        return Singleton<Class>::getInstance();                      \
    }                                                                \
                                                                     \
    static Class *create(QQmlEngine *qmlEngine, QJSEngine *jsEngine) \
    {                                                                \
        Q_UNUSED(qmlEngine)                                          \
        Q_UNUSED(jsEngine)                                           \
        return getInstance();                                        \
    }

#endif // SINGLETON_H

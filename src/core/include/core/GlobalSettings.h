#pragma once

#include "QuickToolSettings.h"

namespace quicktools::core {

class QUICKTOOLS_CORE_EXPORT GlobalSettings : public AbstractQuickToolSettings
{
    Q_OBJECT
    QML_NAMED_ELEMENT(GlobalSettings)
    QML_UNCREATABLE("Can't not create a AbstractQuickToolSettings directly")
    QML_SINGLETON
public:
    QString name() const override
    {
        return "GlobalSettings";
    }

    /**
     * @brief 获取单例实例的指针
     * @return
     */
    static GlobalSettings *getInstance()
    {
        if (instance_ == nullptr)
        {
            instance_ = new GlobalSettings;
        }
        return instance_;
    }

    /**
     * @brief 提供给 QML 创建一个单例实例的静态工厂函数
     * @param qmlEngine
     * @param jsEngine
     * @return
     */
    static GlobalSettings *create(QQmlEngine *qmlEngine, QJSEngine *jsEngine)
    {
        Q_UNUSED(qmlEngine)
        Q_UNUSED(jsEngine)
        return getInstance();
    }

private:
    explicit GlobalSettings(QObject *parent = nullptr);
    void addBasicSettings();

    static GlobalSettings *instance_;

private slots:
    void onSettingChanged(const QString &key, const QVariant &value);
};

} // namespace quicktools::core
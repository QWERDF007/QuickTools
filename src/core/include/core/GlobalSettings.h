#pragma once

#include "QuickToolSettings.h"
#include "common/Singleton.h"

namespace quicktools::core {

class QUICKTOOLS_CORE_EXPORT GlobalSettings : public AbstractQuickToolSettings
{
    Q_OBJECT
    QML_NAMED_ELEMENT(GlobalSettings)
    QML_UNCREATABLE("Can't not create a AbstractQuickToolSettings directly")
    QML_SINGLETON
public:
    QT_QML_SINGLETON(GlobalSettings)

    QString name() const override
    {
        return "GlobalSettings";
    }

private:
    explicit GlobalSettings(QObject *parent = nullptr);
    void addBasicSettings();

private slots:
    void onSettingChange(const QString &key, const QVariant &value);

signals:
    void changeStarted();
    void changeFinished();
};

} // namespace quicktools::core

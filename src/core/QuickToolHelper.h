#pragma once

#include <QObject>
#include <QtQml>

namespace quicktools::core {

class QuickToolHelper : public QObject
{
    Q_OBJECT
    QML_NAMED_ELEMENT(QuickToolHelper)
    QML_UNCREATABLE("Can't not create a QuickToolHelper directly")

public:
    explicit QuickToolHelper(QObject *parent = nullptr);

};

} // namespace quicktools::core

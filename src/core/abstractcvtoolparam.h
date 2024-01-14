#include "abstractquicktoolparam.h"

namespace quicktools::core {

class QUICKTOOLS_CORE_EXPORT AbstractCVInputParams : public AbstractInputParams
{
    Q_OBJECT

    QML_NAMED_ELEMENT(CVInputParams)
    QML_UNCREATABLE("Can't not create a AbstractCVInputParams directly")

    Q_PROPERTY(QString inputImages READ inputImages NOTIFY inputImagesChanged FINAL)
public:
    AbstractCVInputParams(QObject *parent = nullptr)
        : AbstractInputParams(parent)
    {
    }

    virtual ~AbstractCVInputParams() {}

    QString name() const override
    {
        return "AbstractCVInputParams";
    }

    QString inputImages() const
    {
        return "inputImages";
    }

signals:
    void inputImagesChanged();
};

class QUICKTOOLS_CORE_EXPORT AbstractCVOutputParams : public AbstractOutputParams
{
    Q_OBJECT

    QML_NAMED_ELEMENT(CVOutputParams)
    QML_UNCREATABLE("Can't not create a AbstractCVOutputParams directly")

    Q_PROPERTY(QString outputImages READ outputImages NOTIFY outputImagesChanged FINAL)
public:
    AbstractCVOutputParams(QObject *parent = nullptr)
        : AbstractOutputParams(parent)
    {
    }

    virtual ~AbstractCVOutputParams() {}

    QString name() const override
    {
        return "AbstractCVOutputParams";
    }

    QString outputImages() const
    {
        return "outputImages";
    }

signals:
    void outputImagesChanged();
};

} // namespace quicktools::core

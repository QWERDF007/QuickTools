#include "abstractquicktoolparam.h"

namespace quicktools::core {

class QUICKTOOLS_CORE_EXPORT AbstractCVInputParams : public AbstractInputParams
{
    Q_OBJECT

    QML_NAMED_ELEMENT(CVInputParams)
    QML_UNCREATABLE("Can't not create a AbstractCVInputParams directly")

    Q_PROPERTY(QVariant inputImages READ inputImages WRITE setInputImages NOTIFY inputImagesChanged FINAL)
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

    QVariant inputImages() const
    {
        return image_path_;
    }

    bool setInputImages(const QVariant &value);

protected:
    QVariant image_path_{""};

signals:
    void inputImagesChanged();
};

class QUICKTOOLS_CORE_EXPORT AbstractCVOutputParams : public AbstractOutputParams
{
    Q_OBJECT

    QML_NAMED_ELEMENT(CVOutputParams)
    QML_UNCREATABLE("Can't not create a AbstractCVOutputParams directly")

    Q_PROPERTY(QVariant outputImages READ outputImages NOTIFY outputImagesChanged FINAL)
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

    bool setOutputImages(const QVariant &value);

    QVariant outputImages() const
    {
        return output_data_;
    }

protected:
    QVariant output_data_;

signals:
    void outputImagesChanged();
};

} // namespace quicktools::core

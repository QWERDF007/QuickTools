#include "abstractquicktoolparam.h"

namespace quicktools::core {

class QUICKTOOLS_CORE_EXPORT AbstractCVInputParams : public AbstractInputParams
{
    Q_OBJECT

    QML_NAMED_ELEMENT(CVInputParams)
    QML_UNCREATABLE("Can't not create a AbstractCVInputParams directly")

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

protected:
};

class QUICKTOOLS_CORE_EXPORT AbstractCVOutputParams : public AbstractOutputParams
{
    Q_OBJECT

    QML_NAMED_ELEMENT(CVOutputParams)
    QML_UNCREATABLE("Can't not create a AbstractCVOutputParams directly")

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

protected:
};

} // namespace quicktools::core

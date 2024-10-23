#pragma once

#include "CVToolShape.h"
#include "ImageProvider.h"
#include "QuickToolParams.h"


namespace quicktools::core {

class QUICKTOOLS_CORE_EXPORT CVInputParams : public InputParams
{
    Q_OBJECT
    QML_NAMED_ELEMENT(CVInputParams)
    QML_UNCREATABLE("Can't not create a CVInputParams directly")
    Q_PROPERTY(CVToolROI *roi READ roi CONSTANT FINAL)
public:
    CVInputParams(QObject *parent = nullptr, QQmlEngine *qml_engine = nullptr, QJSEngine *js_engine = nullptr);

    virtual ~CVInputParams() {}

    QString name() const override
    {
        return "CVInputParams";
    }

    /**
     * @brief 添加输入图像参数
     * @param[in] name 参数名称
     * @param[in] uuid 工具的 uuid
     * @param[in] display_name 参数展示名称
     * @param[in] desc 参数描述
     * @param[in] is_property 参数是否作为属性被界面访问
     * @param[in] visible 参数是否可见
     * @return
     */
    ImageProviderWrapper *addImage(const QString &name, const QString &uuid, const QString &display_name,
                                   const QString &desc, const bool is_property = false, const bool visible = true);

    CVToolROI *roi();

private:
    CVToolROI *roi_{nullptr};
};

class QUICKTOOLS_CORE_EXPORT CVOutputParams : public OutputParams
{
    Q_OBJECT
    QML_NAMED_ELEMENT(CVOutputParams)
    QML_UNCREATABLE("Can't not create a CVOutputParams directly")
public:
    CVOutputParams(QObject *parent = nullptr, QQmlEngine *qml_engine = nullptr, QJSEngine *js_engine = nullptr);

    virtual ~CVOutputParams() {}

    QString name() const override
    {
        return "CVOutputParams";
    }

    CVToolShapeListModel *shapesListModel()
    {
        return shapes_list_model_;
    }

private:
    CVToolShapeListModel *shapes_list_model_;
};

} // namespace quicktools::core

#pragma once

#include "core/CVTool.h"
#include "core/QuickToolConfig.h"
#include "core/QuickToolManager.h"

#include <opencv2/core.hpp>

#include <QObject>

namespace quicktools::imgproc {

class ImageHistogram : public core::AbstractCVTool
{
    Q_OBJECT
public:
    ImageHistogram(QObject *parent = nullptr, QQmlEngine *qml_engine = nullptr, QJSEngine *js_engine = nullptr);

    ~ImageHistogram();

    std::tuple<int, QString> doInProcess() override;

    QString name() const override
    {
        return tr("图像直方图");
    }

    int acceptedShapes() const override
    {
        return core::CVToolShape::Rectangle | core::CVToolShape::Circle;
    }

    QString doc() const override;

private:
    int initInputParams() override;
    int initOutputParams() override;
    int initSettings() override;

    int cvtColor(const cv::Mat &src, cv::Mat &dst, const QString &color_space);

    static QString doc_;

    inline static const QVariantList COLOR_SPACES{
        "UNCHANGED",
        "RGB",
        "HSV",
        "Gray",
    };

    core::ImageProviderWrapper *image_provider_{nullptr};
};

class ImageHistogramConfig : public core::AbstractQuickToolConfig
{
public:
    QString title() const override
    {
        return QString("图像直方图");
    }

    QString url() const override
    {
        return "qrc:/qt/qml/QuickTools/imgproc/imagehistogram/ImageHistogram.qml";
    }

    QString desc() const override
    {
        return QString("图像中像素值分布, 横坐标为像素值, 纵坐标为像素值的统计");
    }

    QString icon() const override
    {
        return "/icons/bar";
    }

    QVariantMap extra() const override
    {
        return {
            {"recentlyUpdated", true},
            {  "recentlyAdded", true},
        };
    }

    int task() const override
    {
        return core::quicktooltasktype::QuickToolTaskType::EmptyTask;
    }

    int group() const override
    {
        return core::quicktoolgrouptype::QuickToolGroupType::ImageProcessGroup;
    }
};

REGISTER_QUICKTOOL(core::quicktooltype::ImageHistogramTool, ImageHistogram, ImageHistogramConfig)

} // namespace quicktools::imgproc

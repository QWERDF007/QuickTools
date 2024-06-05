#include "samples/PyTest.h"

#include <iostream>

namespace quicktools::samples {

using core::paramtypes::QuickToolParamRole;
using core::paramtypes::QuickToolParamType;

PyTest::PyTest(QObject *parent)
    : core::AbstractPythonCVTool(parent)
{
}

std::tuple<int, QString> PyTest::process()
{
    auto    algorithm_start_time = std::chrono::high_resolution_clock::now();
    int     status               = 0;
    QString msg                  = tr("运行成功");
    auto    input_params         = getInputParams();
    auto    output_params        = getOutputParams();
    if (input_params == nullptr || output_params == nullptr)
        return {-1, tr("输入/输出参数为空指针")};
    pybind11::gil_scoped_acquire acquire;
    QString                      result;
    {
        pybind11::module_ module = pybind11::module_::import(importModule().toLocal8Bit().constData());
        module.reload();
        pybind11::object res = module.attr("get_current_time")();
        result               = QString::fromStdString(res.cast<std::string>());
    }
    //    if (object_)
    //    {
    //        pybind11::object res = object_.attr("get_current_time")();
    //        result = QString::fromStdString(res.cast<std::string>());
    //    }
    //    else
    //    {
    //        status = -1;
    //        msg = tr("模块未初始化: ") + importModule();
    //    }
    auto algorithm_end_time = std::chrono::high_resolution_clock::now();
    auto algorithm_time = std::chrono::duration<double, std::milli>(algorithm_end_time - algorithm_start_time).count();
    addAlgorithmTime(algorithm_time);
    output_params->setData("Now", result);
    return {status, msg};
}

int PyTest::initInputParams()
{
    return 0;
}

int PyTest::initOutputParams()
{
    if (output_params_)
    {
        output_params_->addParam("Now", tr("当前时间"), "", QuickToolParamType::ParamTextType, QVariant(), QVariant(),
                                 false, true);
    }
    return 0;
}

int PyTest::initSettings()
{
    return 0;
}

QString PyTest::importModule() const
{
    return "mytest";
}

} // namespace quicktools::imgproc

#include "samples/PyTest.h"

#include "core/PythonInterface.h"

namespace quicktools::samples {

using core::paramtypes::QuickToolParamRole;
using core::paramtypes::QuickToolParamType;

class PyTestPythonInterface : public core::AbstractPythonInterface
{
public:
    PyTestPythonInterface(QObject *parent = nullptr)
        : core::AbstractPythonInterface(parent)
    {
    }

protected:
    QString importModule() const override
    {
        return "pytest";
    }
};

PyTest::PyTest(QObject *parent, QQmlEngine *qml_engine, QJSEngine *js_engine)
    : AbstractTool(parent, qml_engine, js_engine)
{
    python_interface_ = new PyTestPythonInterface(this);
}

std::tuple<int, QString> PyTest::doInProcess()
{
    auto algorithm_start_time = std::chrono::high_resolution_clock::now();

    int     status = 0;
    QString msg    = tr("运行成功");

    auto input_params  = getInputParams();
    auto output_params = getOutputParams();
    if (input_params == nullptr || output_params == nullptr)
        return {-1, tr("输入/输出参数为空指针")};

    QString key = input_params->data("Key", QuickToolParamRole::ParamValueRole).toString();
    QString now;
    double  value{0};

    pybind11::gil_scoped_acquire acquire;
    {
        pybind11::object current_time = python_interface_->module.attr("get_current_time")();
        pybind11::object v            = python_interface_->module.attr(key.toStdString().c_str());

        now   = QString::fromStdString(current_time.cast<std::string>());
        value = v.cast<double>();
    }

    auto algorithm_end_time = std::chrono::high_resolution_clock::now();
    auto algorithm_time = std::chrono::duration<double, std::milli>(algorithm_end_time - algorithm_start_time).count();

    addAlgorithmTime(algorithm_time);
    output_params->setData("Now", now);
    output_params->setData("Var", value);

    return {status, msg};
}

int PyTest::initInputParams()
{
    if (input_params_)
    {
        input_params_->addParam("Key", tr("变量名"), tr("需要获取的变量的名称"), QuickToolParamType::TextParamType,
                                "BB", QVariant(), true, false, true, true);
    }
    return 0;
}

int PyTest::initOutputParams()
{
    if (output_params_)
    {
        output_params_->addParam("Now", tr("当前时间"), "", QuickToolParamType::TextParamType, QVariant(), QVariant(),
                                 false, true);
        output_params_->addParam("Var", tr("变量值"), "", QuickToolParamType::DoubleParamType, QVariant(), QVariant(),
                                 false, true);
    }
    return 0;
}

int PyTest::initSettings()
{
    return 0;
}

} // namespace quicktools::samples

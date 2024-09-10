#pragma once

#include "CoreGlobal.h"

#include <opencv2/highgui.hpp>

#include <QObject>
#include <QStringList>


#undef slots
#include <pybind11/embed.h>
#include <pybind11/numpy.h>
#define slots Q_SLOTS

namespace quicktools::core {

class QUICKTOOLS_CORE_EXPORT AbstractPythonInterface : public QObject
{
    Q_OBJECT
public:
    AbstractPythonInterface(QObject *parent = nullptr);

    /**
     * @brief 析构, 释放 @ref module 和 @ref obj 的资源
     */
    virtual ~AbstractPythonInterface();

    /**
     * @brief 初始化, 导入 @ref importModule 中定义的模块
     * @return
     */
    std::tuple<int, QString> init();

    /**
     * @brief 重新加载 @ref importModule 中定义的模块 , 刷新 python 代码
     * @return
     */
    std::tuple<int, QString> reloadModule();

    /**
     * @brief 释放 @ref module 和 @ref obj 的资源
     */
    void release();

    /**
     * @brief 用于模块导入后持久化, 在接口类释放时释放资源
     */
    pybind11::module_ module;

    /**
     * @brief 可以用于模块内定义的类实例持久化, 在接口类释放时释放资源
     */
    pybind11::object obj;

protected:
    /**
     * @brief 导入的 python 模块
     * @return
     */
    virtual QString importModule() const = 0;
};

class PythonHelper
{
public:
    /**
     * @brief 检查模板类型是否为 float 或 uint8_t
     *
     * @tparam T
     */
    template<typename T>
    using is_float_or_uint8 = std::conditional_t<std::is_same<T, float>::value || std::is_same<T, uint8_t>::value,
                                                 std::true_type, std::false_type>;

    /**
     * @brief 将 cv::Mat 转换成 pybind11 的 pybind11::array_t,
     *        pybind11 传递给 python 时转换为 numpy.ndarray
     * @param img
     * @return
     */
    template<typename T>
    static typename std::enable_if_t<is_float_or_uint8<T>::value, pybind11::array_t<T>> toNumpy(const cv::Mat &img)
    {
        const size_t rows      = img.rows;
        const size_t cols      = img.cols;
        const size_t chs       = img.channels();
        const size_t item_size = img.elemSize1();

        bool single_channel = chs == 1;

        const std::string format = pybind11::format_descriptor<T>::format();
        pybind11::ssize_t ndim   = single_channel ? 2 : 3;

        pybind11::array::ShapeContainer shape = single_channel ? pybind11::array::ShapeContainer{rows, cols}
                                                               : pybind11::array::ShapeContainer{rows, cols, chs};
        pybind11::array::ShapeContainer strides
            = single_channel ? pybind11::array::ShapeContainer{img.step[0], item_size}
                             : pybind11::array::ShapeContainer{img.step[0], img.step[1], item_size};

        return pybind11::array_t<T>(pybind11::buffer_info(img.data, item_size, format, ndim, shape, strides));
    }

    /**
     * @brief 将 pybind11::array_t 转换成 cv::Mat
     * @param obj
     * @return
     */
    template<typename T>
    static typename std::enable_if_t<is_float_or_uint8<T>::value, cv::Mat> fromNumpy(const pybind11::object &obj)
    {
        if (!obj.is_none() && pybind11::isinstance<pybind11::array_t<T>>(obj))
        {
            pybind11::array_t<T>  buf  = obj;
            pybind11::buffer_info info = buf.request();

            int rows = info.shape[0];
            int cols = info.shape[1];
            int chs  = info.shape.size() == 2 ? 1 : info.shape[2];
            int type = info.format == "B" ? CV_8UC(chs) : CV_32FC(chs);

            return cv::Mat(rows, cols, type, info.ptr);
        }
        return cv::Mat();
    }
};

} // namespace quicktools::core

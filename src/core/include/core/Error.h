#pragma once

#include "CoreGlobal.h"

#include <QObject>
#include <QString>
#include <map>

namespace quicktools {

//! @addtogroup core_utils
//! @{

namespace Error {

/**
 * @brief 工具错误码
 */
enum Code
{
    Success = 0,        //!< 一切正常
    Ok      = 0,        //!< Success 别名
    InvalidArgument,    //!< 无效的参数
    InternalError,      //!< 发生内部错误
    InputParamsEmpty,   //!< 输入参数为空
    OutputParamsEmpty,  //!< 输出参数为空
    RuntimeParamsEmpty, //!< 运行时参数为空
    FilePathEmpty,      //!< 文件路径为空
    FileNotFound,       //!< 找不到文件
    ImageFilePathEmpty, //!< 图像路径为空

    // DeepLearning
    DeepLearningError = 1000, //!< 深度学习错误
    ModelFilePathEmpty,       //!< 模型文件路径为空
    ModelFileInvalid,         //!< 模型文件无效
};

class QUICKTOOLS_CORE_EXPORT ErrorGenerator : public QObject
{
public:
    /**
     * @brief 获取错误码对应的错误信息
     * @param[in] code 错误码 @ref Error::Code
     * @return
     */
    static QString getErrorString(const int code, const QString &additional_msg = "");

    static std::map<int, QString> error_msg_maps_;
};

} // namespace Error

//! @} core_utils

} // namespace quicktools

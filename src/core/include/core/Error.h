#pragma once

#include "CoreGlobal.h"

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
    Success          = 0,  //!< 一切正常
    Ok               = 0,  //!< Success 别名
    InvalidArgument  = -1, //!< 无效的参数
    InternalError    = -2, //!< 发生内部错误
    InputParamsEmpty = -3, //!< 输入参数为空
    FileNotFound     = -4, //!< 找不到文件

    // DeepLearning
    ModelFileEmpty   = -1000, //!< 模型文件为空
    ModelFileInvalid = -1001, //!< 模型文件无效
};

class QUICKTOOLS_CORE_EXPORT ErrorGenerator
{
public:
    /**
     * @brief 获取错误码对应的错误信息
     * @param[in] code 错误码 @ref Error::Code
     * @return
     */
    static QString getErrorString(const int code, const QString& additional_msg = "");

    static std::map<int, QString> error_msg_maps_;
};

} // namespace Error

//! @} core_utils

} // namespace quicktools

#pragma once

#include <QString>

namespace quicktools {

//! @addtogroup core_utils
//! @{

namespace Error {

/**
 * @brief 工具错误码
 */
enum Code
{
    Success         = 0,  //!< 一切正常
    Ok              = 0,  //!< Success 别名
    InvalidArgument = -1, //!< 无效的参数
    InternalError   = -2, //!< 发生内部错误
};

/**
 * @brief 获取错误码对应的错误信息
 * @param[in] code 错误码 @ref Error::Code
 * @return
 */
QString getErrorString(const int code);

} // namespace Error

//! @} core_utils

} // namespace quicktools

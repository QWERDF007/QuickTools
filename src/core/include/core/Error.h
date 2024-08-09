#pragma once

namespace quicktools {

//! @addtogroup core_utils
//! @{

namespace Error {
enum Code
{
    Success         = 0,  //!< 一切正常
    Ok              = 0,  //!< Success 别名
    InvalidArgument = -1, //!< 无效的参数
    InternalError   = -2, //!< 发生内部错误
};
} // namespace Error

//! @} core_utils

} // namespace quicktools

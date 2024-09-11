#include "core/Error.h"

namespace quicktools::Error {

std::map<int, QString> ErrorGenerator::error_msg_maps_ = {
    {         Success,       "运行成功"},
    { InvalidArgument,    "无效的参数"},
    {   InternalError, "发生内部错误"},
    {InputParamsEmpty, "输入参数为空"},
    {    FileNotFound,    "找不到文件"},
    { InputImageEmpty, "输入图像为空"},

    // DeepLearning
    {  ModelFileEmpty, "模型文件为空"},
    {ModelFileInvalid, "模型文件无效"},
};

QString ErrorGenerator::getErrorString(const int code, const QString &additional_msg)
{
    auto error_msg = error_msg_maps_.find(code);
    if (error_msg == error_msg_maps_.end())
        return "未定义错误";
    return error_msg->second + additional_msg;
}

} // namespace quicktools::Error

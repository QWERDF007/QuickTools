#include "core/Error.h"

namespace quicktools::Error {

std::map<int, QString> ErrorGenerator::error_msg_maps_ = {
    {           Success, tr("运行成功")},
    {   InvalidArgument, tr("无效的参数")},
    {     InternalError, tr("发生内部错误")},
    {  InputParamsEmpty, tr("输入参数为空")},
    { OutputParamsEmpty, tr("输入参数为空")},
    {     FilePathEmpty, tr("文件路径为空")},
    {      FileNotFound, tr("找不到文件")},
    {ImageFilePathEmpty, tr("图像路径为空")},

    // DeepLearning
    { DeepLearningError, tr("深度学习错误")},
    {ModelFilePathEmpty, tr("模型文件路径为空")},
    {  ModelFileInvalid, tr("模型文件无效")},
};

QString ErrorGenerator::getErrorString(const int code, const QString &additional_msg)
{
    auto error_msg = error_msg_maps_.find(code);
    if (error_msg == error_msg_maps_.end())
        return tr("未定义错误");
    return error_msg->second + additional_msg;
}

} // namespace quicktools::Error

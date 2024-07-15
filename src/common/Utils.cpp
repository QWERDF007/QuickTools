#include "common/Utils.h"

#include <QUuid>
#include <QStringList>

namespace quicktools::common {

QString uuid()
{
    return QUuid::createUuid().toString().remove('-').remove('{').remove('}');
}

QString toQString(const QStringList & str_list, const QString &sep, Qt::SplitBehavior behavior)
{
    QString str;
    if (str_list.isEmpty())
        return str;
    int size = str_list.size() - 1;
    if (size == 0)
        return str_list[0];
    for (int i = 0; i < size; ++i)
    {
        if (behavior == Qt::SkipEmptyParts && str_list[i].isEmpty())
            continue;
        str += str_list[i] + sep;
    }
    str += str_list[size];
    return str;
}

#ifdef _WIN32

std::string wcharToString(const wchar_t *wstr)
{
    if (wstr == nullptr)
        return "";
    // 首先计算需要的字符数量
    int len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, nullptr, 0, nullptr, nullptr);

    // 创建一个足够大的字符数组来存储转换后的字符串
    std::string str(len, 0);

    // 执行转换
    WideCharToMultiByte(CP_UTF8, 0, wstr, -1, &str[0], len, nullptr, nullptr);

    return str;
}

std::wstring stringToWchar(const std::string &str)
{
    if (str.empty())
        return L"";

    // 首先计算需要的宽字符数量
    int len = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);

    // 创建一个足够大的宽字符数组来存储转换后的字符串
    std::wstring wstr(len, 0);

    // 执行转换
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &wstr[0], len);

    return wstr;
}

#endif // _WIN32

} // namespace quicktools::common

#include "common/Utils.h"

#include <QDir>
#include <QFileInfo>
#include <QStringList>
#include <QUuid>

namespace quicktools::common {

QString uuid()
{
    return QUuid::createUuid().toString().remove('-').remove('{').remove('}');
}

QString toQString(const QStringList &str_list, const QString &sep, Qt::SplitBehavior behavior)
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

QString getDirectory(const QString &path)
{
    QFileInfo fileinfo(path);
    if (fileinfo.isFile() || fileinfo.isDir())
        return fileinfo.dir().path();
    else
        return path;
}

std::vector<QString> getFiles(const QString &path, const QStringList &name_filters, bool recursive)
{
    QFileInfo fileinfo(path);
    if (fileinfo.isFile())
    {
        return {path};
    }
    else if (fileinfo.isDir())
    {
        QDir dir(path);
        dir.setNameFilters(name_filters);
        dir.setFilter(recursive ? QDir::Files : QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
        dir.setSorting(QDir::Name);
        std::vector<QString> files;
        for (const auto &entry_info : dir.entryInfoList())
        {
            if (entry_info.isFile())
            {
                files.emplace_back(entry_info.absoluteFilePath());
            }
            else if (recursive && entry_info.isDir())
            {
                std::vector<QString> tmp_files = getFiles(entry_info.absoluteFilePath(), name_filters, recursive);
                files.insert(files.end(), tmp_files.begin(), tmp_files.end());
            }
        }
        return files;
    }
    else
    {
        return {};
    }
}

std::vector<QString> getDirectories(const QString &path, bool recursive)
{
    QFileInfo fileinfo(path);
    if (fileinfo.isDir())
    {
        QDir dir(path);
        dir.setFilter(recursive ? QDir::Dirs : QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
        dir.setSorting(QDir::Name);
        std::vector<QString> dirs;
        for (const auto &entry_info : dir.entryInfoList())
        {
            if (entry_info.isDir())
            {
                const QString &tmp_path = entry_info.absoluteFilePath();
                dirs.emplace_back(tmp_path);
                if (recursive)
                {
                    std::vector<QString> tmp_dirs = getDirectories(tmp_path);
                    dirs.insert(dirs.end(), tmp_dirs.begin(), tmp_dirs.end());
                }
            }
        }
        return dirs;
    }
    else
    {
        return {};
    }
}

const QString FileReader::read(const QString &root, bool recursive, bool circular)
{
    QString path;
    if (root_ != root)
    {
        root_  = root;
        cur_   = 0;
        paths_ = getFiles(root, name_filters_, recursive);
    }
    if (paths_.empty())
        return path;
    path = paths_[cur_];
    if (circular)
        cur_ = (cur_ + 1) % size();
    else
        cur_ = std::min<int>(size() - 1, cur_ + 1);
    return path;
}

#endif // _WIN32

} // namespace quicktools::common

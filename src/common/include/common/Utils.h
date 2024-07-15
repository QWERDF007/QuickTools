#pragma once

#include <QString>
#include <string>

#ifdef _WIN32
#    include <Windows.h>
#endif

namespace quicktools::common {

QString uuid();
QString toQString(const QStringList&, const QString &sep=", ", Qt::SplitBehavior behavior = Qt::KeepEmptyParts);

#ifdef _WIN32
std::string  wcharToString(const wchar_t *wstr);
std::wstring stringToWchar(const std::string &str);
#endif

} // namespace quicktools::common

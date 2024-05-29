#pragma once

#include <string>
#include <QString>

#ifdef _WIN32
#include <Windows.h>
#endif


namespace quicktools::common {

QString uuid();

#ifdef _WIN32
std::string wcharToString(const wchar_t *wstr);
std::wstring stringToWchar(const std::string &str);
#endif

} // namespace quicktools::common

#pragma once

#include "core/CoreExport.h"

#include <string>

namespace quicktools::core {

// 获取完整版本信息字符串，包含版本号、分支、提交哈希和构建时间
CORE_API std::string GetFullVersionString();

// 获取版本号字符串，格式如 "0.0.1-beta"
CORE_API std::string GetVersionString();

// 获取 Git 分支名称
CORE_API std::string GetBranchString();

// 获取 Git 提交哈希值
CORE_API std::string GetCommitHashString();

// 获取构建时间字符串
CORE_API std::string GetBuildTimeString();

} // namespace quicktools::core

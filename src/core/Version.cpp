#include "core/Version.h"
#include "core/VersionDef.h"
#include "core/detail/VersionUtils.h"

#include <string>

namespace quicktools::core {

std::string GetVersionString()
{
    return QUICKTOOLS_VERSION_STRING;
}

std::string GetBranchString()
{
    return QUICKTOOLS_BRANCH;
}

std::string GetCommitHashString()
{
    return QUICKTOOLS_COMMIT;
}

std::string GetFullVersionString()
{
    std::string s = QUICKTOOLS_VERSION_STRING;
    s += " - [";
    s += QUICKTOOLS_BRANCH;
    s += "] - (";
    s += QUICKTOOLS_COMMIT;
    s += ")";
    return s;
}

std::string GetBuildTimeString()
{
    return QUICKTOOLS_BUILD_TIME;
}

} // namespace quicktools::core

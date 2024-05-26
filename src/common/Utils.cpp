#include "Utils.h"
#include <QString>
#include <QUuid>

namespace quicktools::common {

QString uuid()
{
    return QUuid::createUuid().toString().remove('-').remove('{').remove('}');
}

} // namespace quicktools::common

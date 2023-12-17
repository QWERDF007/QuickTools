#include "quickfont.h"

namespace QuickTools::ui {

QuickFont *QuickFont::instance_ = nullptr;

QuickFont::QuickFont(QObject *parent)
    : QObject(parent)
{
    QFont body;
    body.setPixelSize(13);
    Body(body);
}

QuickFont::~QuickFont()
{

}

QuickFont *QuickFont::getInstance()
{
    if (instance_ == nullptr)
    {
        instance_ = new QuickFont;
    }
    return instance_;
}



} // namespace QuickTools::ui

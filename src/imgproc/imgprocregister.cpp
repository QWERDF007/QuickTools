#include "imgprocregister.h"

#include "imagehistogram/imagehistogram.h"
#include "abstractquicktool.h"

namespace quicktools::imgproc {

void registerTools()
{
    registerImageHistogram();
}

} // namespace quicktools::imgproc

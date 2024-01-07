#include "imgprocregister.h"

#include "abstractquicktool.h"
#include "imagehistogram/imagehistogram.h"

namespace quicktools::imgproc {

void registerTools()
{
    registerImageHistogram();
}

} // namespace quicktools::imgproc

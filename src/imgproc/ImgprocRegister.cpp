#include "imgprocregister.h"

#include "imagehistogram/ImageHistogram.h"

namespace quicktools::imgproc {

void registerTools()
{
    registerImageHistogram();
}

} // namespace quicktools::imgproc

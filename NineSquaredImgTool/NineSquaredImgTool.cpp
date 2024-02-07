#include <iostream>
#include <opencv2/opencv.hpp>
#include "NineSquareMaker.h"
#include "ImagePathHelper.h"
#include "Windows.h"

void ExportCallback(int state,void* userdata)
{
    HIGH_LIGHT_MESSAGE("Created")
}

int main(int argc, char* argv[])
{
    ImagePathHelper ImagePath = ImagePathHelper();
    ImagePath.InitWithSelector();
    ImagePath.PrepareExport();
    cv::namedWindow("NineSquareMakerWindow",cv::WINDOW_NORMAL);
    cv::String TestImgPath = ImagePath.OriginPath;
    NineSquareMaker* Maker = new NineSquareMaker("NineSquareMakerWindow");
    
    Maker->LoadMainImg(TestImgPath);
    Maker->ExportMainImgAsNineSquared(ImagePath);
    //cv::waitKey(0);
    printf("Finished");
    ImagePath.OpenExportFolder();
    
    return 0;
}

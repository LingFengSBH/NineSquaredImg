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
    system("chcp 65001");
    ImagePathHelper ImagePath = ImagePathHelper();
    HIGH_LIGHT_MESSAGE(R"(Place select a image as main image of Nine-Squared images, click 'Open' to execute)")
    ImagePath.InitWithSelector();
    ImagePath.PrepareExport();
    cv::namedWindow("NineSquareMakerWindow",cv::WINDOW_NORMAL);
    cv::String TestImgPath = ImagePath.OriginPath;
    NineSquareMaker* Maker = new NineSquareMaker("NineSquareMakerWindow");
    
    Maker->LoadMainImg(TestImgPath);
    HIGH_LIGHT_MESSAGE(R"(You chose image: )"<<ImagePath.ImageName<<R"(. Now, let's create each image of Nine-Squared images)")
    Maker->ExportMainImgAsNineSquared(ImagePath);
    //cv::waitKey(0);
    HIGH_LIGHT_MESSAGE(R"(Nine-Squared images had been exported to path: )"<<ImagePath.ExportFolder)
    ImagePath.OpenExportFolder();
    
    return 0;
}

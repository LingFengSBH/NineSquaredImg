#pragma once
#include <string>
#include <opencv2/opencv.hpp>
#include "SharedLibrary.h"
#include "ImagePathHelper.h"
using namespace std;

class NineSquareMaker
{
protected:
    cv::Mat CurrentMainImg;
    cv::Size MainImgOrigineSize = cv::Size(0,0);
    cv::Size SquaredMainImgSize = cv::Size(0,0);
    cv::Size LUOffset = cv::Size(0,0);

    cv::String WindowName;
    //Events
    void OnMainImgChanged();

    cv::Mat RenderCanvas = cv::Mat::zeros(128,128,CV_8UC3); 
    list<cv::Mat> CurrentSquareSubImages;

    
public:
    NineSquareMaker(cv::String WindowName);
    bool LoadMainImg(cv::String ImgPath);
    bool SetMainImgAs(cv::Mat* TargetMat);
    cv::Mat GetMainImg();

    cv::Range GetNineSquaredRangeX(int Index);
    cv::Range GetNineSquaredRangeY(int Index);

    cv::Mat GetNineSquaredMatInIndex(int U,int V);
    

    static void CopyMatToRange(cv::Mat from,cv::Mat to,cv::Rect Range);
    static void FillMat(cv::Mat Mat,cv::Scalar Color);

    void ExportMainImgAsNineSquared(ImagePathHelper& PathHelper);
    void RenderAndShowCurrentCanvas(cv::Mat MainSquaredImg);
};

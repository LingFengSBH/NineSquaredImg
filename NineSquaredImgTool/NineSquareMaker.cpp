#include "NineSquareMaker.h"

void NineSquareMaker::OnMainImgChanged()
{
    MainImgOrigineSize = CurrentMainImg.size();
    HIGH_LIGHT_MESSAGE("Image changed,new image with size:"<<MainImgOrigineSize)
    int AimSquaredWidth = min(MainImgOrigineSize.width,MainImgOrigineSize.height);
    AimSquaredWidth = static_cast<int>(floor(static_cast<float>(AimSquaredWidth) / 3.0f));
    SquaredMainImgSize = cv::Size(AimSquaredWidth,AimSquaredWidth);
    AimSquaredWidth *= 3;
    LUOffset = (MainImgOrigineSize-cv::Size(AimSquaredWidth,AimSquaredWidth))/2;
}

NineSquareMaker::NineSquareMaker(cv::String InWindowName)
{
    WindowName = InWindowName;
}

bool NineSquareMaker::LoadMainImg(cv::String ImgPath)
{
    CurrentMainImg = cv::imread(ImgPath);
    OnMainImgChanged();
    return CurrentMainImg.data;
}

bool NineSquareMaker::SetMainImgAs(cv::Mat* TargetMat)
{
    CurrentMainImg = *TargetMat;
    OnMainImgChanged();
    return CurrentMainImg.data;
}

cv::Mat NineSquareMaker::GetMainImg()
{
    return CurrentMainImg;
}

cv::Range NineSquareMaker::GetNineSquaredRangeX(int Index)
{
    int Start = Index*SquaredMainImgSize.width+LUOffset.width;
    return cv::Range(Start,Start+SquaredMainImgSize.width);
}

cv::Range NineSquareMaker::GetNineSquaredRangeY(int Index)
{
    int Start = Index*SquaredMainImgSize.height+LUOffset.height;
    return cv::Range(Start,Start+SquaredMainImgSize.height);
}

cv::Mat NineSquareMaker::GetNineSquaredMatInIndex(int U, int V)
{
    return CurrentMainImg(GetNineSquaredRangeY(V),GetNineSquaredRangeX(U));
}

void NineSquareMaker::CopyMatToRange(cv::Mat from, cv::Mat to, cv::Rect Range)
{
    cv::Mat TempMat = cv::Mat::zeros(Range.height,Range.width,CV_8UC3);
    cv::resize(from,TempMat,cv::Size(Range.width,Range.height),0,0,cv::INTER_LINEAR);
    TempMat.copyTo(to(Range));
}

void NineSquareMaker::FillMat(cv::Mat Mat, cv::Scalar Color)
{
    cv::Size AimSize = Mat.size();
    Mat = cv::Mat::zeros(AimSize.height,AimSize.width,CV_8UC3);
    vector<cv::Point> Points;
    Points.push_back(cv::Point(0,0));
    Points.push_back(cv::Point(AimSize.width,0));
    Points.push_back(cv::Point(AimSize.width,AimSize.height));
    Points.push_back(cv::Point(0,AimSize.height));
    cv::fillConvexPoly(Mat,Points,cv::Scalar(255,255,255));
}

void NineSquareMaker::ExportMainImgAsNineSquared(ImagePathHelper& PathHelper)
{
    if(!PathHelper.PreparedForExport)
    {
        PathHelper.PrepareExport();
    }
   
    for(int iy = 0;iy <= 2; iy++)
    {
        for(int ix = 0;ix <= 2; ix++)
        {
            CurrentSquareSubImages.clear();
            HIGH_LIGHT_MESSAGE("Display Image Index: "<<ix<<","<<iy)
            cv::Mat CurrentMat = GetNineSquaredMatInIndex(ix,iy);
            RenderAndShowCurrentCanvas(CurrentMat);
            
            std::string SelectedPath = ImagePathHelper::SelectPath();
            while(SelectedPath != "")
            {
                cv::Mat SelectMat = cv::imread(SelectedPath);
                CurrentSquareSubImages.emplace_back(SelectMat);
                RenderAndShowCurrentCanvas(CurrentMat);
                SelectedPath = ImagePathHelper::SelectPath();
            }
           
            
            string Suffix = "_"+std::to_string(iy)+"_"+std::to_string(ix);
            cv::imwrite(PathHelper.CreateExportPathWithSuffix(Suffix),RenderCanvas);
            HIGH_LIGHT_MESSAGE("Export Image to : "<<PathHelper.CreateExportPathWithSuffix(Suffix))
            
        }
    }
}

void NineSquareMaker::RenderAndShowCurrentCanvas(cv::Mat MainSquaredImg)
{
    int Width = MainSquaredImg.rows;
    
    list<cv::Mat>::iterator SubMat;
    int TotalSubImages = static_cast<int>(CurrentSquareSubImages.size());
    int CurrentIndex = 0;
    int UHeight = 0;
    int DHeight = 0;

    list<pair<int,cv::Mat>> DesiredHeightMap; 
    for(SubMat = CurrentSquareSubImages.begin();SubMat!=CurrentSquareSubImages.end();SubMat++)
    {
        cv::Mat CurrentSubMat = *SubMat;
        cv::Size SubMatSize = CurrentSubMat.size();
        int Height = (int)((float)SubMatSize.height/(float)SubMatSize.width*(float)Width);
        HIGH_LIGHT_MESSAGE("Size = "<<SubMatSize)
        HIGH_LIGHT_MESSAGE("Height = "<<Height)
        DesiredHeightMap.emplace_back(pair<int,cv::Mat>(Height,*SubMat));
        if(CurrentIndex<TotalSubImages/2)
        {
            UHeight += Height;
        }
        else
        {
            DHeight += Height;
        }
        CurrentIndex++;
    }
    
    UHeight = max(UHeight,DHeight);
    cv::Size AimSize = cv::Size(Width,Width+UHeight+UHeight);
    HIGH_LIGHT_MESSAGE("AimSize = "<<AimSize)
    RenderCanvas = cv::Mat::zeros(AimSize.height,AimSize.width,CV_8UC3);
    FillMat(RenderCanvas,cv::Scalar(255,255,255));

    CurrentIndex = 0;
    int HeightUsed = 0;
    list<pair<int,cv::Mat>>::iterator SubMatInfo;
    for(SubMatInfo = DesiredHeightMap.begin();SubMatInfo!=DesiredHeightMap.end();SubMatInfo++)
    {
        int CurrentHeight = SubMatInfo->first;
        cv::Mat CurrentSubMat = SubMatInfo->second;
        cv::Rect Range;
        if(CurrentIndex==TotalSubImages/2)
            HeightUsed = 0;
        
        if(CurrentIndex<TotalSubImages/2)
        {
            Range = cv::Rect(0,HeightUsed,Width,CurrentHeight);
        }
        else
        {
            Range = cv::Rect(0,HeightUsed+UHeight+Width,Width,CurrentHeight);
        }
        CopyMatToRange(CurrentSubMat,RenderCanvas,Range);

        HeightUsed += CurrentHeight;
        CurrentIndex++;
    }
    
    cv::Rect Range(0,UHeight,Width,Width);
    HIGH_LIGHT_MESSAGE(Range)
    CopyMatToRange(MainSquaredImg,RenderCanvas,Range);
    int ScaledHeight = (int)((float)AimSize.height/(float)AimSize.width*256.0);
    cv::resizeWindow(WindowName,256,ScaledHeight);
    cv::imshow(WindowName,RenderCanvas);
}

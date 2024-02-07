#pragma once
#include <string>
#include <Windows.h>
#include "SharedLibrary.h"


struct ImagePathHelper
{
    std::string OriginPath;
    std::string FolderPath;
    std::string ImageName;
    std::string ImageType;

    std::string ExportFolder;
    wchar_t* LPCWExportFolder;

    std::string SubFolder = "Export";

    bool PreparedForExport = false;

    std::basic_string<char>::size_type PathLength{};
    std::basic_string<char>::size_type NameP{};
    std::basic_string<char>::size_type TypeP{};
    
    ImagePathHelper(std::string Path);
    ImagePathHelper(): PathLength(0), NameP(0), TypeP(0){}
    ~ImagePathHelper(){delete[] LPCWExportFolder;}

    void InitWithPath(std::string Path);
    void InitWithSelector();

    static std::string SelectPath();
    static void StringToLPCW(std::string str,wchar_t*& out);
    static std::string LPCWToString(LPCWSTR lpcwstr);

    void PrepareExport();
    
    std::string CreateExportPathWithSuffix(std::string Suffix) const;
    cv::String CVCreateExportPathWithSuffix(std::string Suffix) const {return cv::String(CreateExportPathWithSuffix(Suffix));}
    cv::String CVGetOriginPath() const {return cv::String(OriginPath);}

    void OpenExportFolder();
};

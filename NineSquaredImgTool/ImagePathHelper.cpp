#include "ImagePathHelper.h"
#include "Windows.h"

ImagePathHelper::ImagePathHelper(std::string Path)
{
    InitWithPath(Path);
}

void ImagePathHelper::InitWithPath(std::string Path)
{
    OriginPath = Path;
    PathLength = OriginPath.length();
    NameP = OriginPath.rfind("\\")+1;
    TypeP = OriginPath.rfind(".")+0;
    FolderPath = OriginPath.substr(0,NameP);
    //HIGH_LIGHT_MESSAGE("Folder Path is: "<<FolderPath)

    ImageName = OriginPath.substr(NameP,TypeP-NameP);
    //HIGH_LIGHT_MESSAGE("ImageName is: "<<ImageName)

    ImageType = OriginPath.substr(TypeP,PathLength-TypeP);
    SubFolder = ImageName + SubFolder;
    //HIGH_LIGHT_MESSAGE("ImageType is: "<<ImageType)

    PreparedForExport = false;
}

void ImagePathHelper::InitWithSelector()
{
    InitWithPath(SelectPath());
}

std::string ImagePathHelper::SelectPath()
{
    OPENFILENAME ofn;
    char szFile[300];
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = (LPWSTR)szFile;
    ofn.lpstrFile[0] = '\0';
    ofn.nFilterIndex = 1;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = L"ALL\0*.*\0Text\0*.TXT\0";
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    std::string path_image = "";
    if (GetOpenFileName(&ofn))
    {
        path_image = LPCWToString(ofn.lpstrFile);
        return path_image;
    }
    else
    {
        return "";
    }
}

void ImagePathHelper::StringToLPCW(std::string str, wchar_t*& out)
{
    int length = static_cast<int>(str.length());
    out = new wchar_t[length+1];
    MultiByteToWideChar(CP_ACP,0,str.c_str(),length,out,length*sizeof(wchar_t));
    out[length] = 0;
}


std::string ImagePathHelper::LPCWToString(LPCWSTR lpcwstr)
{
    int length = WideCharToMultiByte(CP_ACP,0,lpcwstr,-1,NULL,0,NULL,NULL);
    if(length==0)
        return "";
    char* str = new char[length];
    WideCharToMultiByte(CP_ACP,0,lpcwstr,length*sizeof(wchar_t),str,length,NULL,NULL);
    str[length-1] = 0;
    std::string RetString(str);
    delete[] str;
    return RetString;
}

void ImagePathHelper::PrepareExport()
{
    if(LPCWExportFolder!=NULL)
        delete[] LPCWExportFolder;

    ExportFolder = FolderPath + SubFolder;
    StringToLPCW(ExportFolder,LPCWExportFolder);

    CreateDirectory(LPCWExportFolder,NULL);
    PreparedForExport = true;
}


std::string ImagePathHelper::CreateExportPathWithSuffix(std::string Suffix) const
{
    return ExportFolder + "\\"+  ImageName + Suffix + ImageType;
}

void ImagePathHelper::OpenExportFolder()
{
    ShellExecute(NULL,NULL,LPCWExportFolder,NULL,NULL,SW_SHOWNORMAL);
}

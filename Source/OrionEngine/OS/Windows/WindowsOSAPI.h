#pragma once

#if defined(_WIN64)
   #include <Windows.h>
   #include <commdlg.h>
#else
    #error OrionEngine only runs on Windows (64 bit).
#endif 

#pragma comment(lib, "Comdlg32.lib")

#include <filesystem>

namespace OrionEngine::OS
{
    std::filesystem::path OpenWindowsFileDialog(const wchar_t* filter)
    {
        wchar_t filePath[MAX_PATH] = {};

        OPENFILENAMEW openFileName = {};
        openFileName.lStructSize = sizeof(OPENFILENAMEW);
        openFileName.lpstrFile = filePath;
        openFileName.nMaxFile = MAX_PATH;
        openFileName.lpstrFilter = filter;
        openFileName.nFilterIndex = 1;
        openFileName.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

        if (GetOpenFileNameW(&openFileName))
            return std::filesystem::path(filePath);
        return {};
    }
}
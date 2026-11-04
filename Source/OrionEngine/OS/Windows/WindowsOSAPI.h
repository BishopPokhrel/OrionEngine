#pragma once

#if defined(_WIN64)

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <commdlg.h>
#include <wincodec.h>
#include <wrl/client.h>

#else
#error OrionEngine only runs on Windows (64 bit).
#endif 

#pragma comment(lib, "Comdlg32.lib")
#pragma comment(lib, "Windowscodecs.lib")
#pragma comment(lib, "Ole32.lib")

#include <filesystem>
#include <vector>

namespace OrionEngine::OS
{
    std::filesystem::path OpenWindowsFileDialog(const wchar_t* filter);
    std::vector<unsigned char> LoadWindowsImage(const std::filesystem::path& path, unsigned int& width, unsigned int& height);
}
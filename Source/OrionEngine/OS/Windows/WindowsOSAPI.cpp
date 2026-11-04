#include "WindowsOSAPI.h"

#include <limits>

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

    std::vector<unsigned char> LoadWindowsImage(const std::filesystem::path& path, unsigned int& width, unsigned int& height)
    {
        width = 0;
        height = 0;

        const HRESULT comResult = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
        if (FAILED(comResult) && comResult != RPC_E_CHANGED_MODE) return {};

        struct COMCleanup
        {
            bool b_Initialized;
            ~COMCleanup() { if (b_Initialized) CoUninitialize(); }
        };

        COMCleanup comCleanup{ SUCCEEDED(comResult) };

        using Microsoft::WRL::ComPtr;

        ComPtr<IWICImagingFactory> factory;
        HRESULT result = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(factory.GetAddressOf()));
        if (FAILED(result)) return {};

        ComPtr<IWICBitmapDecoder> decoder;
        result = factory->CreateDecoderFromFilename(path.c_str(), nullptr, GENERIC_READ, WICDecodeMetadataCacheOnLoad, decoder.GetAddressOf());
        if (FAILED(result)) return {};

        ComPtr<IWICBitmapFrameDecode> frame;
        result = decoder->GetFrame(0, frame.GetAddressOf());
        if (FAILED(result)) return {};

        UINT imageWidth = 0;
        UINT imageHeight = 0;

        result = frame->GetSize(&imageWidth, &imageHeight);
        if (FAILED(result) || imageWidth == 0 || imageHeight == 0) return {};

        ComPtr<IWICFormatConverter> converter;
        result = factory->CreateFormatConverter(converter.GetAddressOf());
        if (FAILED(result)) return {};

        result = converter->Initialize(frame.Get(), GUID_WICPixelFormat32bppRGBA, WICBitmapDitherTypeNone, nullptr, 0.0, WICBitmapPaletteTypeCustom);
        if (FAILED(result)) return {};

        constexpr UINT CHANNEL_COUNT = 4;

        if (imageWidth > (std::numeric_limits<UINT>::max)() / CHANNEL_COUNT) return {};

        const UINT stride = imageWidth * CHANNEL_COUNT;

        if (imageHeight > (std::numeric_limits<UINT>::max)() / stride) return {};

        const UINT bufferSize = stride * imageHeight;

        std::vector<unsigned char> pixels(bufferSize);

        result = converter->CopyPixels(nullptr, stride, bufferSize, pixels.data());
        if (FAILED(result)) return {};

        width = imageWidth;
        height = imageHeight;

        return pixels;
    }
}
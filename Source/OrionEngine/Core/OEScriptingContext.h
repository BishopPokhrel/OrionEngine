#pragma once

#define OE_ABI_VERSION 1

namespace OrionEngine
{
    namespace ECS
    {
        class OEECSRegistry;
    }

    namespace AssetManagement
    {
        class OEAssetManager;
    }

    class OEInputSystem;

    struct OEScriptingContext
    {
        ECS::OEECSRegistry* Registry = nullptr;
        OEInputSystem* InputSystem = nullptr;
        AssetManagement::OEAssetManager* AssetManager = nullptr;
    };
}
#include "OEEditorTheme.h"

#include <imgui.h>

namespace OrionEngine::OrionEditor
{
    void OEEditorTheme::ApplyTheme()
    {
        ImGui::StyleColorsDark();

        ImGuiStyle& style = ImGui::GetStyle();
        ImVec4* colors = style.Colors;

        // Main backgrounds
        colors[ImGuiCol_WindowBg] = ImVec4(0.012f, 0.016f, 0.024f, 1.00f);
        colors[ImGuiCol_ChildBg] = ImVec4(0.024f, 0.031f, 0.051f, 1.00f);
        colors[ImGuiCol_PopupBg] = ImVec4(0.027f, 0.031f, 0.043f, 0.98f);

        // Borders and separators
        colors[ImGuiCol_Border] = ImVec4(0.090f, 0.110f, 0.145f, 0.55f);
        colors[ImGuiCol_BorderShadow] = ImVec4(0.000f, 0.000f, 0.000f, 0.00f);
        colors[ImGuiCol_Separator] = ImVec4(0.090f, 0.115f, 0.160f, 0.65f);
        colors[ImGuiCol_SeparatorHovered] = ImVec4(0.120f, 0.300f, 0.510f, 0.78f);
        colors[ImGuiCol_SeparatorActive] = ImVec4(0.090f, 0.410f, 0.780f, 1.00f);

        // Text
        colors[ImGuiCol_Text] = ImVec4(0.835f, 0.875f, 0.930f, 1.00f);
        colors[ImGuiCol_TextDisabled] = ImVec4(0.365f, 0.400f, 0.460f, 1.00f);

        // Frames, inputs, and editable fields
        colors[ImGuiCol_FrameBg] = ImVec4(0.035f, 0.043f, 0.063f, 1.00f);
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.065f, 0.095f, 0.145f, 1.00f);
        colors[ImGuiCol_FrameBgActive] = ImVec4(0.085f, 0.130f, 0.205f, 1.00f);

        // Buttons - subdued blue until interacted with
        colors[ImGuiCol_Button] = ImVec4(0.045f, 0.070f, 0.115f, 1.00f);
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.075f, 0.145f, 0.245f, 1.00f);
        colors[ImGuiCol_ButtonActive] = ImVec4(0.090f, 0.260f, 0.465f, 1.00f);

        // Headers - hierarchy, details, and collapsible sections
        colors[ImGuiCol_Header] = ImVec4(0.045f, 0.075f, 0.125f, 1.00f);
        colors[ImGuiCol_HeaderHovered] = ImVec4(0.075f, 0.135f, 0.225f, 1.00f);
        colors[ImGuiCol_HeaderActive] = ImVec4(0.085f, 0.220f, 0.390f, 1.00f);

        // Tabs
        colors[ImGuiCol_Tab] = ImVec4(0.025f, 0.033f, 0.050f, 1.00f);
        colors[ImGuiCol_TabHovered] = ImVec4(0.075f, 0.145f, 0.245f, 1.00f);
        colors[ImGuiCol_TabSelected] = ImVec4(0.045f, 0.075f, 0.125f, 1.00f);
        colors[ImGuiCol_TabDimmed] = ImVec4(0.020f, 0.026f, 0.040f, 1.00f);
        colors[ImGuiCol_TabDimmedSelected] = ImVec4(0.035f, 0.050f, 0.075f, 1.00f);

        // Docking
        colors[ImGuiCol_DockingPreview] = ImVec4(0.090f, 0.340f, 0.650f, 0.55f);
        colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.008f, 0.010f, 0.016f, 1.00f);

        // Selection and interactive accents
        colors[ImGuiCol_CheckMark] = ImVec4(0.260f, 0.570f, 0.950f, 1.00f);
        colors[ImGuiCol_SliderGrab] = ImVec4(0.150f, 0.380f, 0.680f, 1.00f);
        colors[ImGuiCol_SliderGrabActive] = ImVec4(0.220f, 0.520f, 0.900f, 1.00f);
        colors[ImGuiCol_TextSelectedBg] = ImVec4(0.080f, 0.260f, 0.480f, 0.55f);

        // Scrollbars
        colors[ImGuiCol_ScrollbarBg] = ImVec4(0.008f, 0.010f, 0.016f, 0.70f);
        colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.055f, 0.075f, 0.110f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.085f, 0.130f, 0.200f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.120f, 0.240f, 0.400f, 1.00f);

        // Resize grips
        colors[ImGuiCol_ResizeGrip] = ImVec4(0.070f, 0.120f, 0.195f, 0.25f);
        colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.090f, 0.280f, 0.500f, 0.70f);
        colors[ImGuiCol_ResizeGripActive] = ImVec4(0.120f, 0.400f, 0.720f, 0.95f);

        // Window title bars
        colors[ImGuiCol_TitleBg] = ImVec4(0.012f, 0.016f, 0.024f, 1.00f);
        colors[ImGuiCol_TitleBgActive] = ImVec4(0.025f, 0.038f, 0.065f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.008f, 0.010f, 0.016f, 1.00f);
        colors[ImGuiCol_MenuBarBg] = ImVec4(0.008f, 0.010f, 0.013f, 1.00f);

        // General styling
        style.WindowRounding = 4.0f;
        style.ChildRounding = 3.0f;
        style.FrameRounding = 3.0f;
        style.PopupRounding = 4.0f;
        style.ScrollbarRounding = 4.0f;
        style.GrabRounding = 3.0f;
        style.TabRounding = 3.0f;

        style.WindowBorderSize = 1.0f;
        style.ChildBorderSize = 1.0f;
        style.PopupBorderSize = 1.0f;
        style.FrameBorderSize = 0.0f;
        style.TabBorderSize = 0.0f;

        style.WindowPadding = ImVec2(8.0f, 8.0f);
        style.FramePadding = ImVec2(6.0f, 4.0f);
        style.CellPadding = ImVec2(5.0f, 4.0f);
        style.ItemSpacing = ImVec2(8.0f, 5.0f);
        style.ItemInnerSpacing = ImVec2(5.0f, 4.0f);
        style.IndentSpacing = 18.0f;
        style.ScrollbarSize = 12.0f;
        style.GrabMinSize = 10.0f;
    }
}
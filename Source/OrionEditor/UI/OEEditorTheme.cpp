#include "OEEditorTheme.h"

#include <OrionEngine/Core/OEAssert.h>

namespace OrionEngine::OrionEditor
{
    ImFont* OEEditorTheme::RegularFont = nullptr;
    ImFont* OEEditorTheme::MediumFont = nullptr;
    ImFont* OEEditorTheme::SemiBoldFont = nullptr;
    ImFont* OEEditorTheme::ItalicFont = nullptr;

    Scope<EditorSpecificRendering::OEEditorIconRenderer> OEEditorTheme::m_EditorIconRenderer = CreateScope<EditorSpecificRendering::OEEditorIconRenderer>();

    void OEEditorTheme::ApplyTheme()
    {
        ImGui::StyleColorsDark();

        ImGuiStyle& Style = ImGui::GetStyle();
        ImVec4* Colors = Style.Colors;

        // ============================================================
        // UNREAL-INSPIRED DARK PALETTE
        // ============================================================

        const ImVec4 WindowBackground = ImVec4(0.075f, 0.075f, 0.075f, 1.00f);
        const ImVec4 PanelBackground = ImVec4(0.090f, 0.090f, 0.090f, 1.00f);
        const ImVec4 ChildBackground = ImVec4(0.105f, 0.105f, 0.105f, 1.00f);
        const ImVec4 PopupBackground = ImVec4(0.125f, 0.125f, 0.125f, 1.00f);

        const ImVec4 ToolbarBackground = ImVec4(0.115f, 0.115f, 0.115f, 1.00f);
        const ImVec4 SurfaceBackground = ImVec4(0.145f, 0.145f, 0.145f, 1.00f);
        const ImVec4 ElevatedBackground = ImVec4(0.190f, 0.190f, 0.190f, 1.00f);

        const ImVec4 Border = ImVec4(0.025f, 0.025f, 0.025f, 1.00f);
        const ImVec4 BorderSubtle = ImVec4(0.160f, 0.160f, 0.160f, 1.00f);

        const ImVec4 Accent = ImVec4(0.290f, 0.420f, 0.550f, 1.00f);
        const ImVec4 AccentHover = ImVec4(0.350f, 0.490f, 0.630f, 1.00f);
        const ImVec4 AccentActive = ImVec4(0.235f, 0.355f, 0.475f, 1.00f);
        const ImVec4 Selection = ImVec4(0.180f, 0.255f, 0.330f, 1.00f);

        const ImVec4 TextPrimary = ImVec4(0.850f, 0.850f, 0.850f, 1.00f);
        const ImVec4 TextSecondary = ImVec4(0.690f, 0.690f, 0.690f, 1.00f);
        const ImVec4 TextMuted = ImVec4(0.500f, 0.500f, 0.500f, 1.00f);
        const ImVec4 TextDisabled = ImVec4(0.380f, 0.380f, 0.380f, 1.00f);

        const ImVec4 Transparent = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);

        // ============================================================
        // TEXT
        // ============================================================

        Colors[ImGuiCol_Text] = TextPrimary;
        Colors[ImGuiCol_TextDisabled] = TextDisabled;

        // ============================================================
        // WINDOWS AND PANELS
        // ============================================================

        Colors[ImGuiCol_WindowBg] = PanelBackground;
        Colors[ImGuiCol_ChildBg] = ChildBackground;
        Colors[ImGuiCol_PopupBg] = PopupBackground;

        Colors[ImGuiCol_Border] = Border;
        Colors[ImGuiCol_BorderShadow] = Transparent;

        // ============================================================
        // FRAMES AND INPUT FIELDS
        // ============================================================

        Colors[ImGuiCol_FrameBg] = SurfaceBackground;
        Colors[ImGuiCol_FrameBgHovered] = ElevatedBackground;
        Colors[ImGuiCol_FrameBgActive] = Selection;

        // ============================================================
        // TITLE BARS
        // ============================================================

        Colors[ImGuiCol_TitleBg] = WindowBackground;
        Colors[ImGuiCol_TitleBgActive] = ToolbarBackground;
        Colors[ImGuiCol_TitleBgCollapsed] = WindowBackground;

        // ============================================================
        // MENUS
        // ============================================================

        Colors[ImGuiCol_MenuBarBg] = ToolbarBackground;

        // ============================================================
        // SCROLLBARS
        // ============================================================

        Colors[ImGuiCol_ScrollbarBg] = WindowBackground;
        Colors[ImGuiCol_ScrollbarGrab] = BorderSubtle;
        Colors[ImGuiCol_ScrollbarGrabHovered] = ElevatedBackground;
        Colors[ImGuiCol_ScrollbarGrabActive] = Accent;

        // ============================================================
        // CHECKBOXES AND SLIDERS
        // ============================================================

        Colors[ImGuiCol_CheckMark] = AccentHover;
        Colors[ImGuiCol_SliderGrab] = Accent;
        Colors[ImGuiCol_SliderGrabActive] = AccentHover;

        // ============================================================
        // BUTTONS
        // ============================================================

        Colors[ImGuiCol_Button] = SurfaceBackground;
        Colors[ImGuiCol_ButtonHovered] = ElevatedBackground;
        Colors[ImGuiCol_ButtonActive] = AccentActive;

        // ============================================================
        // COLLAPSING HEADERS
        // ============================================================

        Colors[ImGuiCol_Header] = ImVec4(0.110f, 0.110f, 0.110f, 1.00f);
        Colors[ImGuiCol_HeaderHovered] = ImVec4(0.165f, 0.165f, 0.165f, 1.00f);
        Colors[ImGuiCol_HeaderActive] = ImVec4(0.200f, 0.200f, 0.200f, 1.00f);

        Colors[ImGuiCol_Separator] = BorderSubtle;
        Colors[ImGuiCol_SeparatorHovered] = Accent;
        Colors[ImGuiCol_SeparatorActive] = AccentHover;

        // ============================================================
        // RESIZE GRIPS
        // ============================================================

        Colors[ImGuiCol_ResizeGrip] = Transparent;
        Colors[ImGuiCol_ResizeGripHovered] = Accent;
        Colors[ImGuiCol_ResizeGripActive] = AccentHover;

        // ============================================================
        // TABS
        // ============================================================

        Colors[ImGuiCol_Tab] = WindowBackground;
        Colors[ImGuiCol_TabHovered] = ElevatedBackground;
        Colors[ImGuiCol_TabSelected] = PanelBackground;
        Colors[ImGuiCol_TabDimmed] = WindowBackground;
        Colors[ImGuiCol_TabDimmedSelected] = ChildBackground;

        // ============================================================
        // DOCKING
        // ============================================================

        Colors[ImGuiCol_DockingPreview] = ImVec4(
            Accent.x, Accent.y, Accent.z, 0.45f
        );

        Colors[ImGuiCol_DockingEmptyBg] = WindowBackground;

        // ============================================================
        // TABLES
        // ============================================================

        Colors[ImGuiCol_TableHeaderBg] = ToolbarBackground;
        Colors[ImGuiCol_TableBorderStrong] = BorderSubtle;
        Colors[ImGuiCol_TableBorderLight] = Border;
        Colors[ImGuiCol_TableRowBg] = Transparent;
        Colors[ImGuiCol_TableRowBgAlt] = ImVec4(0.150f, 0.150f, 0.150f, 0.30f);

        // ============================================================
        // NAVIGATION
        // ============================================================

        Colors[ImGuiCol_NavCursor] = Accent;
        Colors[ImGuiCol_NavWindowingHighlight] = TextPrimary;
        Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.60f);

        // ============================================================
        // DRAG AND DROP
        // ============================================================

        Colors[ImGuiCol_DragDropTarget] = AccentHover;

        // ============================================================
        // MODAL WINDOWS
        // ============================================================

        Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.70f);

        // ============================================================
        // SPACING
        // ============================================================

        Style.WindowPadding = ImVec2(8.0f, 8.0f);
        Style.FramePadding = ImVec2(6.0f, 4.0f);
        Style.CellPadding = ImVec2(5.0f, 4.0f);
        Style.ItemSpacing = ImVec2(8.0f, 5.0f);
        Style.ItemInnerSpacing = ImVec2(6.0f, 4.0f);

        Style.IndentSpacing = 18.0f;
        Style.ScrollbarSize = 12.0f;
        Style.GrabMinSize = 10.0f;

        // ============================================================
        // ROUNDING
        // ============================================================

        Style.WindowRounding = 2.0f;
        Style.ChildRounding = 2.0f;
        Style.FrameRounding = 2.0f;
        Style.PopupRounding = 2.0f;
        Style.ScrollbarRounding = 2.0f;
        Style.GrabRounding = 2.0f;
        Style.TabRounding = 2.0f;

        // ============================================================
        // BORDERS
        // ============================================================

        Style.WindowBorderSize = 1.0f;
        Style.ChildBorderSize = 1.0f;
        Style.PopupBorderSize = 1.0f;
        Style.FrameBorderSize = 0.0f;
        Style.TabBorderSize = 0.0f;

        // ============================================================
        // ALIGNMENT
        // ============================================================

        Style.WindowTitleAlign = ImVec2(0.0f, 0.5f);
        Style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
        Style.SelectableTextAlign = ImVec2(0.0f, 0.5f);

        // ============================================================
        // RENDERING
        // ============================================================

        Style.AntiAliasedLines = true;
        Style.AntiAliasedFill = true;
    }

    void OEEditorTheme::LoadFonts()
    {
        ImGuiIO& io = ImGui::GetIO();

        RegularFont = io.Fonts->AddFontFromFileTTF("Assets/Fonts/Inter/static/Inter_18pt-Regular.ttf", 16.0f);
        MediumFont = io.Fonts->AddFontFromFileTTF("Assets/Fonts/Inter/static/Inter_18pt-Medium.ttf", 16.0f);
        SemiBoldFont = io.Fonts->AddFontFromFileTTF("Assets/Fonts/Inter/static/Inter_18pt-SemiBold.ttf", 16.0f);
        ItalicFont = io.Fonts->AddFontFromFileTTF("Assets/Fonts/Inter/static/Inter_18pt-Italic.ttf", 16.0f);

        OE_CORE_ASSERT(RegularFont != nullptr, "Failed to load Inter Regular font!");
        OE_CORE_ASSERT(MediumFont != nullptr, "Failed to load Inter Medium font!");
        OE_CORE_ASSERT(SemiBoldFont != nullptr, "Failed to load Inter SemiBold font!");
        OE_CORE_ASSERT(ItalicFont != nullptr, "Failed to load Inter Italic font!");

        io.FontDefault = RegularFont;
    }

    void OEEditorTheme::LoadEditorIcon(const std::string& iconName)
    {
        m_EditorIconRenderer->LoadEditorIcon(iconName);
    }

    unsigned int OEEditorTheme::GetEditorIcon(const std::string& iconName)
    {
        return m_EditorIconRenderer->GetIcon(iconName);
    }
}
#include "OEConsolePanel.h"

#include <OrionEngine/Core/OELogging.h>
#include <OrionEditor/UI/OEEditorTheme.h>

#include <imgui.h>

namespace OrionEngine::OrionEditor
{
	// ----------------------------
	// PRIVATE HELPERS
	// ---------------------------

	static const char* GetLogLevelString(OELogging::OELogLevel level) noexcept
	{
		switch (level)
		{
			case OELogging::OELogLevel::Debug:   return "DEBUG";
			case OELogging::OELogLevel::Info:    return "INFO";
			case OELogging::OELogLevel::Warning: return "WARNING";
			case OELogging::OELogLevel::Error:   return "ERROR";
			case OELogging::OELogLevel::Fatal:   return "FATAL";
			default:                             return "UNKNOWN";
		}
	}

	bool OEConsolePanel::DrawSearchBar()
	{
		OEEditorTheme::LoadEditorIcon("Search");

		ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 6.0f);
		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.069, 0.061, 0.225, 1.000f));
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.200f, 0.200f, 0.200f, 1.000f));

		if (ImGui::BeginChild("SearchBar", ImVec2(0.0f, 50.0f)))
		{
			const unsigned int searchIcon = OEEditorTheme::GetEditorIcon("Search");

			if (searchIcon)
				ImGui::Image((ImTextureID)(intptr_t)searchIcon, ImVec2(22.0f, 22.0f));

			ImGui::PushFont(OEEditorTheme::SemiBoldFont);
			ImGui::Text("Search log...");
			ImGui::PopFont();
			ImGui::SameLine();
			
			if (ImGui::InputText("##SearchLog", m_OECPDN.SearchBuffer, sizeof(m_OECPDN.SearchBuffer)))
			{
				m_OECPDN.CurrentlySelectedLog = {};

				if (m_OECPDN.SearchBuffer[0] != '\0')
				{
					for (const auto& log : OELogging::GetLogHistory())
					{
						if (log.Message.find(m_OECPDN.SearchBuffer) != std::string::npos)
						{
							m_OECPDN.CurrentlySelectedLog = log;
							break;
						}
					}
				}
			}
		}

		ImGui::EndChild();
		ImGui::PopStyleColor(2);
		ImGui::PopStyleVar();

		return true;
	}

	bool OEConsolePanel::ShowOutputLog()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 4.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1.25f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(9.0f, 9.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarSize, 4.5f);
		ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarRounding, 5.0f);

		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.021f, 0.019f, 0.049f, 1.000f));

		const std::string logQuery = m_OECPDN.SearchBuffer;
		
		if (ImGui::BeginChild("OutputLog", ImVec2(0.0f, 0.0f)))
		{
			for (const auto& log : OELogging::GetLogHistory())
			{
				if (!logQuery.empty() && log.Message.find(logQuery) == std::string::npos)
					continue;

				bool isLogSelected = m_OECPDN.CurrentlySelectedLog.LogIdentifierID == log.LogIdentifierID && log.LogIdentifierID != INVALID_LOG_IDENTIFIER_ID;

				ImVec4 logColor;

				switch (log.LogLevel)
				{
					case OELogging::OELogLevel::Debug:
						logColor = ImVec4(0.700f, 0.700f, 0.700f, 1.000f);
						break;

					case OELogging::OELogLevel::Info:
						logColor = ImVec4(1.000f, 1.000f, 1.000f, 1.000f);
						break;

					case OELogging::OELogLevel::Warning:
						logColor = ImVec4(1.000f, 1.000f, 0.000f, 1.000f);
						break;

					case OELogging::OELogLevel::Error:
						logColor = ImVec4(1.000f, 0.000f, 0.000f, 1.000f);
						break;

					case OELogging::OELogLevel::Fatal:
						logColor = ImVec4(0.500f, 0.000f, 1.000f, 1.000f);
						break;

					default:
						logColor = ImVec4(1.000f, 1.000f, 1.000f, 1.000f);
						break;
				}

				std::string logLabel = "[" + std::string(GetLogLevelString(log.LogLevel)) + "] " + log.Message;

				ImGui::PushStyleColor(ImGuiCol_Text, logColor);

				ImGui::PushID(std::to_string(log.LogIdentifierID).c_str());
				if (ImGui::Selectable(logLabel.c_str(), isLogSelected))
					m_OECPDN.CurrentlySelectedLog = log;
				ImGui::PopID();
				ImGui::PopStyleColor();
			}
		}

		ImGui::EndChild();
		ImGui::PopStyleVar(5);
		ImGui::PopStyleColor();
		
		return true;
	}

	bool OEConsolePanel::DrawSelectedLogView()
	{
		OEEditorTheme::LoadEditorIcon("Info");

		ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 4.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1.25f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(9.0f, 9.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarSize, 4.5f);
		ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarRounding, 5.0f);

		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.029f, 0.019f, 0.095f, 1.000f));

		if (ImGui::BeginChild("SelectedLogView", ImVec2(0.0f, 0.0f)))
		{
			if (m_OECPDN.CurrentlySelectedLog.LogIdentifierID != INVALID_LOG_IDENTIFIER_ID)
			{
				if (ImGui::BeginTable("LogViewLayout", 2, ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_BordersInnerV))
				{
					ImGui::TableSetupColumn("Property", ImGuiTableColumnFlags_WidthStretch, 0.35f);
					ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch, 0.65f);

					const unsigned int infoIcon = OEEditorTheme::GetEditorIcon("Info");

					if (infoIcon)
						ImGui::Image((ImTextureID)(intptr_t)infoIcon, ImVec2(25.0f, 25.0f));

					ImGui::SameLine();
					ImGui::Text("Log Info");
					ImGui::Spacing();
					ImGui::Separator();

					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ImGui::PushFont(OEEditorTheme::SemiBoldFont);
					ImGui::Text("Log Message:");
					ImGui::PopFont();
					ImGui::TableSetColumnIndex(1);
					ImGui::Text("%s", m_OECPDN.CurrentlySelectedLog.Message.c_str());

					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ImGui::PushFont(OEEditorTheme::SemiBoldFont);
					ImGui::Text("Log ID:");
					ImGui::PopFont();
					ImGui::TableSetColumnIndex(1);
					ImGui::Text("%llu", static_cast<unsigned long long>(m_OECPDN.CurrentlySelectedLog.LogIdentifierID));

					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ImGui::PushFont(OEEditorTheme::SemiBoldFont);
					ImGui::Text("Log Severity:");
					ImGui::PopFont();
					ImGui::TableSetColumnIndex(1);
					ImGui::Text("%s", GetLogLevelString(m_OECPDN.CurrentlySelectedLog.LogLevel));

					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ImGui::PushFont(OEEditorTheme::SemiBoldFont);
					ImGui::Text("File:");
					ImGui::PopFont();
					ImGui::TableSetColumnIndex(1);
					ImGui::Text("%s", m_OECPDN.CurrentlySelectedLog.File.c_str());

					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ImGui::PushFont(OEEditorTheme::SemiBoldFont);
					ImGui::Text("Line:");
					ImGui::PopFont();
					ImGui::TableSetColumnIndex(1);
					ImGui::Text("%d", m_OECPDN.CurrentlySelectedLog.Line);

					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ImGui::PushFont(OEEditorTheme::SemiBoldFont);
					ImGui::Text("Function:");
					ImGui::PopFont();
					ImGui::TableSetColumnIndex(1);
					ImGui::Text("%s", m_OECPDN.CurrentlySelectedLog.Function.c_str());

					ImGui::EndTable();
				}
			}
			else
				ImGui::Text("No log selected yet");
		}

		ImGui::EndChild();
		ImGui::PopStyleVar(5);
		ImGui::PopStyleColor();

		return true;
	}

	bool OEConsolePanel::DrawConsolePanel()
	{
		if (ImGui::Begin("Console"))
		{
			DrawSearchBar();

			if (ImGui::BeginTable("ConsoleLayout", 2, ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_Resizable))
			{
				ImGui::TableSetupColumn("LogOutput", ImGuiTableColumnFlags_WidthStretch, 6.5f);
				ImGui::TableSetupColumn("SelectedLogView", ImGuiTableColumnFlags_WidthStretch, 3.5f);

				ImGui::TableNextRow(ImGuiTableRowFlags_None, ImGui::GetContentRegionAvail().y);

				ImGui::TableSetColumnIndex(0);
				ShowOutputLog();

				ImGui::TableSetColumnIndex(1);
				DrawSelectedLogView();

				ImGui::EndTable();
			}
		}

		ImGui::End();
		return true;
	}
}
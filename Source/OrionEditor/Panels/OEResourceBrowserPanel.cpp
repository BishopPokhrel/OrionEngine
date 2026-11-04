#include "OEResourceBrowserPanel.h"

#include <imgui.h>

#include <OrionEngine/Core/OELogging.h>
#include <OrionEngine/Core/Scene/OESceneSerializer.h>

#include <OrionEditor/UI/OEEditorTheme.h>

namespace OrionEngine::OrionEditor
{
	bool OEResourceBrowserPanel::DrawProjectHierarchy()
	{
		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.025f, 0.019f, 0.175f, 1.000f));
		ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.5f);
		ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarRounding, 4.5f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding , ImVec2(10.0f, 8.0f));

		ImGui::BeginChild("ProjectViewer", ImVec2(220.0f, 0.0f));

		// Show the project in a hierarchy
		const std::string projectName = m_ProjectPath.stem().string();

		const std::filesystem::path projectDirectoryPath = m_ProjectPath.parent_path();
		const std::filesystem::path assetsDirectoryPath = projectDirectoryPath / "Assets";
		const std::filesystem::path scriptsDirectoryPath = projectDirectoryPath / "Scripts";

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
		
		if (m_ProjectPath.empty() || !std::filesystem::exists(m_ProjectPath))
		{
			ImGui::PushFont(OEEditorTheme::ItalicFont);
			ImGui::Text("No project created yet.");
			ImGui::PopFont();
		}
		else 
		{
			if (ImGui::TreeNodeEx(projectName.c_str(), flags))
			{
				// Assets
				if (std::filesystem::exists(assetsDirectoryPath))
				{
					ImGuiTreeNodeFlags assetsFlags = flags;

					if (m_CurrentDirectory == assetsDirectoryPath)
						assetsFlags |= ImGuiTreeNodeFlags_Selected;

					const bool expanded = ImGui::TreeNodeEx("Assets", assetsFlags);
					const bool clicked = ImGui::IsItemClicked();

					if (clicked)
					{
						m_CurrentDirectory = assetsDirectoryPath;
						UpdateDirectoryPath();
						RefreshDirectoryEntries();
					}

					if (expanded)
					{
						// Draw nested directories here if needed
						ImGui::TreePop();
					}
				}

				// Scripts
				if (std::filesystem::exists(scriptsDirectoryPath))
				{
					ImGuiTreeNodeFlags scriptsFlags = flags;

					if (m_CurrentDirectory == scriptsDirectoryPath)
						scriptsFlags |= ImGuiTreeNodeFlags_Selected;

					const bool expanded = ImGui::TreeNodeEx("Scripts", scriptsFlags);
					const bool clicked = ImGui::IsItemClicked();

					if (clicked)
					{
						m_CurrentDirectory = scriptsDirectoryPath;
						UpdateDirectoryPath();
						RefreshDirectoryEntries();
					}

					if (expanded)
					{
						// Draw nested directories here if needed
						ImGui::TreePop();
					}
				}

				ImGui::TreePop();
			}
		}

		ImGui::EndChild();
		ImGui::PopStyleColor();
		ImGui::PopStyleVar(4);
		return true;
	}

	bool OEResourceBrowserPanel::DrawResourceBrowserPanel()
	{
		ImGui::Begin("Resource Browser");

		if (ImGui::BeginTable("ResourceBrowserLayout", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_SizingStretchProp))
		{
			ImGui::TableSetupColumn("Hierarchy", ImGuiTableColumnFlags_WidthFixed, 220.0f);
			ImGui::TableSetupColumn("Resource", ImGuiTableColumnFlags_WidthStretch);

			ImGui::TableNextRow();

			ImGui::TableSetColumnIndex(0);
			DrawProjectHierarchy();

			ImGui::TableSetColumnIndex(1);

			DrawDirectoryToolbar();
			DrawDirectoryContents();

			ImGui::EndTable();
		}

		ImGui::End();

		return true;
	}

	void OEResourceBrowserPanel::SetProjectPath(const std::filesystem::path& path)
	{
		m_ProjectPath = path;
		m_CurrentDirectory = m_ProjectPath.parent_path() / "Assets";

		UpdateDirectoryPath();
		RefreshDirectoryEntries();
	}

	// -------------------------------------------
	// PRIVATE HELPERS
	// -------------------------------------------
	bool OEResourceBrowserPanel::RefreshDirectoryEntries()
	{
		m_DirectoryEntries.clear();

		try
		{
			if (!std::filesystem::exists(m_CurrentDirectory))
			{
				ORION_ENGINE_ERROR("Directory does not exist: ", m_CurrentDirectory.string());
				return false;
			}

			if (!std::filesystem::is_directory(m_CurrentDirectory))
			{
				ORION_ENGINE_ERROR("Current path is not a directory!");
				return false;
			}


			for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(m_CurrentDirectory))
				m_DirectoryEntries.push_back(entry);
		}
		catch (const std::filesystem::filesystem_error& exception)
		{
			ORION_ENGINE_ERROR("Failed to refresh directory: ", exception.what());
			return false;
		}

		return true;
	}

	void OEResourceBrowserPanel::UpdateDirectoryPath()
	{
		const std::string currentDirectory = m_CurrentDirectory.string();
		strncpy_s(m_OERBPDTDPDN.CurrentDirectoryPathBuffer, sizeof(m_OERBPDTDPDN.CurrentDirectoryPathBuffer), currentDirectory.c_str(), _TRUNCATE);
	}

	bool OEResourceBrowserPanel::DrawDirectoryToolbar()
	{
		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.095f, 0.059f, 0.236f, 1.000f));
		ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.9f);
		ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarRounding, 4.5f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5.0f, 5.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4.5f, 4.5f));
		ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.5f);

		ImGui::BeginChild("DirectoryToolbar", ImVec2(0.0f, 45.0f));
				
		ImGui::PushFont(OEEditorTheme::SemiBoldFont);
		ImGui::Text("Current Directory");
		ImGui::PopFont();
		ImGui::SameLine();
		
		if (ImGui::InputText("##DirectoryPath", m_OERBPDTDPDN.CurrentDirectoryPathBuffer, sizeof(m_OERBPDTDPDN.CurrentDirectoryPathBuffer), ImGuiInputTextFlags_EnterReturnsTrue))
		{
			if (std::filesystem::exists(std::filesystem::path(m_OERBPDTDPDN.CurrentDirectoryPathBuffer)))
			{
				const std::filesystem::path requestedDirectory(m_OERBPDTDPDN.CurrentDirectoryPathBuffer);
				std::error_code errorCode;

				if (std::filesystem::is_directory(requestedDirectory, errorCode) && !errorCode)
				{
					m_CurrentDirectory = requestedDirectory;

					UpdateDirectoryPath();
					RefreshDirectoryEntries();
				}
				else
					UpdateDirectoryPath();
			}
		}
		
		ImGui::EndChild();
		ImGui::PopStyleColor();
		ImGui::PopStyleVar(7);

		return true;
	}

	bool OEResourceBrowserPanel::DrawDirectoryContents()
	{
		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.015f, 0.012f, 0.095f, 1.000f));
		ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 4.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 2.0f);
		
		ImGui::BeginChild("ResourceViewer", ImVec2(0.0f, 0.0f));

		OEEditorTheme::LoadEditorIcon("Folder");
		OEEditorTheme::LoadEditorIcon("ScriptFile");

		const unsigned int folderIcon = OEEditorTheme::GetEditorIcon("Folder");
		const unsigned int scriptFileIcon = OEEditorTheme::GetEditorIcon("ScriptFile");

		std::error_code errorCode;
		std::filesystem::path selectedDirectoryPath;

		if (std::filesystem::is_directory(m_CurrentDirectory, errorCode))
		{
			if (!errorCode)
			{
				if (ImGui::BeginTable("ResourceLayoutGrid", 4, ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_BordersInnerV))
				{
					for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(m_CurrentDirectory, errorCode))
					{
						ImGui::TableNextColumn();

						// For folder 
						if (entry.is_directory())
						{
							if (folderIcon != 0)
								ImGui::Image((ImTextureID)(intptr_t)folderIcon, ImVec2(35.0f, 35.0f));

							if (ImGui::Selectable(entry.path().filename().string().c_str(), m_OERBPDCDN.IsFolderSelected))
								m_OERBPDCDN.IsFolderSelected = true;

							if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
								selectedDirectoryPath = entry.path();
						}

						// For file 
						if (entry.is_regular_file())
						{
							if (scriptFileIcon != 0)
								ImGui::Image((ImTextureID)(intptr_t)scriptFileIcon, ImVec2(35.0f, 35.0f));

							if (ImGui::Selectable(entry.path().filename().string().c_str(), m_OERBPDCDN.IsFileSelected))
								m_OERBPDCDN.IsFolderSelected = true;
						}
					}

					ImGui::EndTable();
				}
			}

			if (!selectedDirectoryPath.empty())
			{
				m_CurrentDirectory = selectedDirectoryPath;

				UpdateDirectoryPath();
				RefreshDirectoryEntries();
			}
		}

		ImGui::EndChild();
		ImGui::PopStyleColor();
		ImGui::PopStyleVar(2);

		return true;
	}

	bool OEResourceBrowserPanel::HandleFileDrop(const std::filesystem::path& filePath)
	{
		return true;
	}

	bool OEResourceBrowserPanel::ImportAsset(const std::filesystem::path& filePath)
	{
		return true;
	}
}
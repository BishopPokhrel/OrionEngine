#include "OEEnvironmentDetailsPanel.h"

#include <imgui.h>

#if defined(_WIN64)
#include <OrionEngine/OS/Windows/WindowsOSAPI.h>
#endif

#include <bit>

namespace OrionEngine::OrionEditor
{
	// PRIVATE FUNCTIONS

	bool OEEnvironmentDetailsPanel::DrawEnvironmentLighting()
	{
		OEEnvironment* currentEnvironment = &m_Scene->GetEnvironment();

		strncpy_s(m_EDPDataNeeded.EnvironmentHDRTexturePathBuffer, sizeof(m_EDPDataNeeded.EnvironmentHDRTexturePathBuffer), m_Scene->GetEnvironment().GetEnvironmentHDRTexturePath().string().c_str(), _TRUNCATE);

		ImGui::Text("Current Environment Path");
		ImGui::SameLine();
		ImGui::InputText("##EnvironmentPath", m_EDPDataNeeded.EnvironmentHDRTexturePathBuffer, sizeof(m_EDPDataNeeded.EnvironmentHDRTexturePathBuffer), ImGuiInputTextFlags_ReadOnly);

		if (ImGui::Button("Browse Environment Path"))
		{
			const std::filesystem::path environmentPath = OS::OpenWindowsFileDialog(L"HDR Files (*.hdr)\0*.hdr\0");
			if (!environmentPath.empty())
			{
				currentEnvironment->SetEnvironmentHDRTexturePath(environmentPath);
				strncpy_s(m_EDPDataNeeded.EnvironmentHDRTexturePathBuffer, sizeof(m_EDPDataNeeded.EnvironmentHDRTexturePathBuffer), environmentPath.string().c_str(), _TRUNCATE);
			}
		}

		return true;
	}

	bool OEEnvironmentDetailsPanel::DrawIBLData()
	{
		OEEnvironment* currentEnvironment = &m_Scene->GetEnvironment();

		// Show the IBL Settings only
		if (ImGui::CollapsingHeader("Environment Settings"))
		{
			const char* environmentResOptions[] = {
				"32", "64", "128", "256", "512", "1024"
			};

			ImGui::Text("Environment Resolution");
			ImGui::SameLine();

			if (ImGui::BeginCombo("##EnvironmentResolution", environmentResOptions[m_EDPDataNeeded.EnvironmentResolutionIndex]))
			{
				for (int n = 0; n < IM_ARRAYSIZE(environmentResOptions); n++)
				{
					bool b_IsEnvironmentResolutionSelected = (m_EDPDataNeeded.EnvironmentResolutionIndex == n);
					if (ImGui::Selectable(environmentResOptions[n], b_IsEnvironmentResolutionSelected))
						m_EDPDataNeeded.EnvironmentResolutionIndex = n;

					if (b_IsEnvironmentResolutionSelected)
						ImGui::SetItemDefaultFocus();
				}

				switch (m_EDPDataNeeded.EnvironmentResolutionIndex)
				{
					case 0:
						currentEnvironment->GetEnvironmentIBLSettings().SetEnvironmentResolution(32);
						break;
					case 1:
						currentEnvironment->GetEnvironmentIBLSettings().SetEnvironmentResolution(64);
						break;
					case 2:
						currentEnvironment->GetEnvironmentIBLSettings().SetEnvironmentResolution(128);
						break;
					case 3:
						currentEnvironment->GetEnvironmentIBLSettings().SetEnvironmentResolution(256);
						break;
					case 4:
						currentEnvironment->GetEnvironmentIBLSettings().SetEnvironmentResolution(512);
						break;
					case 5:
						currentEnvironment->GetEnvironmentIBLSettings().SetEnvironmentResolution(1024);
						break;

				}

				ImGui::EndCombo();
			}
		}

		if (ImGui::CollapsingHeader("Irradiance Settings"))
		{
			const char* irradianceResOptions[] = {
				"32", "64", "128", "256", "512", "1024"
			};

			ImGui::Text("Irradiance Resolution");
			ImGui::SameLine();

			if (ImGui::BeginCombo("##IrradianceResolution", irradianceResOptions[m_EDPDataNeeded.IrradianceResolutionIndex]))
			{
				for (int n = 0; n < IM_ARRAYSIZE(irradianceResOptions); n++)
				{
					bool b_IsIrradianceResolutionSelected = (m_EDPDataNeeded.IrradianceResolutionIndex == n);
					if (ImGui::Selectable(irradianceResOptions[n], b_IsIrradianceResolutionSelected))
						m_EDPDataNeeded.IrradianceResolutionIndex = n;

					if (b_IsIrradianceResolutionSelected)
						ImGui::SetItemDefaultFocus();
				}

				switch (m_EDPDataNeeded.IrradianceResolutionIndex)
				{
				case 0:
					currentEnvironment->GetEnvironmentIBLSettings().SetIrradianceResolution(32);
					break;
				case 1:
					currentEnvironment->GetEnvironmentIBLSettings().SetIrradianceResolution(64);
					break;
				case 2:
					currentEnvironment->GetEnvironmentIBLSettings().SetIrradianceResolution(128);
					break;
				case 3:
					currentEnvironment->GetEnvironmentIBLSettings().SetIrradianceResolution(256);
					break;
				case 4:
					currentEnvironment->GetEnvironmentIBLSettings().SetIrradianceResolution(512);
					break;
				case 5:
					currentEnvironment->GetEnvironmentIBLSettings().SetIrradianceResolution(1024);
					break;

				}

				ImGui::EndCombo();
			}
		}

		if (ImGui::CollapsingHeader("Prefilter Settings"))
		{
			const char* prefilterResOptions[] = {
				"32", "64", "128", "256", "512", "1024"
			};

			ImGui::Text("Prefilter Resolution");
			ImGui::SameLine();

			if (ImGui::BeginCombo("##PrefilterResolution", prefilterResOptions[m_EDPDataNeeded.PrefilterResolutionIndex]))
			{
				for (int n = 0; n < IM_ARRAYSIZE(prefilterResOptions); n++)
				{
					bool b_IsPrefilterResolutionSelected = (m_EDPDataNeeded.PrefilterResolutionIndex == n);
					if (ImGui::Selectable(prefilterResOptions[n], b_IsPrefilterResolutionSelected))
						m_EDPDataNeeded.PrefilterResolutionIndex = n;

					if (b_IsPrefilterResolutionSelected)
						ImGui::SetItemDefaultFocus();
				}

				switch (m_EDPDataNeeded.PrefilterResolutionIndex)
				{
				case 0:
					currentEnvironment->GetEnvironmentIBLSettings().SetPrefilterResolution(32);
					break;
				case 1:
					currentEnvironment->GetEnvironmentIBLSettings().SetPrefilterResolution(64);
					break;
				case 2:
					currentEnvironment->GetEnvironmentIBLSettings().SetPrefilterResolution(128);
					break;
				case 3:
					currentEnvironment->GetEnvironmentIBLSettings().SetPrefilterResolution(256);
					break;
				case 4:
					currentEnvironment->GetEnvironmentIBLSettings().SetPrefilterResolution(512);
					break;
				case 5:
					currentEnvironment->GetEnvironmentIBLSettings().SetPrefilterResolution(1024);
					break;

				}

				ImGui::EndCombo();
			}
		}

		const int maxMipLevels = std::bit_width(currentEnvironment->GetEnvironmentIBLSettings().PrefilterResolution);

		int prefilterMipLevels = static_cast<int>(currentEnvironment->GetEnvironmentIBLSettings().PrefilterMipLevels);

		ImGui::Text("Prefilter Mip Levels");
		ImGui::SameLine();
		if (ImGui::SliderInt("##PrefilterMipLevels", &prefilterMipLevels, 1, maxMipLevels))
			currentEnvironment->GetEnvironmentIBLSettings().SetPrefilterMipLevels(static_cast<uint32_t>(prefilterMipLevels));
		return true;
	}

	// Public function
	bool OEEnvironmentDetailsPanel::DrawEnvironmentDetailsPanel()
	{
		if (ImGui::Begin("Environment Details"))
		{
			DrawEnvironmentLighting();

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();

			DrawIBLData();
		}

		ImGui::End();
		return true;
	}
}
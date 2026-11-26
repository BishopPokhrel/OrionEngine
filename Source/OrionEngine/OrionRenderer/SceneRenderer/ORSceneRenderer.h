#pragma once

#include "ORCamera.h"

// ECS includes
#include <OrionEngine/Core/ECS/OEComponent.h>
#include <OrionEngine/Core/ECS/OEGameEntity.h>
#include <OrionEngine/Core/ECS/OEECSRegistry.h>
#include <OrionEngine/Core/Scene/OEEnvironment.h>
#include <OrionEngine/Core/AssetManagement/OEAssetManager.h>

#include <OrionEngine/OrionRenderer/ORRenderer.h>
#include <OrionEngine/OrionRenderer/ORDefaultRendererResources.h>
#include <OrionEngine/OrionRenderer/ORShadowMap.h>
#include <OrionEngine/OrionRenderer/ORShader.h>

namespace OrionEngine
{
	namespace OrionRenderer
	{
		class ORSceneRenderer
		{
		public:

			explicit ORSceneRenderer(ORRenderer* renderer, AssetManagement::OEAssetManager* assetManager) 
				: m_Renderer(renderer), m_AssetManager(assetManager)
			{
				OE_CORE_ASSERT(m_Renderer, "ORSceneRenderer received a null renderer (ORRenderer)!");
				OE_CORE_ASSERT(m_AssetManager, "ORSceneRenderer received a null asset manager!");

				m_ShadowDepthShader = ORDefaultRendererResources::GetDefaultShadowDepthShader();
			}

			void RenderScene(ECS::OEECSRegistry& registry, const ORCamera& camera, const OEEnvironment& environment, const OEDirectionalLight& directionalLight, const glm::mat4& viewProjection);			void Render(ECS::OEECSRegistry& registry, ECS::OEGameEntityID entity);

			bool RenderSceneEnvironment(OEEnvironment& environment);

			void RenderShadowPass(ECS::OEECSRegistry& registry, const glm::mat4& lightSpaceMatrix);

		private:

			ORRenderer* m_Renderer = nullptr;
			AssetManagement::OEAssetManager* m_AssetManager = nullptr;

			Ref<ORShader> m_ShadowDepthShader;
		};
	}
}
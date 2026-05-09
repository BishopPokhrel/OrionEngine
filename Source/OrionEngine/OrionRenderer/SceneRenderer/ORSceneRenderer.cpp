#include "ORSceneRenderer.h"

#include <OrionEngine/OrionRenderer/ORRenderer.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace OrionEngine
{
	namespace OrionRenderer
	{
		void ORSceneRenderer::Render(ECS::OEECSRegistry& registry, ECS::OEGameEntityID entity)
		{
			auto* transform = registry.TransformComponent.GetComponent(entity);
			auto* physics = registry.PhysicsComponent.GetComponent(entity);
			auto* render = registry.RenderableComponent.GetComponent(entity);

			if (!transform || !render)
				return;

			// Build Model Matrix
			glm::mat4 model = glm::translate(glm::mat4(1.0f), transform->Position)
				* glm::mat4_cast(glm::quat(transform->Rotation))
				* glm::scale(glm::mat4(1.0f), transform->Scale);

			ORRenderer::Submit(render->Shader, render->VertexArray, model);
		}

		void ORSceneRenderer::RenderScene(ECS::OEECSRegistry& registry, const ORCamera& camera, const glm::mat4& viewProjection)
		{
			ORRenderer::BeginScene(viewProjection);
			auto& render = registry.RenderableComponent.GetAllComponents();

			for (auto& [id, comp] : render)
			{
				if (!comp.b_Visible)
					continue;
				
				Render(registry, id);
			}

			ORRenderer::EndScene();
		}
	}
}
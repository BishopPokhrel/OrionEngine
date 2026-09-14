#pragma once

#include <OrionEngine/OrionRenderer/ORRenderer.h>
#include <OrionEngine/OrionRenderer/SceneRenderer/ORSceneRenderer.h>
#include <OrionEngine/OrionRenderer/Platform/Window/GLFWWindow.h>
#include <OrionEngine/OrionRenderer/ORDefaultRendererResources.h>
#include <OrionEngine/OrionRenderer/ORFramebuffer.h>

#include <OrionEngine/Core/OEInputSystem.h>
#include <OrionEngine/Core/Scene/OEScene.h>
#include <OrionEngine/Core/OERef.h>

namespace OrionEngine
{
	class OEngine
	{
	public:

		OEngine()
			: m_Renderer(CreateScope<OrionRenderer::ORRenderer>()), m_InputSystem(CreateScope<OEInputSystem>()), m_Scene(CreateScope<OEScene>()),
			m_SceneRenderer(nullptr)
		{ 
		}

		void OInitEngineSubsystems();
		void OShutdownEngineSubsystems();
		void OEngineShutdown(); // This function shuts down the engine; OShutdownEngineSystems simply shuts down the engine subsystems
		void OTick(float deltaTime);

		// Renderer functions
		void OBeginFrame();
		void OEndFrame();

		void Render();

		OEInputSystem* GetInputSystem() const { return m_InputSystem.get(); }
		OEScene* GetScene() const { return m_Scene.get(); }
		Ref<OrionRenderer::ORFrameBuffer> GetFrameBuffer() const { return m_ViewportFramebuffer; }

	private:

		Scope<OrionRenderer::ORRenderer> m_Renderer; 
		Scope<OEInputSystem> m_InputSystem;
		Scope<OEScene> m_Scene; // there will only be one scene 
		Scope<OrionRenderer::ORSceneRenderer> m_SceneRenderer;
		Ref<OrionRenderer::ORFrameBuffer> m_ViewportFramebuffer;
	};
}
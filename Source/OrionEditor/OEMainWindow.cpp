#define GLFW_INCLUDE_NONE // Just in case

#include "OEMainWindow.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <OrionEngine/OrionRenderer/Platform/OpenGL/OpenGLVertexArray.h>
#include <OrionEngine/OrionRenderer/Platform/OpenGL/OpenGLIndexBuffer.h>
#include <OrionEngine/OrionRenderer/Platform/OpenGL/OpenGLVertexBuffer.h>
#include <OrionEngine/OrionRenderer/OROpenGLShaderAPI.h>
#include <OrionEngine/Core/OETime.h>
#include <OrionEngine/Core/OEAssert.h>
#include <OrionEngine/Core/OEGlobals.h>

namespace OrionEngine
{
    namespace OrionEditor
    {
        using namespace OrionRenderer;

        bool OEMainWindow::InitOEMainWindow()
        {
            m_Window = new GLFWWindow(WindowProps());

            ORRenderer::Init(ORGraphicsAPI::OpenGL);
            ORRenderer::SetViewport(0, 0, 1280, 800);
            ORRenderCommand::SetClearColor(0.0f, 0.0f, 0.0f, 1.0f);

            float zoom = 1.0f;
            float aspect = 1280.0f / 800.0f;

            float left = -aspect * zoom;
            float right = aspect * zoom;
            float bottom = -zoom;
            float top = zoom;

            g_InputSystem = CreateScope<OEInputSystem>();
            g_InputSystem->InitializeOEInputSystem(m_Window->GetNativeWindow());
            OE_CORE_ASSERT(g_InputSystem, "Input System is null!");

            m_Camera = CreateRef<ORCamera>(left, right, bottom, top);
            OE_CORE_ASSERT(m_Camera, "Camera creation failed!");

            m_CameraController = CreateRef<ORCameraController>(m_Camera.get(),g_InputSystem.get(),  2.5f);
            OE_CORE_ASSERT(m_CameraController, "CameraController creation failed!");

            // -----------------------
            // TRIANGLE DATA (POSITION + COLOR)
            // -----------------------
            float vertices[] =
            {
                // position           // color
                -0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f, // red
                 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f, // green
                 0.0f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f  // blue
            };

            uint32_t indices[] = { 0, 1, 2 };

            // -----------------------
            // VAO + BUFFERS
            // -----------------------
            m_VertexArray = CreateRef<OpenGLVertexArray>();

            auto vb = CreateRef<OpenGLVertexBuffer>(vertices, sizeof(vertices));
            auto ib = CreateRef<OpenGLIndexBuffer>(indices, 3);

            // IMPORTANT: 6 floats per vertex = position(3) + color(3)
            ORVertexBufferLayout layout;
            layout.Push<glm::vec3>("a_Position");
            layout.Push<glm::vec3>("a_Color");

            vb->SetLayout(layout);

            m_VertexArray->AddVertexBuffer(vb);
            m_VertexArray->SetIndexBuffer(ib);

            // -----------------------
            // SHADER
            // -----------------------
            std::string vs = R"(
                #version 330 core

                layout(location = 0) in vec3 a_Position;
                layout(location = 1) in vec3 a_Color;

                out vec3 v_Color;

                uniform mat4 u_ViewProjection;
                uniform mat4 u_Transform;

                void main()
                {
                    v_Color = a_Color;
                    gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
                }
            )";

            std::string fs = R"(
                #version 330 core

                in vec3 v_Color;
                out vec4 color;

                void main()
                {
                    color = vec4(v_Color, 1.0);
                }
            )";

            m_Shader = std::make_shared<OROpenGLShaderAPI>();
            m_Shader->CompileAndLinkThroughShaderVariables({ vs }, { fs });

            return true;
        }

        bool OEMainWindow::MainLoop()
        {
            while (!m_Window->ShouldClose())
            {
                float dt = OETime::OEGetDeltaTime();

                ORRenderCommand::Clear();

                OE_CORE_ASSERT(m_CameraController, "CameraController is null in MainLoop");
                m_CameraController->OnUpdate(dt);

                OE_CORE_ASSERT(m_Camera, "Camera is null in MainLoop");
                ORRenderer::BeginScene(m_Camera->GetViewProjection());
                ORRenderer::Submit(m_Shader, m_VertexArray, glm::mat4(1.0f));

                ORRenderer::EndScene();

                m_Window->OnUpdate();
            }

            return true;
        }

        bool OEMainWindow::DestoryOEMainWindow()
        {
            ORRenderer::EndScene();
            delete m_Window;
            m_Window = nullptr;
            return true;
        }
    }
}
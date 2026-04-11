#define GLFW_INCLUDE_NONE

#include "OEInputSystem.h"
#include <OrionEngine/Core/OEExceptions.h>

namespace Utils
{
	static int StringToGLFWKey(const std::string& key)
	{
		static const std::unordered_map<std::string, int> keyMap =
		{
			// Letters
			{"A", GLFW_KEY_A}, {"B", GLFW_KEY_B}, {"C", GLFW_KEY_C}, {"D", GLFW_KEY_D},
			{"E", GLFW_KEY_E}, {"F", GLFW_KEY_F}, {"G", GLFW_KEY_G}, {"H", GLFW_KEY_H},
			{"I", GLFW_KEY_I}, {"J", GLFW_KEY_J}, {"K", GLFW_KEY_K}, {"L", GLFW_KEY_L},
			{"M", GLFW_KEY_M}, {"N", GLFW_KEY_N}, {"O", GLFW_KEY_O}, {"P", GLFW_KEY_P},
			{"Q", GLFW_KEY_Q}, {"R", GLFW_KEY_R}, {"S", GLFW_KEY_S}, {"T", GLFW_KEY_T},
			{"U", GLFW_KEY_U}, {"V", GLFW_KEY_V}, {"W", GLFW_KEY_W}, {"X", GLFW_KEY_X},
			{"Y", GLFW_KEY_Y}, {"Z", GLFW_KEY_Z},

			// Numbers
			{"0", GLFW_KEY_0}, {"1", GLFW_KEY_1}, {"2", GLFW_KEY_2}, {"3", GLFW_KEY_3},
			{"4", GLFW_KEY_4}, {"5", GLFW_KEY_5}, {"6", GLFW_KEY_6}, {"7", GLFW_KEY_7},
			{"8", GLFW_KEY_8}, {"9", GLFW_KEY_9},

			// Arrows
			{"UP", GLFW_KEY_UP},
			{"DOWN", GLFW_KEY_DOWN},
			{"LEFT", GLFW_KEY_LEFT},
			{"RIGHT", GLFW_KEY_RIGHT},

			// Special
			{"SPACE", GLFW_KEY_SPACE},
			{"ESC", GLFW_KEY_ESCAPE},
			{"ENTER", GLFW_KEY_ENTER},
			{"TAB", GLFW_KEY_TAB},
			{"SHIFT", GLFW_KEY_LEFT_SHIFT},
			{"CTRL", GLFW_KEY_LEFT_CONTROL},
			{"ALT", GLFW_KEY_LEFT_ALT}
		};

		auto it = keyMap.find(key);
		if (it != keyMap.end())
			return it->second;

		return GLFW_KEY_UNKNOWN;
	}
}

namespace OrionEngine
{
	void OEInputSystem::InitializeOEInputSystem(GLFWwindow* window) noexcept
	{
		m_OEInputStates.clear();
		m_PreviousKeyDown.clear();
		m_PreviousMouseDown.clear();

		mouseX = mouseY = 0.0;
		deltaX = deltaY = 0.0;
		scrollDelta = 0;

		// Set user pointer once
		glfwSetWindowUserPointer(window, this);

		// Scroll callback (ONLY ONCE)
		glfwSetScrollCallback(window,
			[](GLFWwindow* window, double, double yOffset)
			{
				auto* self = static_cast<OEInputSystem*>(glfwGetWindowUserPointer(window));
				self->scrollDelta += static_cast<int>(yOffset);
			});
	}

	void OEInputSystem::Update(GLFWwindow* window) noexcept
	{
		glfwPollEvents();

		// --- KEY STATES ---
		for (auto& [key, state] : m_OEInputStates)
		{
			bool prev = m_PreviousKeyDown[key];
			int glfwKey = key.empty() ? GLFW_KEY_UNKNOWN : key[0]; // keep your string system minimal

			bool curr = (glfwGetKey(window, glfwKey) == GLFW_PRESS);
			m_PreviousKeyDown[key] = curr;

			if (!prev && curr) state = OEInputState::Pressed;
			else if (prev && curr) state = OEInputState::Held;
			else if (prev && !curr) state = OEInputState::Released;
			else state = OEInputState::None;
		}

		// --- MOUSE BUTTONS ---
		for (auto& [trigger, callbacks] : m_MouseCallbacks)
		{
			int button = trigger.key;
			OEInputState required = trigger.state;

			bool prev = m_PreviousMouseDown[button];
			bool curr = (glfwGetMouseButton(window, button) == GLFW_PRESS);
			m_PreviousMouseDown[button] = curr;

			OEInputState state = OEInputState::None;

			if (!prev && curr) state = OEInputState::Pressed;
			else if (prev && curr) state = OEInputState::Held;
			else if (prev && !curr) state = OEInputState::Released;

			if (state == required)
			{
				for (auto& cb : callbacks)
					cb();
			}
		}

		// --- MOUSE POSITION ---
		double cx, cy;
		glfwGetCursorPos(window, &cx, &cy);

		deltaX = cx - mouseX;
		deltaY = cy - mouseY;

		mouseX = cx;
		mouseY = cy;

		// --- KEY CALLBACKS ---
		for (auto& [trigger, callbacks] : m_KeyCallbacks)
		{
			auto it = m_OEInputStates.find(trigger.key ? std::string(1, (char)trigger.key) : "");
			if (it != m_OEInputStates.end() && it->second == trigger.state)
			{
				for (auto& cb : callbacks)
					cb();
			}
		}
	}

	// ---------------------------
	// PUBLIC API
	// ---------------------------

	bool OEInputSystem::IsKeyPressed(const std::string& key) noexcept
	{
		return m_OEInputStates[key] == OEInputState::Pressed;
	}

	bool OEInputSystem::IsKeyReleased(const std::string& key) noexcept
	{
		return m_OEInputStates[key] == OEInputState::Released;
	}

	bool OEInputSystem::IsKeyHeld(const std::string& key) noexcept
	{
		return m_OEInputStates[key] == OEInputState::Held;
	}

	std::pair<double, double> OEInputSystem::GetMousePosition() noexcept
	{
		return { mouseX, mouseY };
	}

	std::pair<double, double> OEInputSystem::GetMouseDelta() noexcept
	{
		return { deltaX, deltaY };
	}

	int OEInputSystem::GetScrollDelta() noexcept
	{
		return scrollDelta;
	}

	void OEInputSystem::RegisterKeyCallback(const std::string& key, std::function<void()> callback, OEInputState state) noexcept
	{
		OEInputKey k{ key.empty() ? 0 : key[0], state };
		m_KeyCallbacks[k].push_back(callback);
	}

	void OEInputSystem::RegisterMouseButtonCallback(int button, std::function<void()> callback, OEInputState state) noexcept
	{
		OEInputKey k{ button, state };
		m_MouseCallbacks[k].push_back(callback);
	}

	void OEInputSystem::ResetPerFrameStates() noexcept
	{
		for (auto& [key, state] : m_OEInputStates)
		{
			if (state == OEInputState::Pressed)
				state = OEInputState::Held;
			else if (state == OEInputState::Released)
				state = OEInputState::None;
		}
	}

	void OEInputSystem::RegisterKey(const std::string& key) noexcept
	{
		m_OEInputStates[key] = OEInputState::None;
		m_PreviousKeyDown[key] = false;
	}
}
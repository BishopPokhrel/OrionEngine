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
	void OEInputSystem::InitializeOEInputSystem() noexcept
	{
		for (auto& [key, value] : m_OEInputStates)
		{
			m_OEInputStates.clear();
			m_OEInputStates["Pressed"] = OEInputState::None;
			m_OEInputStates["Released"] = OEInputState::None;
			m_OEInputStates["Held"] = OEInputState::None;
		}
		
		mouseX = 0.0f;
		mouseY = 0.0f;
		deltaX = 0.0f;
		deltaY = 0.0f;
		scrollDelta = 0;
	}

	void OEInputSystem::Update(GLFWwindow* window) noexcept
	{
		glfwPollEvents();

		// --- Update all keys ---
		for (auto& [key, state] : m_OEInputStates)
		{
			int glfwKey = Utils::StringToGLFWKey(key);
			if (glfwKey == GLFW_KEY_UNKNOWN)
				continue;

			// Keyboard state tracking
			bool b_PreviousKeyDown = m_PreviousKeyDown[key];
			bool b_CurrentKeyDown = (glfwGetKey(window, glfwKey) == GLFW_PRESS);
			m_PreviousKeyDown[key] = b_CurrentKeyDown;

			// Update OEInputState based on previous/current key
			OEInputState nextInputState = OEInputState::None;
			if (!b_PreviousKeyDown && b_CurrentKeyDown) nextInputState = OEInputState::Pressed;
			else if (b_PreviousKeyDown && b_CurrentKeyDown) nextInputState = OEInputState::Held;
			else if (b_PreviousKeyDown && !b_CurrentKeyDown) nextInputState = OEInputState::Released;
			else nextInputState = OEInputState::None;

			m_OEInputStates[key] = nextInputState;
		}

		// --- Mouse buttons ---
		for (auto& [buttonTrigger, callbacks] : m_MouseCallbacks)
		{
			int buttonIndex = buttonTrigger.first;
			OEInputState triggerState = buttonTrigger.second;

			bool b_PreviousMouseDown = m_PreviousMouseDown[buttonIndex];
			bool b_CurrentMouseDown = (glfwGetMouseButton(window, buttonIndex) == GLFW_PRESS);
			m_PreviousMouseDown[buttonIndex] = b_CurrentMouseDown;

			OEInputState nextInputState = OEInputState::None;
			if (!b_PreviousMouseDown && b_CurrentMouseDown) nextInputState = OEInputState::Pressed;
			else if (b_PreviousMouseDown && b_CurrentMouseDown) nextInputState = OEInputState::Held;
			else if (b_PreviousMouseDown && !b_CurrentMouseDown) nextInputState = OEInputState::Released;
			else nextInputState = OEInputState::None;

			// Call callbacks if state matches trigger
			if (nextInputState == triggerState)
			{
				for (auto& cb : callbacks)
					cb();
			}
		}

		// --- Mouse position & delta ---
		double currentX, currentY;
		glfwGetCursorPos(window, &currentX, &currentY);

		deltaX = currentX - mouseX;
		deltaY = currentY - mouseY;

		mouseX = currentX;
		mouseY = currentY;

		// --- Call registered key callbacks ---
		for (auto& [keyTrigger, callbacks] : m_KeyCallbacks)
		{
			const std::string& key = keyTrigger.first;
			OEInputState triggerState = keyTrigger.second;

			auto it = m_OEInputStates.find(key);
			if (it != m_OEInputStates.end() && it->second == triggerState)
			{
				for (auto& callback : callbacks)
					callback();
			}
		}

		// store 'this' inside GLFW window
		glfwSetWindowUserPointer(window, this);

		// non-capturing lambda
		glfwSetScrollCallback(window, [](GLFWwindow* window, double /*xOffset*/, double yOffset)
			{
				// retrieve 'this' pointer
				OEInputSystem* inputSystem = static_cast<OEInputSystem*>(glfwGetWindowUserPointer(window));
				inputSystem->scrollDelta += static_cast<int>(yOffset);
			});
	}
	bool OEInputSystem::IsKeyPressed(const std::string& key) noexcept
	{
		auto it = m_OEInputStates.find(key);
		if (it != m_OEInputStates.end() && it->second == OEInputState::Pressed)
			return true;
		else
			return false;
	}

	bool OEInputSystem::IsKeyReleased(const std::string& key) noexcept
	{
		auto it = m_OEInputStates.find(key);
		if (it != m_OEInputStates.end() && it->second == OEInputState::Released)
			return true;
		else
			return false;
	}

	bool OEInputSystem::IsKeyHeld(const std::string& key) noexcept
	{
		auto it = m_OEInputStates.find(key);
		if (it != m_OEInputStates.end() && it->second == OEInputState::Held)
			return true;
		else
			return false;
	}

	std::pair<double, double> OEInputSystem::GetMousePosition() noexcept
	{
		return std::pair<double, double>(mouseX, mouseY);
	}

	std::pair<double, double> OEInputSystem::GetMouseDelta() noexcept
	{
		return std::pair<double, double>(deltaX, deltaY);
	}

	int OEInputSystem::GetScrollDelta() noexcept
	{
		return scrollDelta;
	}

	void OEInputSystem::RegisterKeyCallback(const std::string& key, std::function<void()> callback, OEInputState triggerState) noexcept
	{
		m_KeyCallbacks[{key, triggerState}].push_back(callback);
	}

	void OEInputSystem::RegisterMouseButtonCallback(int buttonIndex, std::function<void()> callback, OEInputState triggerState) noexcept
	{
		m_MouseCallbacks[{buttonIndex, triggerState}].push_back(callback);
	}

	void OEInputSystem::ResetPerFrameStates()
	{
		for (auto& [key, state] : m_OEInputStates)
		{
			if (state == OEInputState::Pressed) state = OEInputState::Held;
			else if (state == OEInputState::Released) state = OEInputState::None;
			else continue;
		}
	}

	void OEInputSystem::RegisterKey(const std::string& key) noexcept
	{
		m_OEInputStates[key] = OEInputState::None;
		m_PreviousKeyDown[key] = false;
	}
}
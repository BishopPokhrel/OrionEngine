#pragma once
#define GLFW_INCLUDE_NONE

#include <unordered_map>
#include <string>
#include <functional>

#include <GLFW/glfw3.h>

namespace std
{
	template<typename T1, typename T2>
	struct hash<std::pair<T1, T2>>
	{
		size_t operator()(const std::pair<T1, T2>& p) const noexcept
		{
			size_t h1 = std::hash<T1>(p.first);
			size_t h2 = std::hash<T2>(p.second);
			return h1 ^ (h2 << 1);
		}
	};
}

namespace OrionEngine
{
	enum class OEInputState
	{
		None = 0,
		Pressed, 
		Released,
		Held
	};

	class OEInputSystem
	{
	public:

		void InitializeOEInputSystem() noexcept;
		void Update(GLFWwindow* window) noexcept;
		bool IsKeyPressed(const std::string& key) noexcept;
		bool IsKeyReleased(const std::string& key) noexcept;
		bool IsKeyHeld(const std::string& key) noexcept;

		std::pair<double, double> GetMousePosition() noexcept;
		std::pair<double, double> GetMouseDelta() noexcept;

		int GetScrollDelta() noexcept;

		void RegisterKeyCallback(const std::string& key, std::function<void()> callback, OEInputState triggerState) noexcept;
		void RegisterMouseButtonCallback(int buttonIndex, std::function<void()> callback, OEInputState triggerState) noexcept;
		void ResetPerFrameStates() noexcept;
		void RegisterKey(const std::string& key) noexcept;

	private:

		// I did not write this
		std::unordered_map<std::string, OEInputState> m_OEInputStates;
		std::unordered_map<std::pair<std::string, OEInputState>, std::vector<std::function<void()>>> m_KeyCallbacks;
		std::unordered_map<std::pair<int, OEInputState>, std::vector<std::function<void()>>> m_MouseCallbacks;
		std::unordered_map<std::string, bool> m_PreviousKeyDown;
		std::unordered_map<int, bool> m_PreviousMouseDown;

		OEInputState m_OEInputState;

		double mouseX;
		double mouseY;
		double deltaX;
		double deltaY;
		int scrollDelta;
	};
}

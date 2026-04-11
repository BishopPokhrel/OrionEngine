#pragma once
#define GLFW_INCLUDE_NONE

#include <unordered_map>
#include <string>
#include <functional>
#include <vector>
#include <utility>

#include <GLFW/glfw3.h>

namespace OrionEngine
{
	enum class OEInputState
	{
		None = 0,
		Pressed,
		Released,
		Held
	};

	// Proper key structure (replaces std::pair usage)
	struct OEInputKey
	{
		int key = 0;
		OEInputState state = OEInputState::None;

		bool operator==(const OEInputKey& other) const noexcept
		{
			return key == other.key && state == other.state;
		}
	};

	// Hash for OEInputKey
	struct OEInputKeyHash
	{
		size_t operator()(const OEInputKey& k) const noexcept
		{
			size_t h1 = std::hash<int>{}(k.key);
			size_t h2 = std::hash<int>{}(static_cast<int>(k.state));
			return h1 ^ (h2 << 1);
		}
	};

	class OEInputSystem
	{
	public:
		void InitializeOEInputSystem(GLFWwindow* window) noexcept;
		void Update(GLFWwindow* window) noexcept;

		bool IsKeyPressed(const std::string& key) noexcept;
		bool IsKeyReleased(const std::string& key) noexcept;
		bool IsKeyHeld(const std::string& key) noexcept;

		std::pair<double, double> GetMousePosition() noexcept;
		std::pair<double, double> GetMouseDelta() noexcept;
		int GetScrollDelta() noexcept;

		void RegisterKeyCallback(const std::string& key, std::function<void()> callback, OEInputState state) noexcept;
		void RegisterMouseButtonCallback(int button, std::function<void()> callback, OEInputState state) noexcept;

		void ResetPerFrameStates() noexcept;
		void RegisterKey(const std::string& key) noexcept;

	private:
		std::unordered_map<std::string, OEInputState> m_OEInputStates;

		std::unordered_map<OEInputKey, std::vector<std::function<void()>>, OEInputKeyHash> m_KeyCallbacks;
		std::unordered_map<OEInputKey, std::vector<std::function<void()>>, OEInputKeyHash> m_MouseCallbacks;

		std::unordered_map<std::string, bool> m_PreviousKeyDown;
		std::unordered_map<int, bool> m_PreviousMouseDown;

		double mouseX = 0.0;
		double mouseY = 0.0;
		double deltaX = 0.0;
		double deltaY = 0.0;
		int scrollDelta = 0;
	};
}
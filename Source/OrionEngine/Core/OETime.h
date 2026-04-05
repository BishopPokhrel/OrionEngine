#pragma once
#include <chrono>

namespace OrionEngine
{
	class OETime
	{
	public:

		static void OETimeInit()
		{
			s_StartTime = Clock::now();
			s_LastFrameTime = s_StartTime;
		}

		static void OETimeUpdate()
		{
			auto now = Clock::now();
			s_DeltaTime = std::chrono::duration<float>(now - s_LastFrameTime).count();
			s_TimeSinceStart = std::chrono::duration<float>(now - s_StartTime).count();

			s_LastFrameTime = now;
			s_FrameCount++;
		}

		static float OEGetDeltaTime() { return s_DeltaTime; }
		static float OEGetTime() { return s_TimeSinceStart; }
		static float OEGetFrameCount() { return s_FrameCount; }

	private:

		using Clock = std::chrono::high_resolution_clock;

		inline static std::chrono::time_point<Clock> s_StartTime;
		inline static std::chrono::time_point<Clock> s_LastFrameTime;

		inline static float s_DeltaTime = 0.0f;
		inline static float s_TimeSinceStart = 0.0f;
		inline static uint64_t s_FrameCount = 0.0f;
	};
}
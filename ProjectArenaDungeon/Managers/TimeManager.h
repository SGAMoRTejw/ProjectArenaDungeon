#pragma once

class TimeManager
{
	DECLARE_SINGLETON(TimeManager)

public:
	using Clock = std::chrono::steady_clock;
	using TimePoint = std::chrono::time_point<Clock>;
	using Duration = std::chrono::duration<double>;

public:
	void Update();

	float GetDeltaTime() const { return static_cast<float>(deltaTime); }
	double GetWorldTime() const { return worldTime; }
	UINT GetFPS() const { return fps; }

	void SetTargetFPS(double fps);
	void WaitToTargetFrameRate();

	void CheckTime() const { std::cout << "[ FPS : " << fps << " ], [ WolrdTime : " << worldTime << " ], [ Delta : " << deltaTime << " ] \n"; }

private:
	TimePoint currentTime;
	TimePoint lastTime;
	double deltaTime = 0.0;

	double worldTime = 0.0;

	UINT frameCount = 0;
	double fpsTimeElapsed = 0.0;
	UINT fps = 0;

	Duration targetFrameTime = Duration::zero();
};
#pragma once
// TimeManager
// - 프레임 기반 시간 정보(deltaTime, worldTime) 및 FPS를 계산/제공
// - 목표 FPS(target FPS)가 설정된 경우 프레임 제한(WaitToTargetFrameRate()) 적용
// - Update()는 매 프레임 1회 호출

class TimeManager
{
	DECLARE_SINGLETON(TimeManager)

public:
	using Clock = std::chrono::steady_clock;
	using TimePoint = std::chrono::time_point<Clock>;
	using Duration = std::chrono::duration<double>;

public:
	// Update
	// - 현재 시간을 갱신하고 delataTime, worldTime, FPS를 계산
	void Update();

	float GetDeltaTime() const { return static_cast<float>(deltaTime); }
	double GetWorldTime() const { return worldTime; }
	UINT GetFPS() const { return fps; }

	// SetTargetFPS
	// - fps <= 0: 프레임 제한 해제
	// - fps > 0: 목표 프레임(1/fps) 설정
	void SetTargetFPS(double fps);
	// WaitToTargetFrameRate
	// - Update() 이후 호출이 전제
	// - 목표 프레임 시간까지 남은 시간이 있다면 대기
	void WaitToTargetFrameRate();

	// Debug helper
	void CheckTime() const { std::cout << "[ FPS : " << fps << " ], [ WolrdTime : " << worldTime << " ], [ Delta : " << deltaTime << " ] \n"; }

private:
	// 시간 측정용
	TimePoint currentTime;
	TimePoint lastTime;

	// 프레임 시간(초)
	double deltaTime = 0.0;

	// 누적 시간(초)
	double worldTime = 0.0;

	// FPS 측정용
	UINT frameCount = 0;
	double fpsTimeElapsed = 0.0;
	UINT fps = 0;

	// 목표 프레임 시간(초) = 1 / targetFPS
	Duration targetFrameTime = Duration::zero();
};
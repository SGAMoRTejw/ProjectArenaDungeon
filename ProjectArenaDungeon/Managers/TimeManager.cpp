#include "pch.h"
#include "TimeManager.h"

TimeManager::TimeManager()
{
	currentTime = Clock::now();
	lastTime = currentTime;
}

void TimeManager::Update()
{
	currentTime = Clock::now();
	Duration delta = currentTime - lastTime;
	deltaTime = delta.count();

	lastTime = currentTime;

	worldTime += deltaTime;

	fpsTimeElapsed += deltaTime;
	++frameCount;
	
	// fps 측정 및 frameCount, 경과 시간 리셋
	if (fpsTimeElapsed >= 1.0)
	{
		fps = frameCount;
		frameCount = 0;
		fpsTimeElapsed = 0.0;
	}
}

void TimeManager::SetTargetFPS(double fps)
{
	if (fps <= 0.0)
	{
		targetFrameTime = Duration::zero();
	}	
	else
	{
		targetFrameTime = Duration(1.0 / fps);
	}
}

void TimeManager::WaitToTargetFrameRate()
{
	if (targetFrameTime.count() <= 0.0)
	{
		return;
	}

	// 현재 시간과 경과 시간을 기준으로 
	// 목표 시간(targetFrameTime)에 도달하지 못한 경우 대기하며
	// 현재 시간과 경과 시간을 갱신
	// NOTE:
	// - CPU 사용량이 증가할 수 있다. 개선 방안 모색.
	TimePoint now = Clock::now();
	Duration elapsed = now - currentTime;

	while (elapsed < targetFrameTime)
	{
		now = Clock::now();
		elapsed = now - currentTime;
	}
}
#include "pch.h"
#include "GameInstance.h"

// Scenes
#include "Scenes/Scene.h"
// TODO: 테스트 씬 클래스가 생기면 include
// #include "Scenes/TestScene.h"

GameInstance::GameInstance() {}

GameInstance::~GameInstance()
{
	// NOTE:
	// - 씬이 해제되는 과정에서 다른 시스템(물리/리소스 등)을 참조할 수 있으므로
	//   해제 순서가 중요한 시스템이 생기면 여기에서 순서를 명확하게 정리
	currentScene = nullptr;
	sceneList.clear();
}

void GameInstance::Init()
{
	// TODO: 전역 유틸 초기화
	// Random::Init();

	// TODO: 테스트 씬
	// sceneList.push_back(std::make_shared<TestScene>());

	if (!sceneList.empty())
	{
		currentSceneIndex = 0;
		currentScene = sceneList[currentSceneIndex];
		currentScene->Init();
	}
}

void GameInstance::Update()
{
	// 테스트 전용 입력 기반 씬 전환
	// - 기본: 동일 씬 무시
	// - 디버그: 강제로 다시 로드 (Destroy -> Init)
	if (INPUT.GetKeyPress(VK_F1)) RequestSceneChange(0, false);
	if (INPUT.GetKeyPress(VK_F2)) RequestSceneChange(1, false);
	if (INPUT.GetKeyPress(VK_F3)) RequestSceneChange(2, false);
	// 강제 재시작 예시
	//if (INPUT.GetKeyPress(VK_F5)) RequestSceneChange(currentSceneIndex, true);

	if (currentScene)
		currentScene->Update();

	ApplySceneChange();
}

void GameInstance::Render()
{
	if (currentScene)
		currentScene->Render();
}

void GameInstance::RequestSceneChange(size_t index, bool bForceReload)
{
	if (index >= sceneList.size())
		return;

	// NOTE:
	// - 기본 정책: 동일 씬이면 무시
	// - 디버그/테스트 목적: 강제 재로드 요청 시 동일 씬도 허용
	if (!bForceReload && currentSceneIndex == index)
		return;

	pendingSceneIndex = index;
	pendingForceReload = bForceReload;
}

void GameInstance::ApplySceneChange()
{
	if (pendingSceneIndex == npos)
		return;

	// Apply 시점에서 최종 정책 재확인
	if (!pendingForceReload && currentSceneIndex == pendingSceneIndex)
	{
		pendingSceneIndex = npos;
		pendingForceReload = false;
		return;
	}

	// 기존 씬 정리
	if (currentScene)
	{
		currentScene->Destroy();
	}

	// 씬 교체
	currentSceneIndex = pendingSceneIndex;
	currentScene = sceneList[currentSceneIndex];

	// 요청 상태 리셋
	pendingSceneIndex = npos;
	pendingForceReload = false;

	// 새 씬 초기화
	if (currentScene)
	{
		currentScene->Init();
	}
}
#pragma once

class Scene;

// GameInstance
// - 프로그램 실행 동안 유지되는 최상위 게임 컨텍스트
// - Scene의 생성/소유/전환을 관리하고 Update/Render 호출을 연결
//
// NOTE:
// - 현재는 테스트 씬 1개를 기준으로 단순 구성
class GameInstance
{
public:
	GameInstance();
	~GameInstance();

	// Init
	// - 게임 실행에 필요한 초기 구성(씬 목록 구성, 초기 씬 설정)
	void Init();

	// Update
	// - 매 프레임 게임을 갱신
	// - 씬 전환 요청이 있는 경우 처리
	void Update();

	// Render
	// - 매 프레임 렌더링 호출
	void Render();

	// RequestSceneChange
	// - 씬 전환 요청 (즉시 전환하지 않고, 안전한 시점에 ApplySceneChange에서 처리)
	// - bForceReload == true 인 경우, 동일 씬이어도 Destroy -> Init 수행
	void RequestSceneChange(size_t index, bool bForceReload = false);

private:
	// ApplySceneChange
	// - 씬 전환 요청을 처리
	void ApplySceneChange();

private:
	// 씬 목록
	std::vector<std::shared_ptr<Scene>> sceneList;

	// 현재 활성 씬
	std::shared_ptr<Scene> currentScene;

	// NOTE: 
	// - -1을 size_t로 캐스팅할 때 발생하는 가장 큰 값을 "없음"의 의미로 사용
	// - 실제 값이 있다면 값에 해당하는 sceneIndex로 전환 요청
	static constexpr size_t npos = static_cast<size_t>(-1);

	// 현재 씬 인덱스
	size_t currentSceneIndex = npos;

	// 전환 대기 중 씬 인덱스
	size_t pendingSceneIndex = npos;

	// 동일 씬 다시 로드 여부
	bool pendingForceReload = false;
};
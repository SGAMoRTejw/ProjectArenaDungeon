#pragma once
#include <memory>
#include <vector>
#include <utility>

#include "Objects/Object.h"

class GameContext;     // 런 전체 상태
class DirectorSystem;  // 전투/이벤트/선택 흐름 제어

class Scene
{
public:
	virtual ~Scene() = default;

	// Init
	// - 씬 진입 시 1회 호출
	// - 필요한 Object 생성, 초기 상태 세팅
	virtual void Init() = 0;

	// Destroy
	// - 씬 종료 시 1회 호출
	// - 오브젝트 해제, 임시 리소스 정리
	virtual void Destroy() = 0;

	// Update
	// - 매 프레임 로직 처리
	virtual void Update()
	{
		for (const auto& object : objects)
			object->Update();
	}

	// Render
	// - 매 프레임 렌더링
	virtual void Render()
	{
		for (const auto& object : objects)
			object->Render();
	}

	// AddObject
	// - 씬에 Object 추가
	void AddObject(std::shared_ptr<Object> object)
	{
		object->Awake();
		objects.push_back(std::move(object));
	}

	// Context / Director 연결
	void SetContext(GameContext* context) { gameContext = context; }
	void SetDirector(DirectorSystem* director) { directorSystem = director; }

protected:
	// 씬에 소속된 오브젝트 목록
	std::vector<std::shared_ptr<Object>> objects;

	// 런 전체 상태 (씬 외부 데이터)
	GameContext* gameContext = nullptr;

	// 게임 흐름 제어자 (선택, 전투 흐름, 결과 반영)
	DirectorSystem* directorSystem = nullptr;
};
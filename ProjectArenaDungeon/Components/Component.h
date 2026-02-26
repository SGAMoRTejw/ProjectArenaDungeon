#pragma once

#include <string>
#include <utility>

class Object;
class Collider;

// Component
// - Object에 결합되는 기능 단위의 기반 클래스
// - Awake/Update/Render 와 충돌 이벤트 콜백을 제공
class Component
{
public:
	explicit Component(std::string compName);
	virtual ~Component() = default;

	// Awake
	// - 오브젝트가 씬에 배치되거나 활성화될 때 초기화 용도로 호출
	virtual void Awake() {}

	// Update
	// - 매 프레임 호출되는 갱신 함수
	virtual void Update() {}

	// Render
	// - 매 프레임 렌더링 호출
	virtual void Render() {}

	// OnCollisionEnter
	// - 충돌 시작 시점 콜백
	// - 충돌 반응이 필요한 컴포넌트만 override 해서 사용
	virtual void OnCollisionEnter(Collider* other) {}

	// OnCollisionExit
	// - 충돌 종료 시점 콜백
	// - 충돌 반응이 필요한 컴포넌트만 override 해서 사용
	virtual void OnCollisionExit(Collider* other) {}

	// GetName
	// - 컴포넌트 식별용 이름 반환
	const std::string& GetName() const { return name; }

	// GetOwner
	// - 소유 오브젝트 반환
	Object* GetOwner() const { return owner; }

	// SetOwner
	// - 소유 오브젝트를 설정한다
	// - 일반적으로 Object::AddComponent에서만 호출되는 것을 전제로 한다
	void SetOwner(Object* obj) { owner = obj; }

protected:
	std::string name;
	Object* owner = nullptr;
};
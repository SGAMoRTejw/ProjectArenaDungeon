#pragma once
#include <string>
#include <memory>
#include <unordered_map>
#include <vector>

#include "Config/ConstantValues.h"

class Component;
class Transform;
class Collider;

// Object
// - 씬에 존재하는 모든 실체의 기반 클래스
// - Transform을 기본으로 보유하며, 컴포넌트를 결합하여 기능을 확장
class Object
{
public:
	Object(const std::string& name, 
		DirectX::SimpleMath::Vector2 position = VEC2_ZERO,
		DirectX::SimpleMath::Vector2 scale = VEC2_ONE,
		float rotation = 0.0f);

	virtual ~Object() = default;

	// Awake
	// - 컴포넌트 초기화 진입점
	// - 씬에 배치된 직후(또는 활성화 직후) 1회 호출
	virtual void Awake();

	// Update
	// - 매 프레임 호출되는 갱신 함수
	// - Transform은 다른 컴포넌트들의 변경이 끝난 뒤 마지막에 1회만 업데이트
	virtual void Update();

	// Render
	// - 매 프레임 렌더링 호출
	virtual void Render();

	// OnCollisionEnter
	// - 충돌 시작 시점 이벤트 전달
	virtual void OnCollisionEnter(Collider* other);
	
	// OnCollisionExit
	// - 충돌 종료 시점 이벤트 전달
	virtual void OnCollisionExit(Collider* other);

	// AddComponent
	// - 컴포넌트를 오브젝트에 결합
	// - 동일 이름 컴포넌트가 이미 존재하면 추가하지 않음
	void AddComponent(const std::shared_ptr<Component>& component);

	template<typename T>
	std::shared_ptr<T> GetComponent(const std::string& compName) const
	{
		auto it = components.find(compName);
		if (it == components.end())
			return nullptr;

		return std::dynamic_pointer_cast<T>(it->second);
	}

	// GetTransform
	// - 오브젝트의 Transform 접근자
	Transform* GetTransform() { return transform.get(); }
	const Transform* GetTransform() const { return transform.get(); }

	// GetObjectName
	// - 디버깅/식별을 위한 이름 반환
	const std::string& GetObjectName() const { return name; }

	// SetActive
	// - 오브젝트 활성/비활성 상태 변경
	void SetActive(bool value) { bActive = value; }

	// IsActive
	// - 오브젝트가 활성 상태인지 반환
	bool IsActive() const { return bActive; }

protected:
	std::string name;

	// NOTE: Object는 Transform을 반드시 보유한다.
	std::shared_ptr<Transform> transform;

	// NOTE: key는 컴포넌트 고유 이름이며 중복되면 결합하지 않는다.
	std::unordered_map<std::string, std::shared_ptr<Component>> components;

	// NOTE: Transform은 updateList[0]에 고정 배치하고,
	//       Update()에서는 Transform을 제외한 컴포넌트를 먼저 갱신한 뒤 Transform을 마지막에 업데이트한다.
	std::vector<std::shared_ptr<Component>> updateList;

	bool bActive = true;
};
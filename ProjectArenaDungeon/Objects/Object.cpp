#include "pch.h"
#include "Object.h"

// Components
#include "Components/Component.h"
#include "Components/Transform.h"
//#include "Components/Collider.h"

Object::Object(const std::string& name, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, float rotation)
	: name(name)
{
	transform = std::make_shared<Transform>();

	transform->SetPosition(position);
	transform->SetScale(scale);
	transform->SetRotationDegree(rotation);

	// NOTE: Transform은 항상 존재하며 updateList[0]에 배치된다.
	AddComponent(transform);
}

void Object::Awake()
{
	if (!bActive)
		return;

	for (const auto& component : updateList)
	{
		if (component)
			component->Awake();
	}
}

void Object::Update()
{
	if (!bActive)
		return;

	// NOTE: Transform(0번)을 제외한 컴포넌트를 먼저 갱신한다.
	for (size_t i = 1; i < updateList.size(); ++i)
	{
		if (updateList[i])
			updateList[i]->Update();
	}

	// NOTE: Transform은 마지막에 1회만 업데이트한다.
	if (transform)
		transform->Update();
}

void Object::Render()
{
	if (!bActive)
		return;

	for (const auto& component : updateList)
	{
		if (component)
			component->Render();
	}
}

//void Object::OnCollisionEnter(Collider* other)
//{
//	if (!bActive)
//		return;
//
//	for (const auto& component : updateList)
//	{
//		if (component)
//			component->OnCollisionEnter(other);
//	}
//}
//
//void Object::OnCollisionExit(Collider* other)
//{
//	if (!bActive)
//		return;
//
//	for (const auto& component : updateList)
//	{
//		if (component)
//			component->OnCollisionExit(other);
//	}
//}

void Object::AddComponent(const std::shared_ptr<Component>& component)
{
	if (!component)
		return;

	const std::string& compName = component->GetName();

	auto [it, inserted] = components.try_emplace(compName, component);
	if (!inserted)
		return;

	component->SetOwner(this);

	// NOTE: Transform은 항상 updateList[0]에 유지한다.
	if (auto asTransform = std::dynamic_pointer_cast<Transform>(component))
	{
		transform = asTransform;

		if (updateList.empty())
			updateList.emplace_back(component);
		else
			updateList[0] = component;

		return;
	}

	updateList.emplace_back(component);
}
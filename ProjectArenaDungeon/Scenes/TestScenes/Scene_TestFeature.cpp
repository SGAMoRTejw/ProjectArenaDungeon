#include "pch.h"
#include "Scene_TestFeature.h"

#include <box2d/box2d.h>
#include "_Libraries/DirectXTK/SimpleMath.h"

#include "Objects/Object.h"

#include "Components/Transform.h"
#include "Components/MeshRenderer.h"
#include "Components/RigidBody.h"
#include "Components/BoxCollider.h"

#include "Utilities/ObjectFactory.h"
#include "Utilities/Random.h"

namespace
{
	using Vector2 = DirectX::SimpleMath::Vector2;
	constexpr Vector2 TEST_DEFAULT_OBJECT_SCALE = Vector2(50.0f, 50.0f);
}

void Scene_TestFeature::Init()
{
	testObj = ObjectFactory::CreateColorRect(CENTER, TEST_DEFAULT_OBJECT_SCALE, 0.0f, RED);
	testObj->AddComponent(std::make_shared<RigidBody>(BodyType::Dynamic));
	testObj->AddComponent(std::make_shared<BoxCollider>());
	AddObject(testObj);

	auto transform = testObj->GetTransform();
	if (transform)
	{
		std::cout << "valid transform" << '\n';
	}

	auto material = testObj->GetComponent<MeshRenderer>("MeshRenderer")->GetMaterial();
	if (material)
	{
		std::cout << "valid material" << '\n';
	}

	auto body = testObj->GetComponent<RigidBody>("RigidBody");
	if (b2Body_IsValid(body->GetBodyId()))
	{
		std::cout << "valid rigid body" << '\n';
	}

	auto collider = testObj->GetComponent<BoxCollider>("BoxCollider");
	if (b2Shape_IsValid(collider->GetShapeId()))
	{
		std::cout << "valid collider" << '\n';
	}
}

void Scene_TestFeature::Destroy()
{
	testObj = nullptr;

	objects.clear();
}

void Scene_TestFeature::Update()
{
	std::cout << Random::Range(1, 10) << '\n';

	__super::Update();
}

void Scene_TestFeature::Render()
{
	__super::Render();
}

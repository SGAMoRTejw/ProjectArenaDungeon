#pragma once
#include "Scenes/Scene.h"

class Object;

class Scene_TestFeature : public Scene
{
public:
	void Init() override;
	void Destroy() override;

	void Update() override;
	void Render() override;

private:
	std::shared_ptr<Object> testObj;
};
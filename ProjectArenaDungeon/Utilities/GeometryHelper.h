#pragma once
#include <memory>

class Mesh;

// GeometryHelper
// - 공용 프리미티브 메시를 생성/캐싱하여 재사용
// - 동일 메시를 반복 생성하지 않기 위해 static 캐시를 사용
namespace GeometryHelper
{
std::shared_ptr<Mesh> CreateRectangle();
std::shared_ptr<Mesh> CreateLine();
std::shared_ptr<Mesh> CreateWireCircle(UINT segments);
std::shared_ptr<Mesh> CreateSolidCircle(UINT segments);
std::shared_ptr<Mesh> CreateTexturedQuad();
}
#pragma once
#include <memory>
#include <string>
#include "_Libraries/DirectXTK/SimpleMath.h"

class Object;

using Vector2 = DirectX::SimpleMath::Vector2;
using Color = DirectX::SimpleMath::Color;

// ObjectFactory
// - 자주 쓰는 기본 오브젝트 생성을 단일 API로 제공한다.
// - Transform(Object 생성자) + MeshRenderer(메시/머티리얼 결합)까지 구성하여 반환한다.
//
// NOTE:
// - 최종 목표는 RenderSystem에 DrawCall을 요청하는 구조로 전환하는 것이지만,
//   현재 단계에서는 MeshRenderer가 직접 DrawIndexed를 수행하는 구조를 사용한다.
namespace ObjectFactory
{
std::shared_ptr<Object> CreateSprite(Vector2 position, Vector2 scale, float rotation, const std::wstring& path);
std::shared_ptr<Object> CreateColorRect(Vector2 position, Vector2 scale, float rotation, Color color);
std::shared_ptr<Object> CreateLine(Vector2 position, float length, float rotation, Color color);
std::shared_ptr<Object> CreateWireCircle(Vector2 position, Vector2 scale, float rotation, Color color, UINT segments);
std::shared_ptr<Object> CreateSolidCircle(Vector2 position, Vector2 scale, float rotation, Color color, UINT segments);
}
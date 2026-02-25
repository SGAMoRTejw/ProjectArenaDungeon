#pragma once
#include "Renders/IA/VertexBuffer.h"
#include "Renders/IA/IndexBuffer.h"

// Mesh
// - 정점/인덱스 버퍼(VertexBuffer/IndexBuffer)를 소유하고 IA 단계 바인딩을 담당하는 클래스
class Mesh
{
public:
	// Create
	// - CPU 메모리의 vertices/indices로 GPU VB/IB 생성
	template<typename T>
	void Create(const std::vector<T>& vertices, const std::vector<UINT>& indices)
	{
		VB = std::make_unique<VertexBuffer>();
		VB->Create(vertices);

		IB = std::make_unique<IndexBuffer>();
		IB->Create(indices);
	}

	// Bind
	// - IA 단계에 VB/IB 바인딩
	void Bind();

	IndexBuffer* GetIndexBuffer() { return IB.get(); }
	UINT GetIndexCount() const { return IB ? IB->GetCount() : 0; }

private:
	std::unique_ptr<VertexBuffer> VB;
	std::unique_ptr<IndexBuffer> IB;
};
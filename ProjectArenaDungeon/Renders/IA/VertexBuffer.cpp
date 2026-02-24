#include "pch.h"
#include "VertexBuffer.h"

void VertexBuffer::Bind(UINT slot)
{
	DEVICE_CONTEXT->IASetVertexBuffers(slot, 1, buffer.GetAddressOf(), &stride, &offset);
}
#include "pch.h"
#include "Mesh.h"

void Mesh::Bind()
{
	if (VB) VB->Bind();
	if (IB) IB->Bind();
}
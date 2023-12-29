#include "Framework.h"
#include "ModelMesh.h"

ModelMesh::ModelMesh()
{
}

ModelMesh::~ModelMesh()
{
	delete mesh;
}

void ModelMesh::Create()
{
	mesh = new Mesh(vertices, indices);
}

void ModelMesh::Render()
{
	material->SetMaterial();
	    mesh->SetMesh();

	DC->DrawIndexed(indices.size(), 0, 0);
}

void ModelMesh::RenderInstanced(UINT instanceCount)
{
	material->SetMaterial();
	    mesh->SetMesh();

	DC->DrawIndexedInstanced(indices.size(), instanceCount, 0, 0, 0);
}

#include "Framework.h"
#include "Model.h"

Model::Model(string name, wstring shaderFile)
{
	reader = new ModelReader(name);
	reader->SetShader(shaderFile);
}

Model::~Model()
{
	delete reader;
}

void Model::Update()
{
	Transform::Update();
}

void Model::Render()
{
	Transform::SetWorld();

	reader->Render();
}

void Model::RenderInstanced(UINT instanceCount)
{
	Transform::SetWorld();

	reader->RenderInstanced(instanceCount);
}

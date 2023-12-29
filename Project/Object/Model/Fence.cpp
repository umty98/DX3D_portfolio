#include "Framework.h"
#include "Fence.h"

Fence::Fence()
	:Model("wooden_fence")
{
	reader->GetMaterial()[0]->Load(L"wooden_fence.mat");

	this->SetLabel("fence");

}

Fence::~Fence()
{
}

void Fence::Update()
{
	Model::Update();
}

void Fence::Render()
{
	Model::Render();
}

void Fence::PostRender()
{
	Model::Debug();
}

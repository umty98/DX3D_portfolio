#include "Framework.h"
#include "Tree.h"

Tree::Tree()
	:Model("Tree")
{
	reader->GetMaterial()[0]->Load(L"Tree.mat");
	this->rotation.x = XM_PIDIV2;

	this->SetLabel("Tree");
}

Tree::~Tree()
{
}

void Tree::Update()
{
	Model::Update();
}

void Tree::Render()
{
	Model::Render();
}

void Tree::PostRender()
{
	Model::Debug();
}

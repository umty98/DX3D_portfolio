#include "Framework.h"
#include "ModelAnimationScene.h"

ModelAnimationScene::ModelAnimationScene()
{
	groot = new Groot;

	terrain = new Terrain(L"LandScape/Dirt.png", L"HeightMap/HeightMap.png");
	//terrain->scale *= 2.0f;

	//CAMERA->SetTarget(groot);

	crossHair = new Quad();
	crossHair->GetMaterial()->SetDiffuseMap(L"UI/CrossHair.png");
	crossHair->GetMaterial()->SetShader(L"Texture");
	crossHair->scale = { 200, 200, 1 };

	sky = new SkyBox(L"Landscape/Test2.dds");
}

ModelAnimationScene::~ModelAnimationScene()
{
	delete groot;
	delete terrain;
	delete crossHair;

	delete sky;
}

void ModelAnimationScene::Update()
{
	    groot->Update();
	  terrain->Update();
	crossHair->Update();

	groot->translation.y = terrain->GetHeight(groot->GetGlobalPosition());

	crossHair->translation.x = mousePos.x;
	crossHair->translation.y = WIN_HEIGHT - mousePos.y;

	if (KEY_PRESS(VK_LBUTTON))
	{
		groot->rotation.y = LERP(groot->rotation.y, groot->rotation.y - XM_PI, Time::Delta());
	}
}

void ModelAnimationScene::PreRender()
{
	sky->PostRender();
}

void ModelAnimationScene::Render()
{
	      sky->Render();

	    groot->Render();
	  terrain->Render();
}

void ModelAnimationScene::PostRender()
{
	groot->PostRender();

	crossHair->Render();
}

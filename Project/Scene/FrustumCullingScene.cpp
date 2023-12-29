#include "Framework.h"
#include "FrustumCullingScene.h"

FrustumCullingScene::FrustumCullingScene()
{
	for (UINT x = 0; x < SIZE; x++)
	{
		for (UINT y = 0; y < SIZE; y++)
		{
			for (UINT z = 0; z < SIZE; z++)
			{
				Sphere* sphere = new Sphere;

				sphere->translation = Vector3(100 * x, 100 * y, 100 * z);
				sphere->scale *= 20.0f;
				sphere->GetMaterial()->SetDiffuseMap(L"Landscape/Box.png");
				sphere->Update();

				spheres.emplace_back(sphere);
			}
		}
	}
}

FrustumCullingScene::~FrustumCullingScene()
{
	for (Sphere* sphere : spheres)
		delete sphere;

	spheres.clear();
}

void FrustumCullingScene::Update()
{
	for (Sphere* sphere : spheres)
	{
		if (CAMERA->ContainSphere(sphere->GetGlobalPosition(), sphere->Radius()))
			sphere->Update();
	}
}

void FrustumCullingScene::PreRender()
{
}

void FrustumCullingScene::Render()
{
	drawCount = 0;

	for (Sphere* sphere : spheres)
	{
		if (CAMERA->ContainSphere(sphere->GetGlobalPosition(), sphere->Radius()))
		{
			sphere->Render();
			drawCount++;
		}
	}
}

void FrustumCullingScene::PostRender()
{
	ImGui::Text("DrawCount : %d", drawCount);
}

#include "Framework.h"
#include "DeferredRenderScene.h"

DeferredRenderScene::DeferredRenderScene()
{
	CreateObjects();

	gBuffer = new GeometryBuffer();

	material = new Material(L"12DeferredRender");

	vector<UINT> vertices = { 0, 1, 2, 3 };

	vertexBuffer = new VertexBuffer(vertices);
}

DeferredRenderScene::~DeferredRenderScene()
{
	delete gBuffer;

	delete  floor;
	delete  bunny;
	delete  groot;
	delete sphere;

	delete material;
	delete vertexBuffer;
}

void DeferredRenderScene::Update()
{
	 floor->Update();
	 bunny->Update();
	 groot->Update();
	sphere->Update();
}

void DeferredRenderScene::PreRender()
{
	gBuffer->SetMultiRenderTarget();

	 floor->Render();
	 bunny->Render();
	 groot->Render();
	sphere->Render();
}

void DeferredRenderScene::Render()
{
	vertexBuffer->IASetBuffer(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	 gBuffer->SetSRVs();
	material->SetMaterial();

	DC->Draw(4, 0);
}

void DeferredRenderScene::PostRender()
{
//	gBuffer->PostRender();
}

void DeferredRenderScene::CreateObjects()
{
	floor = new Quad();
	floor->SetLabel("Floor");

	floor->rotation.x = XM_PIDIV2;
	floor->translation.y = 1.0f;
	floor->scale *= 100.0f;

	floor->GetMaterial()->SetShader(L"12GeometryBuffer");

	groot = new Groot;
	groot->GetReader()->SetShader(L"12GeometryBuffer");
	
	bunny = new Model("StanfordBunny");
	bunny->SetLabel("Bunny");
	bunny->scale *= 0.05f;
	bunny->translation = { -30, 12 , 0 };
	bunny->GetReader()->SetShader(L"12GeometryBuffer");

	sphere = new Sphere();
	sphere->scale *= 5.0f;
	sphere->translation = { 30, 20, 0 };
	sphere->GetMaterial()->SetDiffuseMap(L"Landscape/FieldStone_DM.tga");
	sphere->GetMaterial()->SetSpecularMap(L"Landscape/FieldStone_SM.tga");
	sphere->GetMaterial()->SetNormalMap(L"Landscape/FieldStone_NM.tga");
	sphere->GetMaterial()->SetShader(L"12GeometryBuffer");
}

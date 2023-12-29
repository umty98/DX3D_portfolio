#include "Framework.h"
#include "WaterScene.h"

WaterScene::WaterScene()
{
	CreateObjects();

	//reflection = new Reflection(floor);
	//refraction = new Refraction(L"Landscape/Wave.dds");

	sky = new SkyBox(L"Landscape/Snow_ENV.dds");

	//floor->GetMaterial()->SetShader(L"20Refraction");

	water = new Water(L"Landscape/WaveNormal.png");

	shadow = new Shadow(10000, 10000);

	LightBuffer::LightData& light = Environment::GetInstance()->GetLightBuffer()->data.lights[0];

	light.type     = 1;
	light.position = { 0, 100, -50 };
	light.range    = 2000.0f;
}

WaterScene::~WaterScene()
{
	delete  floor;
	delete  bunny;
	delete  groot;
	delete sphere;

	delete sky;

	//delete reflection;
	//delete refraction;

	delete water;

	delete shadow;
}

void WaterScene::Update()
{
	//reflection->Update();
	//refraction->Update();

	 //water->Update();

	 floor->Update();
	 bunny->Update();
	 groot->Update();
	sphere->Update();
}

void WaterScene::PreRender()
{
	//reflection->SetPreRender();
	//refraction->SetPreRender();

	//water->SetRefraction();

	//   sky->Render();
	// bunny->Render();
	// groot->Render();
	//sphere->Render();

	//water->SetReflection();

	//   sky->Render();
	// bunny->Render();
	// groot->Render();
	//sphere->Render();

	shadow->SetPreRender();

	 bunny->Render();
	 groot->Render();
	sphere->Render();
}

void WaterScene::Render()
{
	   sky->Render();

	//reflection->SetRender();
	//refraction->SetRender();
	// water->Render();
	shadow->SetRender();

	 floor->Render();
	 bunny->Render();
	 groot->Render();
	sphere->Render();
}

void WaterScene::PostRender()
{
	//reflection->PostRender();
	//refraction->PostRender();
	//refraction->Debug();
	//water->Debug();
	floor->Debug();
	shadow->PostRender();
}

void WaterScene::CreateObjects()
{
	wstring shaderFile = L"22Shadow";

	floor = new Quad();
	floor->SetLabel("Floor");

	floor->rotation.x = XM_PIDIV2;
	floor->translation.y = -1.0f;
	floor->scale *= 100.0f;

	floor->GetMaterial()->SetShader(shaderFile);

	groot = new Groot;
	groot->GetReader()->SetShader(L"09Light");
	
	bunny = new Model("StanfordBunny");
	bunny->SetLabel("Bunny");
	bunny->scale *= 0.05f;
	bunny->translation = { -30, 12 , 0 };
	bunny->GetReader()->SetShader(L"09Light");

	sphere = new Sphere();
	sphere->scale *= 5.0f;
	sphere->translation = { 30, 20, 0 };
	sphere->GetMaterial()->SetDiffuseMap(L"Landscape/FieldStone_DM.tga");
	sphere->GetMaterial()->SetSpecularMap(L"Landscape/FieldStone_SM.tga");
	sphere->GetMaterial()->SetNormalMap(L"Landscape/FieldStone_NM.tga");
	sphere->GetMaterial()->SetShader(L"09Light");
}

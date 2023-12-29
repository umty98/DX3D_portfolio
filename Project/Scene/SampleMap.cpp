#include "Framework.h"
#include "SampleMap.h"

SampleMap::SampleMap()
{
	sky = new SkyBox(L"Landscape/Snow_ENV.dds");

	terrain1 = new Terrain(L"Landscape/Floor.png", L"HeightMap/AlphaMap.png");
	//terrain1->scale *= 2.0f;

	terrain2 = new Terrain(L"Landscape/Dirt3.png", L"HeightMap/Pock.png");

	terrain2->scale.x *= 2.57f;
	terrain2->rotation.y = -XM_PIDIV2;
	terrain2->translation = { 1.24f,20.19f,0.0f };

	terrain2->SetLabel("Pock");

	water = new Water(L"Landscape/WaveNormal.png");
	water->scale = { 0.25f,0.77f,1.11f };
	//water->scale = { 1.11f ,0.77f,0.25f };
	water->rotation.z = -XM_PIDIV2;
	//water->rotation.x = -XM_PIDIV2;
	water->translation = { -50.47f,30.0f,130.0f };

	//fence = new Fence();

	CreateFence();

	//particle = new ParticleSystem("Water.fx");
	CreateWater();

	tree = new Tree();

}

SampleMap::~SampleMap()
{
	delete terrain1;
	delete terrain2;

	//delete fence;

	delete sky;

	delete water;

	for (Fence* fence : fences)
		delete fence;
	fences.clear();

	//delete particle;
	for (ParticleSystem* particle : particles)
		delete particle;
	particles.clear();

	delete tree;
}

void SampleMap::Update()
{
	water->Update();

	terrain1->Update();
	terrain2->Update();

	for (Fence* fence : fences)
		fence->Update();
	//fence->Update();

	if (particletime > 4.0f)
	{

		//particle->Play({ -48.470f, 30.0f, 130.0f });
		for (ParticleSystem* particle : particles)
		{
			if (particle != nullptr) {
				particle->PlayWP(); 
			}
			
		}


		particletime = 0.0f;
	}
	else
	{
		particletime += Time::Delta();
	}

	
	for (ParticleSystem* particle : particles)
	{
		if (particle != nullptr) {
			particle->Update();
		}
	}

	tree->Update();
}

void SampleMap::PreRender()
{
	water->SetReflection();
	sky->Render();
	terrain1->Render();
	terrain2->Render();
	//fence->Render();

	water->SetRefraction();
	sky->Render();
	terrain1->Render();
	terrain2->Render();
	//fence->Render();
}

void SampleMap::Render()
{
	sky->Render();

	water->Render();

	terrain1->Render();
	terrain2->Render();

	for (Fence* fence : fences)
		fence->Render();
	//fence->Render();

	//particle->Render();

	for (ParticleSystem* particle : particles)
		particle->Render();

	tree->Render();
}

void SampleMap::PostRender()
{
	terrain2->Debug();
	water->Debug();

	for (Fence* fence : fences)
		fence->Debug();
	//fence->Debug();


	tree->Debug();
}

void SampleMap::CreateFence()
{
	for (UINT x = 0; x < 6; x++)
	{
		Fence* fence = new Fence;

		fence->translation = { float(-37.01 + 35 * x), float(29.42 - 5 * x), 0.0f };
		fence->rotation.z = -XM_PIDIV2 * 1.07f;
		fences.emplace_back(fence);

	}
	for (UINT x = 0; x < 6; x++)
	{
		Fence* fence = new Fence;

		fence->translation = { float(-37.01 + 35 * x), float(29.42 - 5 * x), 255.0f };
		fence->rotation.z = +XM_PIDIV2 * 1.07f;
		fence->rotation.y = XM_PI;
		fences.emplace_back(fence);

	}
}

void SampleMap::CreateWater()
{
	for (UINT x = 0; x < 20; x++)
	{
		Vector3 particlePosition = { -48.470f, 34.0f, 82.0f + 5.0f * x };

		ParticleSystem* particle = new ParticleSystem("Water.fx", particlePosition);
		if (particle != nullptr) {
			particles.emplace_back(particle);
		}
	}
}

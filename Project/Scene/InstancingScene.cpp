#include "Framework.h"
#include "InstancingScene.h"

InstancingScene::InstancingScene()
{
	//quads.resize(COUNT * COUNT);

	//for (float x = 0; x < COUNT; x++)
	//{
	//	for (float y = 0; y < COUNT; y++)
	//	{
	//		quads[x + y * COUNT] = new Quad();
	//		quads[x + y * COUNT]->GetMaterial()->SetDiffuseMap(L"Landscape/Box.png");
	//		quads[x + y * COUNT]->translation = { 2 * x, 2 * y, 1 };
	//	}
	//}

	quad = new Quad();
	quad->GetMaterial()->SetDiffuseMap(L"Landscape/Box.png");
	quad->GetMaterial()->SetShader(L"06TextureInstancing");

	instanceData.resize(COUNT * COUNT);

	for (float x = 0; x < COUNT; x++)
	{
		for (float y = 0; y < COUNT; y++)
		{
			Matrix transform = XMMatrixTranslation(2 * x, 2 * y, 1);

			instanceData[x + y * COUNT].transform = XMMatrixTranspose(transform);
			 
			instanceData[x + y * COUNT].color.x = Random(0.0f, 1.0f);
			instanceData[x + y * COUNT].color.y = Random(0.0f, 1.0f);
			instanceData[x + y * COUNT].color.z = Random(0.0f, 1.0f);
		}
	}

	instanceBuffer = new VertexBuffer(instanceData);
}

InstancingScene::~InstancingScene()
{
	//for (Quad* quad : quads)
	//	delete quad;

	//quads.clear();

	delete quad;
	delete instanceBuffer;
}

void InstancingScene::Update()
{
	//for (Quad* quad : quads)
	//	quad->Update();
}

void InstancingScene::PreRender()
{
}

void InstancingScene::Render()
{
	//for (Quad* quad : quads)
	//	quad->Render();

	instanceBuffer->IASetBuffer(1);

	quad->RenderInstanced(instanceData.size());
}

void InstancingScene::PostRender()
{
}

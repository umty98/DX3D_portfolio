#include "Framework.h"
#include "ModelInstancingScene.h"

ModelInstancingScene::ModelInstancingScene()
{
	model = new ModelAnimatorInstancing("Groot");
	model->ReadClip("Sad Idle");
	model->ReadClip("Running");
	model->ReadClip("Standing Melee Attack 360 High");
	model->ReadClip("Mutant Swiping");

	model->GetReader()->GetMaterial()[0]->SetDiffuseMap(L"Model/Groot/groot_diffuse.png");
	model->GetReader()->GetMaterial()[0]->SetNormalMap (L"Model/Groot/groot_normal_map.png");

	model->GetClips()[3]->SetEndEvent(0.1f, bind(&ModelInstancingScene::Count, this));
	model->GetClips()[3]->SetEndEvent(0.9f, bind(&ModelInstancingScene::Count, this));



	for (float z = 0; z < 10; z++)
	{
		for (float x = 0; x < 10; x++)
		{
			Transform* transform = model->Add();

			transform->translation = { x * 10, 0, z * 10 };
			transform->scale *= 0.01f;
		}
	}
}

ModelInstancingScene::~ModelInstancingScene()
{
	delete model;
}

void ModelInstancingScene::Update()
{
	model->Update();
}

void ModelInstancingScene::PreRender()
{
}

void ModelInstancingScene::Render()
{
	model->Render();
}

void ModelInstancingScene::PostRender()
{
	ImGui::SliderInt("InstanceIndex", &instanceIndex, 0, 100);
	ImGui::SliderInt("ClipIndex", &clip, 0, 3);

	if (ImGui::Button("Play"))
		model->PlayClip(instanceIndex, clip, 1.0f, 1.0f);

	//model->Debug();

	ImGui::Text("CanCombo : %d", canCombo);
}

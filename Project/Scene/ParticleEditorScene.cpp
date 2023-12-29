#include "Framework.h"
#include "ParticleEditorScene.h"

ParticleEditorScene::ParticleEditorScene()
{
	quad = new Quad();

	quad->GetMaterial()->SetShader(L"18Particle");
	quad->GetMaterial()->SetDiffuseMap(L"Effect/Snow.png");

	instanceData.resize(MAX_COUNT);

	instanceBuffer = new VertexBuffer(instanceData);

	Init();
}

ParticleEditorScene::~ParticleEditorScene()
{
	delete quad;
	delete instanceBuffer;
}

void ParticleEditorScene::Update()
{
	lifeTime += Time::Delta();

	UpdatePhysics();
	UpdateColor();

	quad->Update();

	if (lifeTime > particleData.duration)
		Init();
}

void ParticleEditorScene::PreRender()
{
}

void ParticleEditorScene::Render()
{
	if (particleData.isAdditive)
		STATE->AdditiveBegin();
	else
		STATE->AlphaBegin();

	STATE->DepthWriteMaskZero();

	instanceBuffer->IASetBuffer(1);

	quad->RenderInstanced(drawCount);

	STATE->AlphaEnd();
	STATE->DepthEnable();
}

void ParticleEditorScene::PostRender()
{
	EditTexture();

	ImGui::Checkbox("isLoop" ,      &particleData.isLoop);
	ImGui::Checkbox("isAdditive" ,  &particleData.isAdditive );
	ImGui::Checkbox("isBillboard" , &particleData.isBillboard);
	ImGui::SliderInt("ParticleCount", (int*)&particleCount, 1, MAX_COUNT);
	ImGui::SliderFloat("duration", &particleData.duration, 0.0f, 30.0f);
	ImGui::SliderFloat3("minStartPos",        (float*)&particleData.minStartPos       , -10, 10);    
	ImGui::SliderFloat3("maxStartPos",        (float*)&particleData.maxStartPos       , -10, 10);
	ImGui::SliderFloat3("minVelocity",        (float*)&particleData.minVelocity       , -1,  1);
	ImGui::SliderFloat3("maxVelocity",        (float*)&particleData.maxVelocity       , -1,  1);
	ImGui::SliderFloat3("minAcceleration",    (float*)&particleData.minAcceleration   , -5,  5);
	ImGui::SliderFloat3("maxAcceleration",    (float*)&particleData.maxAcceleration   , -5,  5);
	ImGui::SliderFloat3("minStartScale",      (float*)&particleData.minStartScale     ,  0,  5);
	ImGui::SliderFloat3("maxStartScale",      (float*)&particleData.maxStartScale     ,  0,  5);
	ImGui::SliderFloat3("minEndScale",        (float*)&particleData.minEndScale       ,  0,  5);
	ImGui::SliderFloat3("maxEndScale",        (float*)&particleData.maxEndScale       ,  0,  5);
	ImGui::SliderFloat ("minSpeed",           (float*)&particleData.minSpeed          ,  0, particleData.maxSpeed);
	ImGui::SliderFloat ("maxSpeed",           (float*)&particleData.maxSpeed          ,  particleData.minSpeed, 200);
	ImGui::SliderFloat ("minAngularVelocity", (float*)&particleData.minAngularVelocity, -10, 10);
	ImGui::SliderFloat ("maxAngularVelocity", (float*)&particleData.maxAngularVelocity, -10, 10);
	ImGui::SliderFloat ("minStartTime",       (float*)&particleData.minStartTime 	  , 0, particleData.maxStartTime);
	ImGui::SliderFloat ("maxStartTime",       (float*)&particleData.maxStartTime 	  , particleData.minStartTime, particleData.duration);
	ImGui::ColorEdit4  ("startColor",         (float*)&particleData.startColor);   	
	ImGui::ColorEdit4  ("endColor ",          (float*)&particleData.endColor  );  

	SaveDialog();
	ImGui::SameLine();
	LoadDialog();
}

void ParticleEditorScene::UpdatePhysics()
{
	drawCount = 0;

	for (ParticleInfo& info : particleInfos)
	{
		if (info.startTime > lifeTime)
			continue;

		info.velocity += info.acceleration * Time::Delta();

		info.transform.translation += info.velocity * info.speed * Time::Delta();

		info.transform.rotation.z += info.angularVelocity * Time::Delta();

		if (particleData.isBillboard)
		{
			Vector3 rot = CAMERA->rotation;

			rot.z = info.transform.rotation.z;

			info.transform.rotation = rot;
		}

		float timeScale = (lifeTime - info.startTime) / (particleData.duration - info.startTime);

		info.transform.scale = LERP(info.startScale, info.endScale, timeScale);

		info.transform.Update();

		//instanceData[drawCount].color.x = Random(0.0f, 1.0f);
		//instanceData[drawCount].color.y = Random(0.0f, 1.0f);
		//instanceData[drawCount].color.z = Random(0.0f, 1.0f);
		instanceData[drawCount++].transform = XMMatrixTranspose(info.transform.GetWorld());
	}

	instanceBuffer->UpdateVertex(instanceData.data(), drawCount);
}

void ParticleEditorScene::UpdateColor()
{
	float timeScale = lifeTime / particleData.duration;

	Vector4 color;
	color.x = LERP(particleData.startColor.x, particleData.endColor.x, timeScale);
	color.y = LERP(particleData.startColor.y, particleData.endColor.y, timeScale);
	color.z = LERP(particleData.startColor.z, particleData.endColor.z, timeScale);
	color.w = LERP(particleData.startColor.w, particleData.endColor.w, timeScale);

	quad->GetMaterial()->GetBuffer()->data.diffuse = color;
}

void ParticleEditorScene::Init()
{
	lifeTime = 0.0f;
	drawCount = 0;
	particleData.count = particleCount;

	 instanceData.clear();
	particleInfos.clear();

	 instanceData.resize(particleData.count);
	particleInfos.resize(particleData.count);

	UINT count = 0;

	for (ParticleInfo& info : particleInfos)
	{
		info.transform.translation = Random(particleData.minStartPos,        particleData.maxStartPos);
		info.velocity              = Random(particleData.minVelocity,        particleData.maxVelocity);
		info.acceleration          = Random(particleData.minAcceleration,    particleData.maxAcceleration);
		info.startScale            = Random(particleData.minStartScale,      particleData.maxStartScale);
		info.endScale              = Random(particleData.minEndScale,        particleData.maxEndScale);
		info.speed                 = Random(particleData.minSpeed,           particleData.maxSpeed);
		info.angularVelocity       = Random(particleData.minAngularVelocity, particleData.maxAngularVelocity);
		info.startTime             = Random(particleData.minStartTime,       particleData.maxStartTime);

		info.velocity.Normalize();

		//instanceData[count].color.x = Random(0.0f, 1.0f);
		//instanceData[count].color.y = Random(0.0f, 1.0f);
		//instanceData[count].color.z = Random(0.0f, 1.0f);

		count++;
	}
}

void ParticleEditorScene::Save(string file)
{
	BinaryWriter* data = new BinaryWriter(file);

	data->WriteData(quad->GetMaterial()->GetDiffuseMap()->GetPath());

	data->WriteData(&particleData, sizeof(ParticleData));

	delete data;
}

void ParticleEditorScene::Load(string file)
{
	BinaryReader* data = new BinaryReader(file);

	wstring diffusePath = data->ReadWString();

	quad->GetMaterial()->SetDiffuseMap(diffusePath);

	ParticleData* temp = new ParticleData();

	data->ReadData((void**)&temp, sizeof(ParticleData));

	particleData = *temp;

	particleCount = particleData.count; // ??

	delete temp;
	delete data;
}

void ParticleEditorScene::SaveDialog()
{
	if (ImGui::Button("Save"))
	{
		Dialog->OpenDialog("Save Particle", "Save", ".fx", "_TextData/");
	}

	if (Dialog->Display("Save Particle", 32, { 500, 400 }))
	{
		if (Dialog->IsOk())
		{
			string path = Dialog->GetFilePathName();

			path = path.substr(GetProjectDir().size(), path.length());

			Save(path);
		}

		Dialog->Close();
	}
}

void ParticleEditorScene::LoadDialog()
{
	if (ImGui::Button("Load"))
	{
		Dialog->OpenDialog("Load Particle", "Load", ".fx", "_TextData/");
	}

	if (Dialog->Display("Load Particle", 32, { 500, 400 }))
	{
		if (Dialog->IsOk())
		{
			string path = Dialog->GetFilePathName();

			path = path.substr(GetProjectDir().size(), path.length());

			Load(path);
		}

		Dialog->Close();
	}
}

void ParticleEditorScene::EditTexture()
{
	ImTextureID textureID = quad->GetMaterial()->GetDiffuseMap()->GetSRV();

	if (ImGui::ImageButton(textureID, ImVec2(50, 50)))
	{
		Dialog->OpenDialog("Select Texture", "Select Texture", ".png,.jpg,.tga", "_Texture/");
	}

	if (Dialog->Display("Select Texture", 32, { 500, 400 }))
	{
		if (Dialog->IsOk())
		{
			string path = Dialog->GetFilePathName();

			path = path.substr(GetProjectDir().size(), path.length());

			quad->GetMaterial()->SetDiffuseMap(ToWString(path));
		}

		Dialog->Close();
	}
}

#include "Framework.h"
#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(string file)
{
	Load(file);

	instanceData.resize(particleData.count);

	instanceBuffer = new VertexBuffer(instanceData);

	Init();
}

ParticleSystem::ParticleSystem(string file, Vector3 pos)
	:pos(pos)
{
	Load(file);

	instanceData.resize(particleData.count);

	instanceBuffer = new VertexBuffer(instanceData);

	Init();
}

ParticleSystem::~ParticleSystem()
{
	delete quad;
	delete instanceBuffer;
}

void ParticleSystem::Update()
{
	if (!IsActive())
		return;

	Transform::Update();

	lifeTime += Time::Delta();

	UpdatePhysics();
	UpdateColor();

	quad->Update();

	if (lifeTime > particleData.duration)
	{
		if (particleData.isLoop)
			Init();
		else
			Stop();
	}
}

void ParticleSystem::Render()
{
	if (!IsActive())
		return;

	Transform::SetWorld();

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

void ParticleSystem::Play(Vector3 pos, Vector3 rot)
{
	quad->IsActive() = true;

	quad->translation = pos;
	quad->rotation    = rot;

	Init();
}

void ParticleSystem::PlayWP(Vector3 rot)
{
	quad->IsActive() = true;

	quad->translation = pos;
	quad->rotation = rot;

	Init();
}

void ParticleSystem::Stop()
{
	quad->IsActive() = false;
}

void ParticleSystem::UpdatePhysics()
{
	drawCount = 0;

	for (ParticleInfo& info : particleInfos)
	{
		if (info.startTime > lifeTime)
			continue;

		info.velocity += info.acceleration * Time::Delta();

		info.transform.translation += info.velocity * info.speed * Time::Delta();

		info.transform.rotation.z += info.angularVelocity * Time::Delta();

	/*	if (particleData.isBillboard)
		{
			Vector3 rot;

			if (!Environment::GetInstance()->IsGameMode())
			{
				rot = CAMERA->rotation;
			}
			else
			{
				Matrix viewMatrix;
				if (!Environment::GetInstance()->IsFPmode())
					viewMatrix = CAMERATP->GetViewMatrix(); 
				else
					viewMatrix = CAMERAFP->GetViewMatrix();


				rot = ExtractEulerAngleFromMatrix(viewMatrix);
			}

			rot.z = info.transform.rotation.z;

			info.transform.rotation = rot;
			
		}
		*/

		if (particleData.isBillboard)
		{
			
			
			if (!Environment::GetInstance()->IsGameMode())
			{
				Vector3 rot;

				rot = CAMERA->rotation;

				rot.z = info.transform.rotation.z;

				info.transform.rotation = rot;
			}
			else
			{
				Vector3 cameraDirection;

				if (!Environment::GetInstance()->IsFPmode())
					cameraDirection = CAMERATP->CalculateDirection();
				else
					cameraDirection = CAMERAFP->CalculateDirection();


				Vector3 billboardRotation = CalculateRotationFromDirection(-cameraDirection);
				info.transform.rotation = billboardRotation;
			}

		}

		float timeScale = (lifeTime - info.startTime) / (particleData.duration - info.startTime);

		info.transform.scale = LERP(info.startScale, info.endScale, timeScale);

		info.transform.Update();

		//instanceData[drawCount].color.x = Random(0.0f, 1.0f);
		//instanceData[drawCount].color.y = Random(0.0f, 1.0f);
		//instanceData[drawCount].color.z = Random(0.0f, 1.0f);
		instanceData[drawCount++].transform = XMMatrixTranspose(info.transform.GetWorld() * world);
	}

	instanceBuffer->UpdateVertex(instanceData.data(), drawCount);
}




Vector3 ParticleSystem::CalculateRotationFromDirection(Vector3 direction)
{
	direction.Normalize();

	// Yaw 계산 - atan2 사용
	float yaw = atan2(direction.x, direction.z);

	// Pitch 계산
	float pitch = atan2(direction.y, sqrt(direction.x * direction.x + direction.z * direction.z));

	// Roll은 빌보드 처리에서 일반적으로 필요하지 않으므로 0으로 설정
	float roll = 0.0f;

	// 오일러 각도로 변환된 값을 반환
	return Vector3(pitch, yaw, roll);
}

void ParticleSystem::UpdateColor()
{
	float timeScale = lifeTime / particleData.duration;

	Vector4 color;
	color.x = LERP(particleData.startColor.x, particleData.endColor.x, timeScale);
	color.y = LERP(particleData.startColor.y, particleData.endColor.y, timeScale);
	color.z = LERP(particleData.startColor.z, particleData.endColor.z, timeScale);
	color.w = LERP(particleData.startColor.w, particleData.endColor.w, timeScale);

	quad->GetMaterial()->GetBuffer()->data.diffuse = color;
}

void ParticleSystem::Init()
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

void ParticleSystem::Load(string file)
{
	BinaryReader* data = new BinaryReader(file);

	wstring diffusePath = data->ReadWString();

	quad = new Quad();
	quad->GetMaterial()->SetShader(L"18Particle");
	quad->GetMaterial()->SetDiffuseMap(diffusePath);

	ParticleData* temp = new ParticleData();

	data->ReadData((void**)&temp, sizeof(ParticleData));

	particleData = *temp;

	particleCount = particleData.count; // ??

	delete temp;
	delete data;
}
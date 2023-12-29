#include "Framework.h"
#include "Grenade.h"

Grenade::Grenade()
	:Model("grenade")
{
	terrain = new Terrain(L"Landscape/Floor.png", L"HeightMap/AlphaMap.png");

	reader->GetMaterial()[0]->Load(L"grenade.mat");
	this->scale *= 0.1f;
	this->SetLabel("FireGrenade");
}

Grenade::~Grenade()
{
	delete terrain;
}

void Grenade::Update()
{
	if (!isFire)
		return;

	Model::Update();

	velocity += gravity * Time::Delta();

	if (isFire)
		this->translation += velocity * Time::Delta();


	Vector3 newPosition = this->translation + velocity * Time::Delta();


	float terrainHeight = terrain->GetHeight(newPosition) + 1.0f;

	if (newPosition.y < terrainHeight)
	{
		this->translation.y = terrainHeight;
		if (!isGround)
		{
			isGround = true;
			timeSinceGround = 0.0f;
		}
		else
		{
			timeSinceGround += Time::Delta();
			if (timeSinceGround >= 1.0f)
			{
				isFire = false;
				isGround = false;
				isBoom = true;
			}
		}
	}
	else
	{
		this->translation = newPosition;
	}

}

void Grenade::Render()
{
	if (!isFire && !isGround)
		return;


	Model::Render();

}

void Grenade::PostRender()
{
	Model::Debug();
}

void Grenade::Fire(Vector3 pos, Vector3 dir, Vector3 forwad, Vector3 up)
{
	this->translation = pos;
	this->rotation = dir;
	// 총알의 초기 속도 벡터를 캐릭터의 방향을 기준으로 설정합니다.
	// 예를 들어, 총알이 캐릭터의 오른쪽으로 그리고 위쪽으로 포물선을 그리게 하려면,
	// 캐릭터의 'up' 벡터와 'right' 벡터를 조합합니다.
	float upSpeed = 20.0f;     // 위쪽으로의 속도
	float forwardSpeed = 30.0f;   // 오른쪽으로의 속도
	this->velocity = forwad * upSpeed + up * forwardSpeed;
	gravity = Vector3(0.0f, -25.8f, 0.0f);

	isFire = true;
}


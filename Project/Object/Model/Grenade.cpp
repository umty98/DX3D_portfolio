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
	// �Ѿ��� �ʱ� �ӵ� ���͸� ĳ������ ������ �������� �����մϴ�.
	// ���� ���, �Ѿ��� ĳ������ ���������� �׸��� �������� �������� �׸��� �Ϸ���,
	// ĳ������ 'up' ���Ϳ� 'right' ���͸� �����մϴ�.
	float upSpeed = 20.0f;     // ���������� �ӵ�
	float forwardSpeed = 30.0f;   // ������������ �ӵ�
	this->velocity = forwad * upSpeed + up * forwardSpeed;
	gravity = Vector3(0.0f, -25.8f, 0.0f);

	isFire = true;
}


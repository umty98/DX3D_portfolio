#include "Framework.h"
#include "Smoke.h"

Smoke::Smoke()
	:Model("M18_Smoke")
{
	terrain = new Terrain(L"Landscape/Floor.png", L"HeightMap/AlphaMap.png");

	reader->GetMaterial()[0]->Load(L"smoke.mat");
	this->scale *= 0.2f;
//	this->rotation.x = XM_PIDIV2;
	this->SetLabel("FireSmoke");

	
}

Smoke::~Smoke()
{
	delete terrain;
}

void Smoke::Update()
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
				isSmoke = true;
			}
		}
	}
	else
	{
		this->translation = newPosition;
	}

}

void Smoke::Render()
{
	if (!isFire && !isGround)
		return;


	Model::Render();

}

void Smoke::PostRender()
{
	Model::Debug();
}

void Smoke::Fire(Vector3 pos, Vector3 dir, Vector3 forwad, Vector3 up)
{
	this->translation = pos;
	this->rotation = dir;
	// �Ѿ��� �ʱ� �ӵ� ���͸� ĳ������ ������ �������� �����մϴ�.
	// ���� ���, �Ѿ��� ĳ������ ���������� �׸��� �������� �������� �׸��� �Ϸ���,
	// ĳ������ 'up' ���Ϳ� 'right' ���͸� �����մϴ�.
	float upSpeed = 15.0f;     // ���������� �ӵ�
	float forwardSpeed = 30.0f;   // ������������ �ӵ�
	this->velocity = forwad * upSpeed + up * forwardSpeed;
	gravity = Vector3(0.0f, -30.8f, 0.0f);

	isFire = true;
}

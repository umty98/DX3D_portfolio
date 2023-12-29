#include "Framework.h"
#include "UsedBullet.h"

UsedBullet::UsedBullet()
	:Model("UsedBullet")
{

	terrain = new Terrain(L"Landscape/Floor.png", L"HeightMap/AlphaMap.png");

	//string name = "Riflemagazine";
	reader->GetMaterial()[0]->Load(L"UsedBullet.mat");
	//string name = "UsedBullet";
	//usedBullet = new Model(name);
	this->SetPivot({ -0.31f, 0.43f, 0.17f });
	//usedBullet->GetReader()->GetMaterial()[0]->Load(L"UsedBullet.mat");
	//usedBullet->SetLabel("usedbullet");
	this->rotation.y = XM_PIDIV2;
	this->scale *= 10.0f;
	this->SetLabel("Bullet");
}

UsedBullet::~UsedBullet()
{
	//delete usedBullet;
	delete terrain;
}

void UsedBullet::Update()
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
			}
		}
	}
	else
	{
		this->translation = newPosition;
	}



}

void UsedBullet::Render()
{
	if (!isFire && !isGround)
		return;

	//if (!isFire)
		//return;

	Model::Render();

	//usedBullet->Render();
}

void UsedBullet::PostRender()
{
	//usedBullet->Debug();
	Model::Debug();
}

void UsedBullet::Fire(Vector3 pos, Vector3 dir)
{
	this->translation = pos;
	this->rotation = dir;
	//velocity = Vector3(5.0f, 5.0f, 0.0f); 
	velocity = dir * initialSpeed;
	gravity = Vector3(0.0f, -9.8f, 0.0f);

	isFire = true;
	isGround = false;


}

void UsedBullet::Fire(Vector3 pos, Vector3 dir, Vector3 forward, Vector3 up, Vector3 right)
{
	this->translation = pos;
	this->rotation = dir;
	// 총알의 초기 속도 벡터를 캐릭터의 방향을 기준으로 설정합니다.
	// 예를 들어, 총알이 캐릭터의 오른쪽으로 그리고 위쪽으로 포물선을 그리게 하려면,
	// 캐릭터의 'up' 벡터와 'right' 벡터를 조합합니다.
	float upSpeed = 10.0f;     // 위쪽으로의 속도
	float rightSpeed = 5.0f;   // 오른쪽으로의 속도
	this->velocity = up * upSpeed + right * rightSpeed;
	gravity = Vector3(0.0f, -30.8f, 0.0f);

	isFire = true;
}

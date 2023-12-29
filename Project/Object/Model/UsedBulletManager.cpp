#include "Framework.h"
#include "UsedBulletManager.h"

UsedBulletManager::UsedBulletManager(UINT poolCount)
{
	for (UINT i = 0; i < poolCount; i++)
	{

		usedBullets.push_back(new UsedBullet);
	}

}

UsedBulletManager::~UsedBulletManager()
{
	for (UsedBullet* bullets : usedBullets)
		delete bullets;

	usedBullets.clear();
}

void UsedBulletManager::Update()
{
	for (UsedBullet* bullets : usedBullets)
		bullets->Update();
}

void UsedBulletManager::Render()
{
	for (UsedBullet* bullets : usedBullets)
	{
		bullets->Render();
	}
}

void UsedBulletManager::PostRender()
{
}

void UsedBulletManager::Fire(Vector3 pos, Vector3 dir, Vector3 forward, Vector3 up, Vector3 right)
{
	for (UsedBullet* bullets : usedBullets)
	{
		if (bullets->IsFire())
			continue;

		bullets->Fire(pos, dir, forward, up, right);
		return;

	}
}



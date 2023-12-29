#pragma once

class UsedBulletManager
{
public:
	UsedBulletManager(UINT poolCount);
	~UsedBulletManager();

	void Update();
	void Render();
	void PostRender();
	void Fire(Vector3 pos, Vector3 dir, Vector3 forward, Vector3 up, Vector3 right);

private:
	vector<UsedBullet*> usedBullets;

};


#pragma once

class UsedBullet : public Model
{
public:
	UsedBullet();
	~UsedBullet();

	void Update();

	void Render();
	void PostRender();

	//Model* GetTransform() { return usedBullet; }

	void Fire(Vector3 pos, Vector3 dir);
	void Fire(Vector3 pos, Vector3 dir, Vector3 forward, Vector3 up, Vector3 right);

	bool& IsFire() { return isFire; }

private:
	//string name;
	//Model* usedBullet;

	bool isFire = false;
	bool isGround = false;
	float timeSinceGround;


	Vector3 velocity;
	Vector3 gravity;
	float initialSpeed = 15.0f;

	Terrain* terrain;

};


#pragma once

class Smoke : public Model
{
public:
	Smoke();
	~Smoke();

	void Update();

	void Render();
	void PostRender();

	void Fire(Vector3 pos, Vector3 dir, Vector3 forwad, Vector3 up);

	bool& IsFire() { return isFire; }

	bool& IsSmoke() { return isSmoke; }


private:
	bool isFire = false;
	bool isGround = false;
	bool isSmoke = false;

	float timeSinceGround;

	Vector3 velocity;
	Vector3 gravity;
	float initialSpeed = 15.0f;

	Terrain* terrain;

};
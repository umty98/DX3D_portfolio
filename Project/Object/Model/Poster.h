#pragma once

class Poster
{
public:
	Poster();
	~Poster();

	void Update();

	void Render();
	void PostRender();

	float& GetMoveDirection() { return moveDirection; }
	float& GetMoveSpeed() { return moveSpeed; }

	bool& IsActive() { return isActive; }

	Transform* GetTransform() { return poster; }

	ColliderBox* GetPosterCollider() { return colliderbox; }

	//void Collision()

	bool& GetIsRayOn() { return isRayOn; }

private:
	Model* poster;

	float x, y;

	bool isActive = true;
	bool isRayOn = true;
	float moveDirection;
	float moveSpeed;

	ColliderBox* colliderbox;

};


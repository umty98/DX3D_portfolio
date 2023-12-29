#pragma once

class UsedBullet;
class UsedBulletManager;
class Grenade;
class Smoke;
class Poster;
class PosterManager;

class Swat : public ModelAnimator
{
	enum AnimState
	{
		IDLE_R, FORWARD_R, BACKWARD_R, ATTACK_R, RELOAD_R, RIGHT, LEFT,
		IDLE_P, FORWARD_P, BACKWARD_P, ATTACK_P, RELOAD_P,
		IDLE_T, THROW_R,   THROW_P,  THROW_S , THROW_E,
		GRAB_R, GRAB_P, GRAB_T,

	}curState = IDLE_R;

public:
	Swat();
	~Swat();

	void Update();
	void Render();

	void Debug();
	void PostRender();

	void UpdateRightHand();

	void SetClip(AnimState state);

	void SwitchWeapon();

	Vector3& GetTrans() { return translation; }

	//UsedBullet* GetUsedBullet() { return usedBullet; }

	bool& IsRifle()   { return isRifle; }
	bool& IsPistol()  { return isPistol; }
	bool& IsGrenade() { return isGrenade; }
	bool& IsSmoke()   { return isSmoke; }

	bool IsGrenadeColliderOn() { return grenadeColliderOn; }
	bool IsPosterRayOn() { return isPosterRayOn; }

	//Vector3& GetFireSmokePosition() { return fireSmokePosition; }
	//Vector3& GetFireParticlePosition() { return fireParticlePosition; }

	int& GetRifleBullet()       { return rifleBullet;       }
	int& GetPistolBullet()      { return pistolBullet;      }
	int& GetRifleTotalBullet()  { return rifleTotalBullet;  }
	int& GetPistolTotalBullet() { return pistolTotalBullet; }
	int& GetTotalGrenade()      { return totalGrenade    ; }
	int& GetTotalSmoke()        { return totalSmoke      ; }

	ColliderSphere* GetGrenadeCollider() { return throwGrenadeCollider; }

	void Collision(PosterManager* posters);
	void RayCollision(PosterManager* posters, Ray inputray);

	void Grab();

private:
	void Move();
	void Shoot();
	void Throw();

	void CreateWeapon();
	void CreateClips();
//	void RenderTrajectory(vector<Vector3>& trajectoryPoints);


private:
	float speed = 1.0f;
	float takeTime = 0.2f;

	//UsedBullet* usedBullet;
	UsedBulletManager* usedBullets;
	Grenade* throwGrenade;
	ColliderSphere* throwGrenadeCollider;
	Smoke*	 throwSmoke;
	ColliderBox* swatCollider;
	Poster* poster;


	Model* rifle;
	Model* riflemagazine;
	Model* grenade;
	Model* smoke;

	Model* pistol;

	Transform* righthand;

	float moveSpeed = 20.0f;
	float  rotSpeed = 3.0f;

	ProgressBar* hpBar;

	Vector3 bulletPosition;
	Vector3 bulletRotation;

	bool isRifle = true;
	bool isPistol = false;
	bool isGrenade = false;
	bool isSmoke = false;


	bool grenadeColliderOn = false;

	bool isThrowing;
	bool isPosterRayOn = true;
	double throwTimer = 0.0f;

	//ParticleSystem* fireParticle;
	//ParticleSystem* fireSmoke;

	Vector3 fireSmokePosition;
	Vector3 firePosition;
//	Vector3 throwPosition;
	//
//	Vector3 throwRotation;

	int rifleBullet = 30;
	int pistolBullet = 7;
	int rifleTotalBullet = 30;
	int pistolTotalBullet = 7;
	int totalGrenade = 1;
	int totalSmoke = 1;

	bool isPistolFired = false;

	ParticleSystem* fireParticle;
	ParticleSystem* fireSmoke;
	ParticleSystem* smokeParticle;
	ParticleSystem* boomParticle;

	double accumulatedTime = 0.0;
	double grenadeColliderTime = 0.0;
	//
	vector<Vector3> trajectoryPoints;
};


#pragma once

class ShootingScene : public Scene
{
public:
	ShootingScene();
	~ShootingScene();

	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;

	void TableObject();
	void SetUI();
	void WeaponUI();

	void SetButton();


	void SetTerrain();
	void SetFence();
	void SetTree();
	//void Set2DTree();


	void SetWater();
	void SetWaterParticle();


private:
	//Groot* groot;
	Swat* swat;
	//Poster* poster;
	PosterManager* posters;
	Terrain* terrain1;
	Terrain* terrain2;
	Terrain* terrain1R;
	Terrain* terrain1L;
	Quad* terrain1D;

	Water* water;

	vector<Fence*> fences;

	ParticleSystem* particle;
	vector<ParticleSystem*> particles;

	float particletime;

	ModelInstancing* tree;
	TreeBillboard* tree2DR;
	TreeBillboard* tree2DL;

	Model* table;
	ColliderBox* tableCollider;
	Model* grenade;
	ColliderBox* grenadeCollider;
	Model* smoke;
	ColliderBox* smokeCollider;
	Model* bulletsCase;
	ColliderBox* caseCollider;
	ColliderBox* swatCollider;

	Model* button;
	ColliderBox* startCollider;
	ColliderBox* stopCollider;

	/*float swatSpeed = 5.0f;
	float desiredX = 0.0f;
	float desiredZ = 0.0f;
	float swatHeightOffset = 2.0f;
	*/
	Quad* crossHair;
	Quad* rifleZoom;
	Quad* pistolZoom;

	SkyBox* sky;

	//UsedBullet* usedBullet;

	bool isZoomIn = false;
	bool isGrenadeOnRay = false;
	bool isCaseOnRay = false;
	bool isSmokeOnRay = false;
	bool isRayOnStart = false;
	bool isRayOnStop = false;

	

	float distance;

	ParticleSystem* grabParticle;
	//ParticleSystem* fireSmoke;
	Vector3 previousSwatPosition;
	float angle;

	Quad* rifleUI;
	Quad* pistolUI;
	Quad* grenadeUI;
	Quad* smokeUI;
	Quad* rectUI;



};


#pragma once

class SampleMap : public Scene
{
public:
	SampleMap();
	~SampleMap();

	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;

	void CreateFence();
	void CreateWater();


private:
	Terrain* terrain1;
	Terrain* terrain2;

	Water* water;

	SkyBox* sky;

	//Fence* fence;

	vector<Fence*> fences;

	ParticleSystem* particle;
	vector<ParticleSystem*> particles;

	float particletime;

	Tree* tree;
	//ModelInstancing* tree;
};


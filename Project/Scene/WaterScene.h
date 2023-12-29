#pragma once

class WaterScene : public Scene
{
public:
	WaterScene();
	~WaterScene();

	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;

private:
	void CreateObjects();

private:
	Quad* floor;
	Groot* groot;
	Model* bunny;
	Sphere* sphere;

	Reflection* reflection;
	Refraction* refraction;

	SkyBox* sky;

	Water* water;

	Shadow* shadow;
};

#pragma once

class ModelAnimationScene : public Scene
{
public:
	ModelAnimationScene();
	~ModelAnimationScene();

	virtual void Update() override;

	virtual void PreRender() override;

	virtual void Render() override;

	virtual void PostRender() override;
private:
	Groot* groot;

	Terrain* terrain;

	Quad* crossHair;

	SkyBox* sky;
};

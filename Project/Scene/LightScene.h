#pragma once

class LightScene : public Scene
{
public:
	LightScene();
	~LightScene();

	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;

private:
	void CreateObjects();

private:
	Quad* floor2;
	Quad* floor;
	Terrain* terrain;

	Groot* groot;

	Model* bunny;

	Sphere* sphere;

	DepthStencil* depthStencil;
	RenderTarget* renderTarget;

	FloatValueBuffer* buffer;
};

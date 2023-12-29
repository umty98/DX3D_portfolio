#pragma once

class DeferredRenderScene : public Scene
{
public:
	DeferredRenderScene();
	~DeferredRenderScene();

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

	GeometryBuffer* gBuffer;

	Material*     material;
	VertexBuffer* vertexBuffer;
};

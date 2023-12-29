#pragma once

class Reflection
{
public:
	Reflection(Transform* target);
	~Reflection();

	void Update();

	void SetPreRender();
	void SetRender();

	void PostRender();

private:
	Transform* target;

	RenderTarget* renderTarget;
	DepthStencil* depthStencil;

	Camera* camera;

	Quad* quad;

	static UINT index;
};

#pragma once

class Refraction
{
public:
	Refraction(wstring file);
	~Refraction();

	void Update();

	void SetPreRender();
	void SetRender();

	void PostRender();
	void Debug();

private:
	RenderTarget* renderTarget;
	DepthStencil* depthStencil;

	Texture* normalMap;

	static UINT index;

	Quad* quad;

	WaterBuffer* buffer;
};

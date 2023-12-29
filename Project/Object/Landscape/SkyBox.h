#pragma once

class SkyBox
{
public:
	SkyBox(wstring file);
	~SkyBox();

	void Render();

	void PostRender();

private:
	Sphere* sphere;

	RasterizerState* rs;

	Texture* cubeMap;
};

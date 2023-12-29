#include "Framework.h"
#include "Refraction.h"

UINT Refraction::index = 0;

Refraction::Refraction(wstring file)
{
	buffer = new WaterBuffer();

	normalMap = Texture::Get(file);

	renderTarget = new RenderTarget(1280, 720);
	depthStencil = new DepthStencil(1280, 720);
	//Debug

	quad = new Quad(Vector2(200, 200));
	quad->translation = { 100, 100, 0 };

	Texture* texture = Texture::Get(L"Refraction_" + to_wstring(index++), renderTarget->GetSRV());

	quad->GetMaterial()->SetDiffuseMap(texture);
	quad->UpdateWorld();
}

Refraction::~Refraction()
{
	delete buffer;
	delete renderTarget;
	delete depthStencil;
	delete quad;
}

void Refraction::Update()
{
	buffer->data.waveTime += Time::Delta();
}

void Refraction::SetPreRender()
{
	renderTarget->Set(depthStencil);
}

void Refraction::SetRender()
{
	buffer->SetPSBuffer(10);

	       DC->PSSetShaderResources(11, 1, &renderTarget->GetSRV());
	normalMap->PSSetShaderResources(12);
}

void Refraction::PostRender()
{
	quad->Render();
}

void Refraction::Debug()
{
	ImGui::Text("Water Option");
	ImGui::ColorEdit4("WaveColor", (float*)&buffer->data.color);

	ImGui::SliderFloat("WaveSpeed    ", (float*)&buffer->data.waveSpeed    , 0.0f,  1.0f);
	ImGui::SliderFloat("WaveScale    ", (float*)&buffer->data.waveScale    , 0.0f,  1.0f);
	ImGui::SliderFloat("WaveShininess", (float*)&buffer->data.waveShininess, 1.0f, 50.0f);
	ImGui::SliderFloat("WaveFresnel  ", (float*)&buffer->data.fresnel      , 0.0f,  1.0f);

}

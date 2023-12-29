#include "Framework.h"
#include "Reflection.h"

UINT Reflection::index = 0;

Reflection::Reflection(Transform* target)
	:target(target)
{
	camera = new Camera();

	renderTarget = new RenderTarget(1280, 720);
	depthStencil = new DepthStencil(1280, 720);
	//Debug

	quad = new Quad(Vector2(200, 200));
	quad->translation = { 100, 100, 0 };

	Texture* texture = Texture::Get(L"Reflection_" + to_wstring(index++), renderTarget->GetSRV());

	quad->GetMaterial()->SetDiffuseMap(texture);
	quad->UpdateWorld();
}

Reflection::~Reflection()
{
	delete renderTarget;
	delete depthStencil;
	delete camera;
	delete quad;
}

void Reflection::Update()
{
	Vector3 pos = CAMERA->translation;
	Vector3 rot = CAMERA->rotation;

	pos.y = target->GetGlobalPosition().y * 2.0f - pos.y;
	rot.x *= -1.0f;

	camera->translation = pos;
	camera->rotation    = rot;

	camera->UpdateWorld();
}

void Reflection::SetPreRender()
{
	renderTarget->Set(depthStencil);

	camera->SetView(); //Slot 1 ม฿บน
}

void Reflection::SetRender()
{
	camera->GetViewBuffer()->SetVSBuffer(10);
	DC->PSSetShaderResources(10, 1, &renderTarget->GetSRV());
}

void Reflection::PostRender()
{
	quad->Render();

	quad->Debug();
}

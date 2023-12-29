#include "Framework.h"
#include "Particle.h"

Particle::Particle()
{
	material = new Material();
}

Particle::~Particle()
{
	delete material;
	delete vertexBuffer;
}

void Particle::Render()
{
	if (!isActive)
		return;

	if (isAdditive)
		STATE->AdditiveBegin();
	else
		STATE->AlphaBegin();

	STATE->DepthWriteMaskZero();

	vertexBuffer->IASetBuffer(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	      material->SetMaterial();
	geometryShader->SetShader();

	DC->Draw(drawCount, 0);

	DC->GSSetShader(nullptr, nullptr, 0);

	STATE->AlphaEnd();
	STATE->DepthEnable();
}

void Particle::Play(Vector3 position)
{
	isActive = true;

	this->position = position;
}

void Particle::Stop()
{
	isActive = false;
}

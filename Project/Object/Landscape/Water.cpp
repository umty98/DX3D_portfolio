#include "Framework.h"
#include "Water.h"

Water::Water(wstring normalFile, float width, float height)
	:size(width, height)
{
	label = "Water_" + to_string(index);

	reflection = new Reflection(this);
	refraction = new Refraction(normalFile);

	CreateMesh();

	mesh = new Mesh(vertices, indices);
	material = new Material();
	material->SetShader(L"21Water");
}

Water::~Water()
{
	delete reflection;
	delete refraction;

	delete mesh;
	delete material;
}

void Water::Update()
{
	Transform::Update();

	reflection->Update();
	refraction->Update();
}

void Water::Render()
{
	Transform::SetWorld();

	mesh->SetMesh();
	material->SetMaterial();

	reflection->SetRender();
	refraction->SetRender();

	STATE->AlphaBegin();
	DC->DrawIndexed(indices.size(), 0, 0);
	STATE->AlphaEnd();
}

void Water::Debug()
{
	Transform::Debug();
	refraction->Debug();
}

void Water::SetReflection()
{
	reflection->SetPreRender();
}

void Water::SetRefraction()
{
	refraction->SetPreRender();
}

void Water::CreateMesh()
{
	float L = -size.x * 0.5f;
	float R = +size.x * 0.5f;
	float B = -size.y * 0.5f;
	float T = +size.y * 0.5f;

	vertices =
	{
		{ Vector3(L, 0, T), Vector2(0, 0) },
		{ Vector3(R, 0, T), Vector2(1, 0) },
		{ Vector3(L, 0, B), Vector2(0, 1) },
		{ Vector3(R, 0, B), Vector2(1, 1) },
	};

	indices =
	{
		0, 1, 2,
		2, 1, 3
	};
}

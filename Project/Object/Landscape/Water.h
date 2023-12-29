#pragma once

class Water : public Transform
{
public:
	Water(wstring normalFile, float width = 100.0f, float height = 100.0f);
	~Water();

	void Update();
	void Render();
	void Debug();

	void SetReflection();
	void SetRefraction();

private:
	void CreateMesh();

private:
	Vector2 size;

	Mesh* mesh;
	Material* material;

	vector<VertexTexture> vertices;
	vector<UINT>           indices;

	Reflection* reflection;
	Refraction* refraction;
};

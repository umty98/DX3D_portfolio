#pragma once

class Terrain : public Transform
{
	typedef VertexTextureNormalTangent VertexType;
public:
	Terrain(wstring diffuseFile, wstring heightFile);
	~Terrain();

	void Render();

	float GetHeight(Vector3 position);
	float GetX() { return width * scale.x; }
	float GetY() { return height * scale.z; }

	Vector2 GetSize() { return Vector2(width, height); }

private:
	void CreateMesh();
	void CreateNormal();
	void CreateTangent();

private:
	vector<VertexType> vertices;
	vector<UINT>        indices;

	Material* material;
	Mesh*     mesh;

	UINT  width = 10;
	UINT height = 10;

	Texture* heightMap;

	const float MAP_HEIGHT = 20.0f;
};

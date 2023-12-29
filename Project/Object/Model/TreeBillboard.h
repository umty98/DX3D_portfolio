#pragma once

class TreeBillboard
{
public:  //, Vector3 scale = {1.0f, 1.0f, 1.0f},Vector3 translation = {0.0f,0.0f,0.0f}, Vector3 rotation = { 0.0f,0.0f,0.0f}
	TreeBillboard(UINT COUNT, Terrain* terrain, Vector3 scale = { 1.0f, 1.0f, 1.0f }, Vector3 translation = { 0.0f,0.0f,0.0f }, Vector3 rotation = { 0.0f,0.0f,0.0f });
	~TreeBillboard();

	void Update();
	void PreRender();
	void Render();
	void PostRender();


private:
	vector<VertexTexture> vertices;
	vector<VertexTexture> drawVertices;

	Material* material;
	VertexBuffer* vertexBuffer;
	GeometryShader* geometryShader;

	UINT COUNT;

	Vector3 scale;
	Vector3 translation;
	Vector3 rotation;
};


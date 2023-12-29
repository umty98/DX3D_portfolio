#pragma once

class ModelMesh
{
	friend class ModelReader;
public:
	ModelMesh();
	~ModelMesh();

	void Create();

	void Render();
	void RenderInstanced(UINT instanceCount);

private:
	string name;

	//UINT materialIndex;

	Material* material;

	Mesh* mesh;

	vector<ModelVertex> vertices;
	vector<UINT>         indices;
};

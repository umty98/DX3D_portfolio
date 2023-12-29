#pragma once

class BillboardScene : public Scene
{
public:
	BillboardScene();
	~BillboardScene();

	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;

private:
	//vector<Quad*> trees;
	Terrain* terrain;

	vector<VertexTexture> vertices;
	vector<VertexTexture> drawVertices;

	Material*       material;
	VertexBuffer*   vertexBuffer;
	GeometryShader* geometryShader;

	const UINT COUNT = 1000;
	
	UINT drawCount = COUNT;
};

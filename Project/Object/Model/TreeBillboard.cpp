#include "Framework.h"
#include "TreeBillboard.h"
// Vector3 scale, Vector3 translation, Vector3 rotation
TreeBillboard::TreeBillboard(UINT COUNT, Terrain* terrain, Vector3 scale, Vector3 translation, Vector3 rotation)
	:COUNT(COUNT), scale(scale), translation(translation), rotation(rotation)
{
	
	material = new Material();
	material->SetShader(L"13Billboard");
	material->SetDiffuseMap(L"Landscape/Tree.png");

	geometryShader = Shader::GetGS(L"13Billboard");


	for (UINT i = 0; i < COUNT; i++)
	{
		Vector2 size;

		size.x = Random(30.0f, 70.0f);
		size.y = Random(30.0f, 70.0f);


		Vector3 pos;
		pos.x = Random(1.0f, terrain->GetSize().x*scale.x-1.0f);
		pos.x += translation.x;
		pos.z = Random(1.0f, terrain->GetSize().y*scale.z-1.0f);
		pos.z += translation.z;

		//pos.y = terrain->GetHeight(pos) + size.y * 0.5f;
		pos.y = terrain->GetHeight(Vector3(pos.x / scale.x - translation.x, 0, pos.z / scale.z - translation.z)) * scale.y + translation.y + size.y * 0.5f;
		pos.y += translation.y;

		VertexTexture vertex;
		vertex.pos = pos;
		vertex.uv = size;

		vertices.push_back(vertex);
	}


	drawVertices.resize(COUNT);

	vertexBuffer = new VertexBuffer(vertices);
}

TreeBillboard::~TreeBillboard()
{
	delete material;
	delete vertexBuffer;
}

void TreeBillboard::Update()
{

}

void TreeBillboard::PreRender()
{
}

void TreeBillboard::Render()
{
	//StateManager::GetInstance()->AlphaBegin();
	StateManager::GetInstance()->AlphaToCoverageEnable();
	//StateManager::GetInstance()->DepthWriteMaskZero();

	//for (Quad* tree : trees)
	//	tree->Render();

	vertexBuffer->IASetBuffer(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	material->SetMaterial();
	geometryShader->SetShader();

	DC->Draw(COUNT, 0);

	DC->GSSetShader(nullptr, nullptr, 0);

	StateManager::GetInstance()->AlphaEnd();
	//StateManager::GetInstance()->DepthEnable();
}

void TreeBillboard::PostRender()
{
}

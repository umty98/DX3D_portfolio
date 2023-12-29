#include "Framework.h"
#include "CollisionScene.h"

CollisionScene::CollisionScene()
{
	for (UINT i = 0; i < 2; i++)
	{
		colliders.push_back(new ColliderSphere);
		colliders[i]->SetLabel("ColliderSphere" + to_string(i));
		colliders[i]->translation.x = 3 * i;
	}

	for (UINT i = 2; i < 4; i++)
	{
		colliders.push_back(new ColliderBox);
		colliders[i]->SetLabel("ColliderBox" + to_string(i));
		colliders[i]->translation.x = 3 * i;
	}
}

CollisionScene::~CollisionScene()
{
	for (Collider* collider : colliders)
		delete collider;

	colliders.clear();
}

void CollisionScene::Update()
{
	for (Collider* collider : colliders)
		collider->Update();

	Ray ray = CAMERA->ScreenPointToRay(mousePos);

	static Contact contact = {};

	if (colliders[2]-> Collision(ray, &contact))
		colliders[2]->SetColor(1, 0, 0);
	else
		colliders[2]->SetColor(0, 1, 0);

	//colliders[1]->translation = contact.hitPoint;
}

void CollisionScene::PreRender()
{
}

void CollisionScene::Render()
{
	for (Collider* collider : colliders)
		collider->Render();
}

void CollisionScene::PostRender()
{
	for (Collider* collider : colliders)
		collider->Debug();
}

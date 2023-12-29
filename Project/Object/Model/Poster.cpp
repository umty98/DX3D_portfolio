#include "Framework.h"
#include "Poster.h"

Poster::Poster()
//:x(x),y(y)
{
	string name = "Poster";
	poster = new Model(name);

	colliderbox = new ColliderBox;
	colliderbox->SetParent(poster);
	colliderbox->SetLabel("Collider");
	colliderbox->translation.y = 4.3f;
	colliderbox->scale.y = 15.72f;
	colliderbox->scale.z = 11.63f;
	//poster->translation.x = 10;
	//poster->translation.y = y;
	//poster->rotation.x = -XM_PIDIV2;
	poster->GetReader()->GetMaterial()[0]->Load(L"Poster.mat");
	poster->SetLabel("Poster");


}

Poster::~Poster()
{
	delete poster;
	delete colliderbox;

}

void Poster::Update()
{
	if (!isActive)
		return;
	/*
	if (Environment::GetInstance()->IsGameMode())
	{
		Ray ray;

		if (!Environment::GetInstance()->IsFPmode())
			ray = CAMERATP->ScreenPointToRayLookAt(mousePos);

		else
			ray = CAMERAFP->ScreenPointToRayLookAt(mousePos);
		
		static Contact contact = {};

		

		if (isRayOn && KEY_DOWN(VK_LBUTTON))
		{
			if (colliderbox->Collision(ray, &contact))
			{
				colliderbox->SetColor(1, 0, 0);
				isActive = false;
				poster->translation = { 0.0f,0.0f,0.0f };
			}
			else
				colliderbox->SetColor(0, 1, 0);
		}

	}
	*/

	poster->Update();
	colliderbox->Update();
}

void Poster::Render()
{
	if (!isActive)
		return;

	poster->Render();
	colliderbox->Render();
}

void Poster::PostRender()
{
	poster->Debug();
	colliderbox->Debug();
}



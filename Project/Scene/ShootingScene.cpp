#include "Framework.h"
#include "ShootingScene.h"

ShootingScene::ShootingScene()
{
	//groot = new Groot();
	//CAMERA->translation = { -204.0f, 106.0f, -249.0f };
	//CAMERA->rotation = { 0.206f,0.799f,0.000f };
	
	sky = new SkyBox(L"Landscape/Snow_ENV.dds");

	SetTerrain();
	SetWater();
	SetWaterParticle();
	SetFence();
	SetTree();
	//{1.0f,1.0f,1.0f},{0.0f,0.0f,255.0f}
	tree2DR = new TreeBillboard(20, terrain1R, { 1.0f,1.0f,1.0f }, { 0.0f,0.0f,255.0f });
	tree2DL = new TreeBillboard(20, terrain1L, { 1.0f,1.0f,1.0f }, { 0.0f,0.0f,-255.0f });

	float centerX = terrain1->GetX();
	float centerZ = terrain1->GetY() / 2.0f;

	swat = new Swat();
	swat->scale *= 0.2f;
	swatCollider = new ColliderBox;
	swatCollider->SetParent(swat);
	swatCollider->SetLabel("SwatCollider");
	swatCollider->scale = { 50.0f,160.0f,50.0f };
	swatCollider->translation = { 0.0f,80.0f,-2.93f };

	swat->translation = { centerX,0, centerZ };

	//usedBullet = new UsedBullet();
	//usedBullet->GetTransform()->translation = swat->translation;

	//poster = new Poster();
	posters = new PosterManager(20, terrain1);


	CAMERATP->SetTarget(swat);
	CAMERAFP->SetTarget(swat);

	WeaponUI();
	SetUI();
	TableObject();
	SetButton();

	grabParticle = new ParticleSystem("Grab.fx");

	SoundManager::GetInstance()->AddSound("grabSound", "_Sound/grabSound.mp3", false);
	SoundManager::GetInstance()->AddSound("buttonSound", "_Sound/buttonSound.mp3", false);

	
}

ShootingScene::~ShootingScene()
{
	

	//Delete Terrain
	delete terrain1;
	delete terrain2;
	delete terrain1R;
	delete terrain1L;
	delete terrain1D;

	delete tree;
	delete tree2DR;
	delete tree2DL;

	for (Fence* fence : fences)
		delete fence;
	fences.clear();

	for (ParticleSystem* particle : particles)
		delete particle;
	particles.clear();

	//Delete Model
	delete posters;
	delete swat;
	delete water;
	//Delete UI
	delete crossHair;
	delete rifleZoom;
	delete pistolZoom;
	delete sky;
	//Delete TableObject
	delete table;
	delete tableCollider;
	delete grenade;
	delete grenadeCollider;
	delete smoke;
	delete smokeCollider;
	delete bulletsCase;
	delete caseCollider;
	delete swatCollider;
	//Delete Buttons
	delete button;
	delete startCollider;
	delete stopCollider;
	//Delete Particle
	delete grabParticle;
	//Delete WeaponUI
	delete rifleUI;
	delete pistolUI;
	delete grenadeUI;
	delete smokeUI;
	delete rectUI;

	
}

void ShootingScene::Update()
{
	//groot->Update();
	water          ->Update();
	swat           ->Update();
	table          ->Update();
	tableCollider  ->Update();
	grenade        ->Update();
	grenadeCollider->Update();
	smoke          ->Update();
	smokeCollider  ->Update();
	bulletsCase    ->Update();
	caseCollider   ->Update();
	swatCollider   ->Update();

	button		   ->Update();
	startCollider  ->Update();
	stopCollider   ->Update();

	posters        ->Update();
	terrain1       ->Update();
	terrain2       ->Update();
	terrain1R      ->Update();
	terrain1L      ->Update();
	terrain1D	   ->Update();

	tree		   ->Update();
	tree2DR->Update();
	tree2DL->Update();

	for (Fence* fence : fences)
		fence->Update();

	if (particletime > 4.0f)
	{

		//particle->Play({ -48.470f, 30.0f, 130.0f });
		for (ParticleSystem* particle : particles)
		{
			if (particle != nullptr) {
				particle->PlayWP();
			}

		}


		particletime = 0.0f;
	}
	else
	{
		particletime += Time::Delta();
	}


	for (ParticleSystem* particle : particles)
	{
		if (particle != nullptr) {
			particle->Update();
		}
	}

	crossHair      ->Update();
	rifleZoom      ->Update();
	pistolZoom     ->Update();

	rifleUI->Update();
	pistolUI->Update();
	grenadeUI->Update();
	smokeUI->Update();
	rectUI->Update();

	swat->translation.y = terrain1->GetHeight(swat->GetGlobalPosition());

	crossHair->translation.x = mousePos.x;
	crossHair->translation.y = WIN_HEIGHT - mousePos.y;

	rifleZoom->translation.x = mousePos.x - 120.0f;
	rifleZoom->translation.y = WIN_HEIGHT - mousePos.y - 123.0f;

	pistolZoom->translation.x = mousePos.x + 250.0f;
	pistolZoom->translation.y = WIN_HEIGHT - mousePos.y - 320.0f;

	
	if (Environment::GetInstance()->IsGameMode())
	{
		Ray ray;

		if (!Environment::GetInstance()->IsFPmode())
		{
			ray = CAMERATP->ScreenPointToRayLookAt(mousePos);
		}
		else
		{
			ray = CAMERAFP->ScreenPointToRayLookAt(mousePos);
		}

		ray.direction = ray.direction.GetNormalized();

		static Contact contact = {};

		distance = Distance(table->translation, swat->translation);

		if (distance <= 35.0f)
		{
			if (caseCollider->Collision(ray, &contact))
			{
				isCaseOnRay = true;
				caseCollider->SetColor(1, 0, 0);
				if (KEY_DOWN('F'))
				{
					SoundManager::GetInstance()->Play("grabSound", 1.95f);
					swat->Grab();
					grabParticle->Play(contact.hitPoint);
					
					if (swat->IsRifle())
						swat->GetRifleTotalBullet() += 30;
					else if (swat->IsPistol())
						swat->GetPistolTotalBullet() += 7;

				}
			}
			else
			{
				isCaseOnRay = false;
				caseCollider->SetColor(0, 1, 0);
			}
			if (grenadeCollider->Collision(ray, &contact))
			{
				isGrenadeOnRay = true;
				grenadeCollider->SetColor(1, 0, 0);
				if (KEY_DOWN('F'))
				{
					SoundManager::GetInstance()->Play("grabSound", 1.95f);
					swat->Grab();
					grabParticle->Play(contact.hitPoint);

					swat->GetTotalGrenade()++;
				}
			}
			else
			{
				isGrenadeOnRay = false;
				grenadeCollider->SetColor(0, 1, 0);
			}

			if (smokeCollider->Collision(ray, &contact))
			{
				isSmokeOnRay = true;
				smokeCollider->SetColor(1, 0, 0);
				if (KEY_DOWN('F'))
				{
					SoundManager::GetInstance()->Play("grabSound", 1.95f);
					swat->Grab();
					grabParticle->Play(contact.hitPoint);

					swat->GetTotalSmoke()++;
				}
			}
			else
			{
				isSmokeOnRay = false;
				smokeCollider->SetColor(0, 1, 0);
			}
		}
		else
		{
			isCaseOnRay = false;
			isGrenadeOnRay = false;
			isSmokeOnRay = false;
			caseCollider->SetColor(0, 1, 0);
			grenadeCollider->SetColor(0, 1, 0);
			smokeCollider->SetColor(0, 1, 0);
		}

		if (startCollider->Collision(ray, &contact))
		{
			isRayOnStart = true;
			startCollider->SetColor(1, 0, 0);
			if (KEY_DOWN(VK_LBUTTON))
			{
				SoundManager::GetInstance()->Play("buttonSound", 0.75f);
				grabParticle->Play(contact.hitPoint);
				posters->GetIsPlay() = true;
			}
		}
		else
		{
			isRayOnStart = false;
			startCollider->SetColor(0, 1, 0);
		}

		if (stopCollider->Collision(ray, &contact))
		{
			isRayOnStop = true;
			stopCollider->SetColor(1, 0, 0);
			if (KEY_DOWN(VK_LBUTTON))
			{
				SoundManager::GetInstance()->Play("buttonSound", 0.75f);
				grabParticle->Play(contact.hitPoint);
				posters->GetIsPlay() = false;
				posters->SetAllPosterFalse();
			}
		}
		else
		{
			isRayOnStop = false;
			stopCollider->SetColor(0, 1, 0);
		}




		if(swat->IsGrenadeColliderOn())
			swat->Collision(posters);

		if(swat->IsPosterRayOn())
			swat->RayCollision(posters, ray);

		grabParticle->Update();
	}

	///////////////////////////////////////////////////////////////
	
	Vector3 movementDirection = swat->translation - previousSwatPosition;
	Vector3 tableToSwatDirection = swat->translation - table->translation;
	angle = CalculateAngleBetweenVectors(tableToSwatDirection, movementDirection);
	if (swatCollider->Collision(tableCollider)) {
		

		if (angle > SOME_THRESHOLD) {
			// table 방향으로의 이동 차단
			swat->translation = previousSwatPosition;
		}

		swatCollider->SetColor(1, 0, 0);
	}
	else {
		swatCollider->SetColor(0, 1, 0);
		previousSwatPosition = swat->translation;
	}

	////WeaponUIBOX
	if (swat->IsRifle() || swat->IsPistol())
	{
		rectUI->scale       = { 320.0f, 110.0f, 0.0f };
		rectUI->translation = { 150.0f,  55.0f, 0.0f };
	}
	else
	{
		rectUI->scale       = { 170.0f, 190.0f, 0.0f };
		rectUI->translation = { 100.0f,  62.0f, 0.0f };
	}




}

void ShootingScene::PreRender()
{
	water->SetReflection();
	sky->Render();
	terrain1->Render();
	terrain2->Render();

	water->SetRefraction();
	sky->Render();
	terrain1->Render();
	terrain2->Render();

	
}

void ShootingScene::Render()
{
	sky->Render();

	water->Render();

	posters ->Render();
	terrain1->Render();
	terrain2->Render();
	terrain1R->Render();
	terrain1L->Render();
	terrain1D->Render();

	tree    ->Render();
	tree2DR->Render();
	tree2DL->Render();

	for (Fence* fence : fences)
		fence->Render();

	for (ParticleSystem* particle : particles)
		particle->Render();

	swat           ->Render();
	table          ->Render();
	tableCollider  ->Render();
	grenade        ->Render();
	grenadeCollider->Render();
	smoke          ->Render();
	smokeCollider  ->Render();
	bulletsCase    ->Render();
	caseCollider   ->Render();
	swatCollider   ->Render();

	button         ->Render();
	startCollider  ->Render();
	stopCollider   ->Render();

	grabParticle   ->Render();


}

void ShootingScene::PostRender()
{
	//groot->Debug();
	//usedBullet->PostRender();
	/*
	posters->PostRender();
	swat->PostRender();
	table->Debug();
	tableCollider->Debug();
	grenade->Debug();
	grenadeCollider->Debug();
	smoke->Debug();
	smokeCollider->Debug();
	bulletsCase->Debug();
	caseCollider->Debug();
	swatCollider->Debug();
	tree->Debug();

	button->Debug();
	startCollider->Debug();
	stopCollider->Debug();

	rifleUI->Debug();
	rectUI->Debug();

	terrain1R->Debug();
	terrain1L->Debug();
	terrain1D->Debug();
	*/
	//water->Debug();

	if (Environment::GetInstance()->IsGameMode())
	{
		rectUI->Render();
		
		if (swat->IsRifle())
		{
			wstring str = to_wstring(swat->GetRifleBullet()) + L" / " + to_wstring(swat->GetRifleTotalBullet());
			rifleUI->Render();
			FONT->RenderText(str, "D2Coding", Vector2(150, 650));
		}
		else if (swat->IsPistol())
		{
			wstring str = to_wstring(swat->GetPistolBullet()) + L" / " + to_wstring(swat->GetPistolTotalBullet());
			pistolUI->Render();
			FONT->RenderText(str, "D2Coding", Vector2(150, 650));
		}
		else if (swat->IsGrenade())
		{
			wstring str = to_wstring(swat->GetTotalGrenade());
			grenadeUI->Render();
			FONT->RenderText(str, "D2Coding", Vector2(150, 650));
		}
		else if (swat->IsSmoke())
		{
			wstring str = to_wstring(swat->GetTotalSmoke());
			smokeUI->Render();
			FONT->RenderText(str, "D2Coding", Vector2(150, 650));
		}
	}


	if (isGrenadeOnRay)
	{
		wstring str = L"PRESS 'F' TO GET GRENADE";
		FONTBLINK->RenderText(str, "D2Coding", Vector2(450, 360));
	}
	if(isCaseOnRay)
	{
		wstring str = L"PRESS 'F' TO GET AMMO";
		FONTBLINK->RenderText(str, "D2Coding", Vector2(450, 360));
		
	}
	if (isSmokeOnRay)
	{
		wstring str = L"PRESS 'F' TO GET SMOKE";
		FONTBLINK->RenderText(str, "D2Coding", Vector2(450, 360));
	}
	if (isRayOnStart)
	{
		wstring str = L"SHOOT BUTTON TO START TARGET MOVE";
		FONTBLINK->RenderText(str, "D2Coding", Vector2(350, 360));
	}
	if (isRayOnStop)
	{
		wstring str = L"SHOOT BUTTON TO STOP TARGET MOVE";
		FONTBLINK->RenderText(str, "D2Coding", Vector2(350, 360));
	}


	//ImGui::DragFloat3("Translation", (float*)&previousSwatPosition, 0.01f, -WIN_WIDTH, WIN_WIDTH);

	//ImGui::Text("Angle: %f", angle);



	if (Environment::GetInstance()->IsGameMode())
	{
		if (KEY_PRESS(VK_RBUTTON)&&Environment::GetInstance()->IsFPmode())
		{
			if (swat->IsRifle())
			{
				rifleZoom->Render();
			}
			else if(swat->IsPistol())
			{
				pistolZoom->Render();
			}
		}
		else
			crossHair->Render();
	}

}


void ShootingScene::TableObject()
{
	table = new Model("wooden_table");
	table->GetReader()->GetMaterial()[0]->Load(L"wooden_table.mat");
	table->scale = { 0.2f,0.2f,0.15f };
	table->rotation = { XM_PIDIV2, 0.0f,XM_PIDIV2 };
	table->translation = { 210.0f,7.54f,20.40f };
	table->SetLabel("Table");
	tableCollider = new ColliderBox;
	tableCollider->SetParent(table);
	tableCollider->scale = { 73.0f,185.0f,89.9f };
	tableCollider->SetLabel("TableCollider");

	grenade = new Model("grenade");
	grenade->GetReader()->GetMaterial()[0]->Load(L"grenade.mat");
	grenade->scale *= 0.2f;
	grenade->translation = { 196.85f,15.40f,20.40f };
	grenade->SetLabel("TGrenade");
	grenadeCollider = new ColliderBox;
	grenadeCollider->SetParent(grenade);
	grenadeCollider->scale *= 13.0f;
	grenadeCollider->translation.y = 1.91f;
	grenadeCollider->SetLabel("GrenadeCollider");

	smoke = new Model("M18_Smoke");
	smoke->GetReader()->GetMaterial()[0]->Load(L"smoke.mat");
	smoke->scale *= 0.2f;
	smoke->rotation.x = XM_PIDIV2;
	smoke->translation = { 202.67f,15.47f,20.40f };
	smoke->SetLabel("TSmoke");
	smokeCollider = new ColliderBox;
	smokeCollider->SetParent(smoke);
	smokeCollider->scale = { 7.0f,7.0f,14.13f };
	smokeCollider->translation = { 2.77f,-1.63f,-0.89f };
	smokeCollider->SetLabel("SmokeCollider");

	bulletsCase = new Model("bulletsCase");
	bulletsCase->GetReader()->GetMaterial()[0]->Load(L"bulletsCase.mat");
	bulletsCase->scale *= 0.2f;
	bulletsCase->rotation = { XM_PIDIV2, XM_PIDIV2, 0.0f };
	bulletsCase->translation = { 215.0f,14.3f,20.04f };
	bulletsCase->SetLabel("BulletsCase");
	caseCollider = new ColliderBox;
	caseCollider->SetParent(bulletsCase);
	caseCollider->scale = { 19.76f,49.76f,30.0f };
	caseCollider->translation = { -0.1f,0.0f,-15.31f };
	caseCollider->SetLabel("CaseCollider");

}

void ShootingScene::SetUI()
{

	crossHair = new Quad();
	crossHair->GetMaterial()->SetDiffuseMap(L"UI/CrossHair.png");
	crossHair->GetMaterial()->SetShader(L"Texture");
	crossHair->scale = { 70, 70, 1 };

	rifleZoom = new Quad();
	rifleZoom->GetMaterial()->SetDiffuseMap(L"UI/RifleZoom.png");
	rifleZoom->GetMaterial()->SetShader(L"Texture");
	rifleZoom->scale = { 800, 800, 1 };

	pistolZoom = new Quad();
	pistolZoom->GetMaterial()->SetDiffuseMap(L"UI/PistolZoom.png");
	pistolZoom->GetMaterial()->SetShader(L"Texture");
	pistolZoom->scale = { 800, 800, 1 };

}

void ShootingScene::WeaponUI()
{
	rifleUI = new Quad();
	rifleUI->GetMaterial()->SetDiffuseMap(L"UI/rifle.png");
	rifleUI->GetMaterial()->SetShader(L"Texture");
	rifleUI->scale = { 100, 100, 1 };
	rifleUI->rotation.z = -XM_PIDIV4;
	rifleUI->translation = { 70.0f,60.0f,0.0f };

	pistolUI = new Quad();
	pistolUI->GetMaterial()->SetDiffuseMap(L"UI/pistol.png");
	pistolUI->GetMaterial()->SetShader(L"Texture");
	pistolUI->scale = { 90,90,1 };
	pistolUI->translation = { 70.0f,60.0f,0.0f };


	grenadeUI = new Quad();
	grenadeUI->GetMaterial()->SetDiffuseMap(L"UI/grenade.png");
	grenadeUI->GetMaterial()->SetShader(L"Texture");
	grenadeUI->scale = { 100,100,1 };
	grenadeUI->translation = { 70.0f,60.0f,0.0f };


	smokeUI = new Quad();
	smokeUI->GetMaterial()->SetDiffuseMap(L"UI/smoke.png");
	smokeUI->GetMaterial()->SetShader(L"Texture");
	smokeUI->scale = { 100,100,1 };
	smokeUI->translation = { 70.0f,60.0f,0.0f };

	rectUI = new Quad();
	rectUI->GetMaterial()->SetDiffuseMap(L"UI/rect.png");
	rectUI->GetMaterial()->SetShader(L"Texture");


}

void ShootingScene::SetButton()
{
	button = new Model("btn2");
	button->GetReader()->GetMaterial()[0]->Load(L"button.mat");
	button->SetLabel("Button");
	button->translation = { 242.75f,25.0f,30.0f };
	button->scale *= 15.0f;
	button->rotation.y = XM_PI;
	startCollider = new ColliderBox;
	startCollider->SetParent(button);
	startCollider->SetLabel("StartCollider");
	startCollider->scale = { 0.3f,0.35f,0.41f };
	startCollider->translation = { 0.0f,0.22f,-0.15f };
	stopCollider = new ColliderBox;
	stopCollider->SetParent(button);
	stopCollider->SetLabel("StopCollider");
	stopCollider->scale = { 0.3f,0.35f,0.41f };
	stopCollider->translation = { 0.0f,-0.19f,-0.15f };
}

void ShootingScene::SetTerrain()
{
	terrain1 = new Terrain(L"Landscape/Dirt3.png", L"HeightMap/AlphaMap.png");
	terrain2 = new Terrain(L"Landscape/Dirt3.png", L"HeightMap/Pock.png");
	terrain2->scale.x *= 2.57f;
	terrain2->rotation.y = -XM_PIDIV2;
	terrain2->translation = { 1.24f,20.19f,0.0f };

	terrain2->SetLabel("Pock");

	terrain1R = new Terrain(L"Landscape/Dirt3.png", L"HeightMap/AlphaMap.png");
	terrain1R->translation.z = 255.0f;
	terrain1R->SetLabel("Terrain1R");
	terrain1L = new Terrain(L"Landscape/Dirt3.png", L"HeightMap/AlphaMap.png");
	terrain1L->translation.z = -255.0f;
	terrain1L->SetLabel("Terrain1L");

	terrain1D = new Quad();
	terrain1D->GetMaterial()->SetDiffuseMap(L"Landscape/Dirt3.png");
	terrain1D->SetLabel("Terrain1D");
	terrain1D->scale = { 250.f,760.f,1.0f };
	terrain1D->rotation.x = XM_PIDIV2;
	terrain1D->translation = { 380.0f,0.0f,130.0f };
	


}

void ShootingScene::SetFence()
{
	for (UINT x = 0; x < 6; x++)
	{
		Fence* fence = new Fence;

		fence->translation = { float(-37.01 + 35 * x), float(29.42 - 5 * x), 0.0f };
		fence->rotation.z = -XM_PIDIV2 * 1.07f;
		fences.emplace_back(fence);

	}
	for (UINT x = 0; x < 6; x++)
	{
		Fence* fence = new Fence;

		fence->translation = { float(-37.01 + 35 * x), float(29.42 - 5 * x), 255.0f };
		fence->rotation.z = +XM_PIDIV2 * 1.07f;
		fence->rotation.y = XM_PI;
		fences.emplace_back(fence);

	}
}

void ShootingScene::SetTree()
{
	tree = new ModelInstancing("Tree");

	for (float z = 0; z < 3; z++)
	{
		for (float x = 0; x < 3; x++)
		{
			Transform* transform = tree->Add();

			Vector3 pos;
			pos.x = Random(-30.0f, -5.0f);
			
			if (Random(0.0f, 1.0f) < 0.5f)
			{
				pos.z = Random(5.0f, 66.0f);
			}
			else
			{
				pos.z = Random(184.0f, 230.0f);
			}
			pos.y = terrain2->GetHeight(pos) + 20.0f;
			
			transform->translation = pos;
			transform->rotation.x = XM_PIDIV2;


		}
	}
}

void ShootingScene::SetWater()
{
	water = new Water(L"Landscape/WaveNormal.png");
	water->scale = { 0.25f,0.77f,1.11f };
	//water->scale = { 1.11f ,0.77f,0.25f };
	water->rotation.z = -XM_PIDIV2;
	//water->rotation.x = -XM_PIDIV2;
	water->translation = { -50.47f,30.0f,130.0f };
}

void ShootingScene::SetWaterParticle()
{
	for (UINT x = 0; x < 20; x++)
	{
		Vector3 particlePosition = { -48.470f, 34.0f, 82.0f + 5.0f * x };

		ParticleSystem* particle = new ParticleSystem("Water.fx", particlePosition);
		if (particle != nullptr) {
			particles.emplace_back(particle);
		}
	}
}

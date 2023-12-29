#include "Framework.h"
#include "Swat.h"

Swat::Swat()
	:ModelAnimator("Swat")
{
	scale *= 0.5f;
	
	CreateClips();

	CreateTexture();
	CreateWeapon();

	clips[ATTACK_R]->SetEndEvent(0.7f, bind(&Swat::SetClip, this, IDLE_R));
	clips[RELOAD_R]->SetEndEvent(0.8f, bind(&Swat::SetClip, this, IDLE_R));

	clips[ATTACK_P]->SetEndEvent(0.8f, bind(&Swat::SetClip, this, IDLE_P));
	clips[RELOAD_P]->SetEndEvent(0.8f, bind(&Swat::SetClip, this, IDLE_P));

	clips[THROW_R]->SetEndEvent(0.8f, bind(&Swat::SetClip, this, IDLE_T));
	clips[THROW_P]->SetEndEvent(0.8f, bind(&Swat::SetClip, this, IDLE_T));

	clips[GRAB_R]->SetEndEvent(0.8f, bind(&Swat::SetClip, this, IDLE_R));
	clips[GRAB_P]->SetEndEvent(0.8f, bind(&Swat::SetClip, this, IDLE_P));
	clips[GRAB_T]->SetEndEvent(0.8f, bind(&Swat::SetClip, this, IDLE_T));

	//clips[THROW_S]->SetEndEvent(0.8f, bind(&Swat::SetClip, this, IDLE_T));

	//hpBar = new ProgressBar(L"UI/hp_bar.png", L"UI/hp_bar_BG.png");
	//hpBar->SetLabel("HP Bar");
	//hpBar->scale.x *= 0.5f;

	//usedBullet = new UsedBullet();
	usedBullets = new UsedBulletManager(30);

	fireParticle = new ParticleSystem("Fire6.fx");
	fireSmoke = new ParticleSystem("FireSmoke2.fx");
	smokeParticle = new ParticleSystem("Smoke3.fx");
	smokeParticle->scale *= 7.0f;
	smokeParticle->Stop();
	boomParticle = new ParticleSystem("Bomb.fx");
	boomParticle->Stop();
	boomParticle->scale *= 15.0f;

	throwGrenade = new Grenade();
	throwGrenadeCollider = new ColliderSphere;
	throwGrenadeCollider->SetParent(throwGrenade);
	throwGrenadeCollider->SetLabel("ThrowGrenadeCollider");
	throwGrenadeCollider->scale *= 500.0f;
	throwSmoke   = new Smoke();


	SoundManager::GetInstance()->AddSound("riflefire"  , "_Sound/rifleSound.mp3"  , false);
	SoundManager::GetInstance()->AddSound("pistolSound" , "_Sound/pistolSound.mp3" , false);
	SoundManager::GetInstance()->AddSound("reloadSound" , "_Sound/reload2Sound.mp3" , false);
	SoundManager::GetInstance()->AddSound("grenadeSound", "_Sound/grenadeSound.mp3", false);
	SoundManager::GetInstance()->AddSound("noammoSound", "_Sound/noammoSound.mp3", false);
	//SoundManager::GetInstance()->AddSound("PlayerShoot", "_Sound/PlayerShoot.wav", false);
}

Swat::~Swat()
{
	delete rifle;
	delete riflemagazine;

	delete pistol;

	delete grenade;
	delete smoke;

	//delete hpBar;
	//delete usedBullet;
	delete usedBullets;
	delete throwGrenade;
	delete throwSmoke;

	delete fireParticle;
	delete fireSmoke;
	delete smokeParticle;
	delete boomParticle;

	delete throwGrenadeCollider;
	
}

void Swat::Update()
{
	ModelAnimator::Update();

	accumulatedTime += Time::Delta();
	grenadeColliderTime += Time::Delta();
	
	SwitchWeapon();

	if (isRifle)
		rifle->Update();
	else if (isPistol)
		pistol->Update();
	else if (isGrenade)
		grenade->Update();
	else if (isSmoke)
		smoke->Update();
	

	riflemagazine->Update();
	usedBullets  ->Update();
	throwGrenade ->Update();
	throwSmoke   ->Update();

	if (Environment::GetInstance()->IsGameMode())
	{
		Move();

		if (isRifle || isPistol)
		{
			Shoot();
			isPosterRayOn = true;
			ResumeAnimation();
			needToStop = false;
			isThrowing = false;

		}
		else if (isGrenade || isSmoke)
		{
			Throw();
			isPosterRayOn = false;
		}
	}
	
	
	UpdateRightHand();

	if (throwSmoke->IsSmoke())
	{
		smokeParticle->Play(throwSmoke->translation);
		throwSmoke->IsSmoke() = false;

	}

	if (throwGrenade->IsBoom())
	{
		boomParticle->Play(throwGrenade->translation);
		SoundManager::GetInstance()->Play("grenadeSound", 0.35f);
		grenadeColliderOn = true;
		grenadeColliderTime = 0.0f;
		throwGrenade->IsBoom() = false;
	}

	if (grenadeColliderOn)
	{
		if (grenadeColliderTime >= 1.0f)
		{
			grenadeColliderOn = false;
		}
	}


	fireParticle->Update();
	fireSmoke->Update();
	smokeParticle->Update();
	boomParticle->Update();
	
	if (grenadeColliderOn)
		throwGrenadeCollider->Update();

}

void Swat::Render()
{
	ModelAnimator::Render();
	if (!Environment::GetInstance()->IsGameMode())
	{
		reader->GetRenderMesh0() = true;
		reader->GetRenderMesh1() = true;
		this->rotation.x = 0;
	}
	else
	{
		if (Environment::GetInstance()->IsFPmode())
		{
			reader->GetRenderMesh0() = false;
			reader->GetRenderMesh1() = false;
		}
		else
		{
			reader->GetRenderMesh0() = true;
			reader->GetRenderMesh1() = true;
		}
	}
	
	if (isRifle)
		rifle->Render();
	else if (isPistol)
		pistol->Render();
	else if (isGrenade)
		grenade->Render();
	else if (isSmoke)
		smoke->Render();

	

	riflemagazine->Render();
	//usedBullet->Render();
	usedBullets->Render();
	throwGrenade->Render();
	throwSmoke  ->Render();

	fireParticle->Render();
	fireSmoke->Render();
	smokeParticle->Render();
	boomParticle->Render();
	
	if (grenadeColliderOn)
		throwGrenadeCollider->Render();

}




void Swat::Debug()
{
	ModelAnimator::Debug();

	//usedBullet->PostRender();
	throwGrenade->PostRender();
	throwSmoke  ->PostRender();

	ImGui::SliderFloat("Speed", &speed, 0.0f, 10.0f);
	ImGui::SliderFloat("TakeTime", &takeTime, 0.0f, 1.0f);

//	ImGui::DragFloat3("ThrowRotation", (float*)&throwRotation, 0.01f, 0.01f, 100.0f);
//	ImGui::DragFloat3("ThrowPosition", (float*)&throwPosition, 0.01f, 0.01f, 100.0f);
	/*
	ImGui::Begin("Ammo Display");
	ImGui::Text("Rifle Bullet: %d", rifleBullet); // 현재 총알 개수 표시
	ImGui::Text("Pistol Bullet: %d", pistolBullet);
	ImGui::End();
	*/
	rifle->Debug();
	pistol->Debug();
	grenade->Debug();
	smoke->Debug();

	throwGrenadeCollider->Debug();

	Transform::Debug();
	label = "Swat";

	static float value = 1.0f;

	//hpBar->SetValue(value);

	//ImGui::SliderFloat("HP", &value, 0.0f, 1.0f);

}

void Swat::PostRender()
{
	Debug();

	//hpBar->Render();
}

void Swat::SetClip(AnimState state)
{
	if (curState != state)
	{
		PlayClip(state);
		curState = state;
	}
}

void Swat::UpdateRightHand()
{
	UINT nodeIndex = reader->GetNodeIndex("mixamorig:RightHand");

	Matrix nodeTransform = GetTransformByNode(nodeIndex);


	righthand->GetWorld() = nodeTransform * world;
}

void Swat::Collision(PosterManager* posters)
{
	for (Poster* poster : posters->GetPosters())
	{
		if (!poster->IsActive())
			continue;

		if (poster->GetPosterCollider()->Collision(throwGrenadeCollider))
		{
			poster->IsActive() = false;
			poster->GetTransform()->translation = { 0.0f,0.0f,0.0f };
		}
	}

}

void Swat::RayCollision(PosterManager* posters, Ray inputray)
{
	for (Poster* poster : posters->GetPosters())
	{
		if (!poster->IsActive())
			continue;
		
		Ray ray = inputray;

	
		static Contact contact = {};
		

		if (poster->GetPosterCollider()->Collision(ray, &contact))
		{

			if(isRifle||isPistol)
				poster->GetPosterCollider()->SetColor(1, 0, 0);
			else
				poster->GetPosterCollider()->SetColor(0, 1, 0);
			
			

			if (isRifle)
			{
				if (KEY_DOWN(VK_LBUTTON))
				{
					poster->IsActive() = false;
					poster->GetTransform()->translation = { 0.0f,0.0f,0.0f };
					poster->GetPosterCollider()->SetColor(0, 1, 0);
				}
			}
			else if (isPistol && isPistolFired)
			{
				if (KEY_DOWN(VK_LBUTTON))
				{
					poster->IsActive() = false;
					poster->GetTransform()->translation = { 0.0f,0.0f,0.0f };
					poster->GetPosterCollider()->SetColor(0, 1, 0);
				}
			}
		
			

		}
		else
		{
			poster->GetPosterCollider()->SetColor(0, 1, 0);
		}


	}
}

void Swat::Grab()
{
	if (isRifle)
		SetClip(GRAB_R);
	else if (isPistol)
		SetClip(GRAB_P);
	else
		SetClip(GRAB_T);

}

void Swat::Move()
{
	if (KEY_PRESS('W'))
	{
		if (!Environment::GetInstance()->IsFPmode())
		{
			if (isRifle)
				SetClip(FORWARD_R);
			else
				SetClip(FORWARD_P);

		}


		translation -= Forward() * moveSpeed * Time::Delta();
	}
	if (KEY_PRESS('S'))
	{
		if (!Environment::GetInstance()->IsFPmode())
		{
			if (isRifle)
				SetClip(BACKWARD_R);
			else
				SetClip(BACKWARD_P);

		}

		translation -= Backward() * moveSpeed * Time::Delta();
	}

	if (KEY_UP('W') || KEY_UP('S')|| KEY_UP('A') || KEY_UP('D'))
	{
		if (isRifle)
			SetClip(IDLE_R);
		else
			SetClip(IDLE_P);

	}

	if (KEY_PRESS('A'))
	{
		//rotation.y -= rotSpeed * Time::Delta();
		SetClip(LEFT);
		translation -= Left() * (moveSpeed * 0.4f )* Time::Delta();
	}

	if (KEY_PRESS('D'))
	{
		//rotation.y += rotSpeed * Time::Delta();
		SetClip(RIGHT);
		translation -= Right() * (moveSpeed * 0.4f ) * Time::Delta();
	}
	

}

void Swat::Shoot()
{
	//VK_LBUTTON
	if (KEY_DOWN(VK_LBUTTON))
	{
		if(isRifle&&rifleBullet==0)
			SoundManager::GetInstance()->Play("noammoSound", 0.45f);
		else if(isPistol&&pistolBullet==0)
			SoundManager::GetInstance()->Play("noammoSound", 0.45f);


		if (isRifle && rifleBullet > 0)
		{
			if (rifleBullet > 0)
				rifleBullet--;

			SetClip(ATTACK_R);
			CAMERATP->ApplyRecoil(0.02f);
			CAMERATP->ApplyLRRecoil();
			CAMERAFP->ApplyRecoil(0.04f);
			CAMERAFP->ApplyLRRecoil();
			/////////////////////////////////총알위치 동적할당/////////////////////////////
			Vector3 muzzleOffsetLocal = Vector3(-2.0f, 14.5f, -5.5f);
			XMVECTOR rotationQuaternion = XMQuaternionRotationRollPitchYaw(this->rotation.x, this->rotation.y, this->rotation.z);
			XMVECTOR muzzleOffsetWorld = XMVector3Rotate(XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&muzzleOffsetLocal)), rotationQuaternion);
			XMVECTOR charWorldPosition = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&this->translation));
			XMVECTOR bulletPositionWorld = charWorldPosition + muzzleOffsetWorld;
			Vector3 bulletPosition;
			XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&bulletPosition), bulletPositionWorld);
			usedBullets->Fire(bulletPosition, this->rotation, Backward(), Up(), Left());
			fireSmoke->Play(bulletPosition);


			//fireSmokePosition = bulletPosition;

			////////////////////////////fireParticle동적할당///////////////////////////////
			muzzleOffsetLocal = Vector3(-1.0f, 15.5f, -9.5f);
			rotationQuaternion = XMQuaternionRotationRollPitchYaw(this->rotation.x, this->rotation.y, this->rotation.z);
			muzzleOffsetWorld = XMVector3Rotate(XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&muzzleOffsetLocal)), rotationQuaternion);
			charWorldPosition = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&this->translation));
			bulletPositionWorld = charWorldPosition + muzzleOffsetWorld;
			Vector3 firePosition;
			XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&firePosition), bulletPositionWorld);

			fireParticle->Play(firePosition);
			
			SoundManager::GetInstance()->Play("riflefire", 0.15f);
		}
		if(isPistol && pistolBullet > 0)
		{
			if (accumulatedTime >= 2.0)
			{
				if (pistolBullet > 0)
					pistolBullet--;

				SetClip(ATTACK_P);
				CAMERATP->ApplyRecoil(0.02f);
				CAMERATP->ApplyLRRecoil();
				CAMERAFP->ApplyRecoil(0.04f);
				CAMERAFP->ApplyLRRecoil();
				//////////////////////////////////////총알위치동적할당////////////////////////////////
				Vector3 muzzleOffsetLocal = Vector3(-2.0f, 14.5f, -7.5f);
				XMVECTOR rotationQuaternion = XMQuaternionRotationRollPitchYaw(this->rotation.x, this->rotation.y, this->rotation.z);
				XMVECTOR muzzleOffsetWorld = XMVector3Rotate(XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&muzzleOffsetLocal)), rotationQuaternion);
				XMVECTOR charWorldPosition = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&this->translation));
				XMVECTOR bulletPositionWorld = charWorldPosition + muzzleOffsetWorld;
				Vector3 bulletPosition;
				XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&bulletPosition), bulletPositionWorld);
				usedBullets->Fire(bulletPosition, this->rotation, Backward(), Up(), Left());

				fireSmoke->Play(bulletPosition);

				//fireSmokePosition = bulletPosition;
				/////////////////////////////////fireparticle위치 동적할당//////////////////////////////
				muzzleOffsetLocal = Vector3(-0.5f, 15.5f, -9.5f);
				rotationQuaternion = XMQuaternionRotationRollPitchYaw(this->rotation.x, this->rotation.y, this->rotation.z);
				muzzleOffsetWorld = XMVector3Rotate(XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&muzzleOffsetLocal)), rotationQuaternion);
				charWorldPosition = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&this->translation));
				bulletPositionWorld = charWorldPosition + muzzleOffsetWorld;
				Vector3 firePosition;
				XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&firePosition), bulletPositionWorld);

				fireParticle->Play(firePosition);

				//fireParticlePosition = firePosition;

				SoundManager::GetInstance()->Play("pistolSound", 0.15f);

				accumulatedTime = 0.0;
				isPistolFired = true;
			}
			else
			{
				isPistolFired = false;
			}
		}
	}

	if (KEY_DOWN('R'))
	{
		
		if (isRifle&&rifleBullet <30 && rifleTotalBullet >0)
		{
			SoundManager::GetInstance()->Play("reloadSound", 0.45f);
			SetClip(RELOAD_R);
			int totalBullet = rifleBullet + rifleTotalBullet;
			if (totalBullet >= 30)
			{
				rifleBullet = 30;
				rifleTotalBullet = totalBullet - 30;
			}
			else
			{
				rifleBullet = totalBullet;
				rifleTotalBullet = 0;
			}
		}
		else if(isPistol&&pistolBullet< 7 &&pistolTotalBullet > 0)
		{
			SoundManager::GetInstance()->Play("reloadSound", 0.45f);
			SetClip(RELOAD_P);
			int totalBullet = pistolBullet + pistolTotalBullet;
			if (totalBullet >= 7)
			{
				pistolBullet = 7;
				pistolTotalBullet = totalBullet - 7;
			}
			else
			{
				pistolBullet = totalBullet;
				pistolTotalBullet = 0;
			}
		}
	}
}

void Swat::Throw()
{
	float cooldownDuration = 2.0f; 
	static float cooldownTimer = cooldownDuration; 

	if (isGrenade && totalGrenade > 0 || isSmoke && totalSmoke > 0)
	{
		if (cooldownTimer < cooldownDuration)
		{
			cooldownTimer += Time::Delta();
		}

		if (KEY_DOWN(VK_LBUTTON) && !isThrowing && cooldownTimer >= cooldownDuration)
		{
			SetClip(THROW_P); // THROW_P 클립 시작
			isThrowing = true; // 키가 눌렸음을 표시
			//needToStop = true;

			throwTimer = 0.0f;

		}

		// F 키를 뗄 때
		if (KEY_UP(VK_LBUTTON) && isThrowing)
		{
			isThrowing = false; // 키가 뗐음을 표시
			needToStop = false;
			ResumeAnimation(); // 애니메이션 재개

			
			Vector3 muzzleOffsetLocal = Vector3(-3.5f, 15.5f, -6.5f);
			XMVECTOR rotationQuaternion = XMQuaternionRotationRollPitchYaw(this->rotation.x, this->rotation.y, this->rotation.z);
			XMVECTOR muzzleOffsetWorld = XMVector3Rotate(XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&muzzleOffsetLocal)), rotationQuaternion);
			XMVECTOR charWorldPosition = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&this->translation));
			XMVECTOR bulletPositionWorld = charWorldPosition + muzzleOffsetWorld;
			Vector3 throwPosition;
			XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&throwPosition), bulletPositionWorld);

			if (isGrenade)
			{
				throwGrenade->Fire(throwPosition, this->rotation, Up(), Backward());
				totalGrenade--;
			}
			else if (isSmoke)
			{
				throwSmoke->Fire(throwPosition, this->rotation * 0.5f, Up(), Backward());
				totalSmoke--;
			}
			
			cooldownTimer = 0.0f;
		}


	}



	if (isThrowing)
	{
		if (throwTimer >= 0.6f)
		{
			needToStop = true;
			throwTimer = 0.0f;

		}
		throwTimer += Time::Delta();
	}

	
}

void Swat::CreateWeapon()
{
	/////////////rifle/////////////////////
	rifle = new Model("Gun_M41D");
	rifle->scale *= 0.002f;
	rifle->rotation.z = +XM_PIDIV2;
	rifle->rotation.y = -XM_PIDIV4 * 1.5f;
	rifle->translation = { 6.44f,0.476f,-0.53f };
	//rifle->translation.z = 15.0f;
	righthand = new Transform();
	rifle->SetParent(righthand);
	rifle->SetLabel("Rifle");

	riflemagazine = new Model("Riflemagazine");
	rifle->scale *= 5.0f;
	//////////////////pistol//////////////////
	pistol = new Model("Pistol");
	pistol->GetReader()->GetMaterial()[0]->Load(L"Pistol.mat");
	pistol->SetLabel("Pistol");
	pistol->SetParent(righthand);
	pistol->scale *= 10.0f;
	pistol->rotation.y = -XM_PIDIV2 * 1.04f;
	pistol->rotation.z = -XM_PIDIV2 * 0.71f;
	pistol->translation = { 8.850f,12.06f,-3.14f };
	///////////////////grenade////////////////
	grenade = new Model("grenade");
	grenade->GetReader()->GetMaterial()[0]->Load(L"grenade.mat");
	grenade->SetLabel("Grenade");
	grenade->SetParent(righthand);
	grenade->rotation.z = -XM_PIDIV2;
	grenade->translation = { -1.56f,7.96f,-3.56f };

	///////////////////smoke//////////////////////////
	smoke = new Model("M18_Smoke");
	smoke->GetReader()->GetMaterial()[0]->Load(L"smoke.mat");
	smoke->SetLabel("Smoke");
	smoke->SetParent(righthand);
	smoke->scale *= 1.3f;
	smoke->rotation.y = -XM_PIDIV2;
	smoke->translation = { 0.0f,10.26f,-7.01f };
}

void Swat::CreateClips()
{
	ReadClip("Rifle Aiming Idle");//IDLERIFLE
	ReadClip("Run Forward"); //FORWADRIFLE
	ReadClip("Backwards Rifle Walk");//BACKRIFLE
	ReadClip("Firing Rifle");//ATTACKRIFLE
	ReadClip("Reloading");//RELOADRIFLE
	ReadClip("RifleSideStepRight");//SideRightRifle
	ReadClip("RifleSideStepLeft");//SideLeftRifle

	ReadClip("Pistol IdleStay");//IDLEPISTOL
	ReadClip("Pistol Walk");//FORWARDPISTOL
	ReadClip("Pistol Walk Backward");//BACKPISTOL
	ReadClip("Shooting");//ATTACKPISTOL
	ReadClip("Reloading");//RELOADPISTOL

	ReadClip("Ready Idle");//THROWIDLE
	ReadClip("THROW");//THROWRIFLE
	ReadClip("THROW");//THROWPISTOL
	ReadClip("ThrowStart");
	ReadClip("ThrowEnd");

	ReadClip("grabobject");//GRABATRIFLE
	ReadClip("grabobject");//GRABATPISTOL
	ReadClip("grabobject");//GRABTHROW
}

/*void Swat::RenderTrajectory(vector<Vector3>& trajectoryPoints)
{
	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(D3D11_BUFFER_DESC));
	vertexBufferDesc.Usage          = D3D11_USAGE_DEFAULT; // 버퍼 사용 방식
	vertexBufferDesc.ByteWidth      = sizeof(Vector3) * trajectoryPoints.size(); // 버퍼 크기
	vertexBufferDesc.BindFlags      = D3D11_BIND_VERTEX_BUFFER; // 버퍼 타입
	vertexBufferDesc.CPUAccessFlags = 0; // CPU 접근 방식
	vertexBufferDesc.MiscFlags      = 0; // 기타 플래그

	D3D11_SUBRESOURCE_DATA vertexData;
	ZeroMemory(&vertexData, sizeof(D3D11_SUBRESOURCE_DATA));
	vertexData.pSysMem = trajectoryPoints.data();

	ID3D11Buffer* vertexBuffer;

	// 정점 데이터를 vertexData에 채워넣기
	// ...

	// 정점 버퍼 생성
	HRESULT hr = DEVICE->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);
	if (FAILED(hr)) {
		// 오류 처리
	}

	// 렌더링 파이프라인 설정
	// 셰이더 바인딩, 렌더링 상태 설정 등
	// ...

	// 정점 버퍼 바인딩
	UINT stride = sizeof(Vector3);
	UINT offset = 0;
	DC->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	DC->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	// 정점 렌더링
	DC->Draw(trajectoryPoints.size(), 0);

	// 버퍼 해제
	vertexBuffer->Release();
}
*/

void Swat::SwitchWeapon()
{
	static bool lastKeyState2 = false;
	static bool lastKeyState3 = false;
	static bool lastKeyState4 = false;
	static bool lastKeyState5 = false;

	bool currentKeyState2 = KEY_DOWN('2');
	bool currentKeyState3 = KEY_DOWN('3');
	bool currentKeyState4 = KEY_DOWN('4');
	bool currentKeyState5 = KEY_DOWN('5');

	int keyPressed = 0; 

	if      (currentKeyState2 && !lastKeyState2) keyPressed = 2;
	else if (currentKeyState3 && !lastKeyState3) keyPressed = 3;
	else if (currentKeyState4 && !lastKeyState4) keyPressed = 4;
	else if (currentKeyState5 && !lastKeyState5) keyPressed = 5;

	switch (keyPressed)
	{
	case 2: // Rifle
		isRifle = true; isPistol = false; isGrenade = false; isSmoke = false;
		CAMERAFP->IsSwatRilfe() = true; CAMERAFP->IsSwatPistol() = false; CAMERAFP->IsThrow() = false;
		SetClip(IDLE_R);
		break;
	case 3: // Pistol
		isRifle = false; isPistol = true; isGrenade = false; isSmoke = false;
		CAMERAFP->IsSwatRilfe() = false; CAMERAFP->IsSwatPistol() = true; CAMERAFP->IsThrow() = false;
		SetClip(IDLE_P);
		break;
	case 4: // Grenade
		isRifle = false; isPistol = false; isGrenade = true; isSmoke = false;
		CAMERAFP->IsSwatRilfe() = false; CAMERAFP->IsSwatPistol() = false; CAMERAFP->IsThrow() = true;
		SetClip(IDLE_T);
		break;
	case 5: // Smoke
		isRifle = false; isPistol = false; isGrenade = false; isSmoke = true;
		CAMERAFP->IsSwatRilfe() = false; CAMERAFP->IsSwatPistol() = false; CAMERAFP->IsThrow() = true;
		SetClip(IDLE_T);
		break;
	}

	lastKeyState2 = currentKeyState2;
	lastKeyState3 = currentKeyState3;
	lastKeyState4 = currentKeyState4;
	lastKeyState5 = currentKeyState5;
}


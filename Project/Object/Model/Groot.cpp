#include "Framework.h"
#include "Groot.h"

Groot::Groot()
	:ModelAnimator("Groot")
{
	scale *= 0.04f;
	ReadClip("Sad Idle");
	ReadClip("Running");
	ReadClip("Standing Melee Attack 360 High");
	CreateTexture();

	reader->GetMaterial()[0]->Load(L"Groot.mat");

	weapon = new Model("Dwarven_Axe");
	weapon->GetReader()->GetMaterial()[0]->Load(L"Dwarven_Axe.mat");
	//weapon->scale *= 100.0f;

	leftHand = new Transform();
	weapon->SetParent(leftHand);
	weapon->rotation.y = XMConvertToRadians(274);
	weapon->rotation.z = XMConvertToRadians(-79);

	weapon->translation = { -50.630f, 68.640, -20.580 };
	
	//////////////

	clips[ATTACK]->SetEndEvent(0.7f, bind(&Groot::SetClip, this, IDLE));

	hpBar = new ProgressBar(L"UI/hp_bar.png", L"UI/hp_bar_BG.png");
	hpBar->SetLabel("HP Bar");
	hpBar->scale.x *= 0.5f;
}

Groot::~Groot()
{
	delete weapon;
	delete leftHand;

	delete hpBar;
}

void Groot::Update()
{
	ModelAnimator::Update();
	weapon->Update();
	//leftHand->Update();
	 hpBar->Update();
	 hpBar->translation = CAMERA->WorldToScreenPoint(this->globalPosition + V_UP * 10);

	//Move();

	UpdateLeftHand();
}

void Groot::Render()
{
	ModelAnimator::Render();
	       weapon->Render();
}

void Groot::Debug()
{
	ModelAnimator::Debug();

	ImGui::SliderFloat("Speed", &speed, 0.0f, 10.0f);
	ImGui::SliderFloat("TakeTime", &takeTime, 0.0f, 1.0f);

	weapon->Debug();

	Transform::Debug();
	label = "Groot";

	static float value = 1.0f;

	hpBar->SetValue(value);

	ImGui::SliderFloat("HP", &value, 0.0f, 1.0f);
}

void Groot::PostRender()
{
	Debug();

	hpBar->Render();
}

void Groot::UpdateLeftHand()
{
	UINT nodeIndex = reader->GetNodeIndex("mixamorig:RightHand");

	Matrix nodeTransform = GetTransformByNode(nodeIndex);

	leftHand->GetWorld() = nodeTransform * world;
}

void Groot::SetClip(AnimState state)
{
	if (curState != state)
	{
		PlayClip(state);
		curState = state;
	}
}

void Groot::Move()
{
	if (KEY_PRESS('W'))
	{
		translation -= Forward() * moveSpeed * Time::Delta();
		SetClip(RUN);
	}

	if (KEY_PRESS('S'))
	{
		translation -= Backward() * moveSpeed * Time::Delta();
		SetClip(RUN);
	}

	if (KEY_UP('W') || KEY_UP('S'))
		SetClip(IDLE);

	if (KEY_PRESS('A'))
	{
		rotation.y -= rotSpeed * Time::Delta();
	}

	if (KEY_PRESS('D'))
	{
		rotation.y += rotSpeed * Time::Delta();
	}

	if (KEY_DOWN(VK_LBUTTON))
	{
		SetClip(ATTACK);
	}
}

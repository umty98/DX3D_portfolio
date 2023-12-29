#include "Framework.h"
#include "Camera.h"

Camera::Camera()
{
	viewBuffer = new ViewBuffer();

	

	Load();

	translation = { 60.0f, 100.0f, -80.0f };
	rotation.x = 0.65f;
}

Camera::~Camera()
{
	Save();

	delete viewBuffer;
}

void Camera::Update()
{
	CalculateFrustum();

	if (target == nullptr)
		FreeMode();
	else
	{
		

	}
	
}

void Camera::Debug()
{
	ImGuiIO io = ImGui::GetIO();
	distance -= io.MouseWheel * moveSpeed;

	if (ImGui::TreeNode("Camera Option"))
	{
		Vector3 pos = translation;
		Vector3 rot = rotation;

		ImGui::Text("Camera Pos : %.3f, %.3f, %.3f", pos.x, pos.y, pos.z);
		ImGui::Text("Camera Rot : %.3f, %.3f, %.3f", rot.x, rot.y, rot.z);

		Transform::Debug();

		ImGui::SliderFloat("Height",   &height,   -10.0f, 100.0f);
		ImGui::SliderFloat("Distance", &distance, -10.0f, 100.0f);

		ImGui::SliderFloat("RotationY", &rotY, 0, XM_2PI);

		ImGui::SliderFloat("Camera MoveDamping", &moveDamping, 0.0f, 30.0f);
		ImGui::SliderFloat("Camera  RotDamping", &rotDamping, 0.0f, 30.0f);



		ImGui::Text("MouseWheel : %f", temp);

		ImGui::TreePop();
	}
}

Ray Camera::ScreenPointToRay(Vector3 screenPos)
{
	Ray ray;
	ray.origin = Transform::translation;

	///////////////InvViewport///////////////

	Vector3 point;

	point.x = +(2.0f * screenPos.x) / WIN_WIDTH  - 1.0f;
	point.y = -(2.0f * screenPos.y) / WIN_HEIGHT + 1.0f;
	point.z =   1.0f; //FarZ


	/////////////InvProjection/////////////

	Matrix projection = Environment::GetInstance()->GetPersMatrix();

	XMFLOAT4X4 proj;
	
	XMStoreFloat4x4(&proj, projection); 

	point.x /= proj._11;
	point.y /= proj._22;

	///////////////InvView//////////

	Matrix invView = Transform::GetWorld();

	ray.direction = point * invView;
	ray.direction.Normalize();

	return ray;
}

Ray Camera::ScreenPointToRayLookAt(Vector3 screenPos, const Matrix& viewMatrix, const Matrix& projectionMatrix)
{
	Ray ray;
	ray.origin = translation;

	///////////////InvViewport///////////////
	Vector3 point;
	point.x = +(2.0f * screenPos.x) / WIN_WIDTH - 1.0f;
	point.y = -(2.0f * screenPos.y) / WIN_HEIGHT + 1.0f;
	point.z = 1.0f; //FarZ

	///////////////InvProjection/////////////
	XMFLOAT4X4 proj;
	XMStoreFloat4x4(&proj, projectionMatrix);
	point.x /= proj._11;
	point.y /= proj._22;

	///////////////InvView//////////
	Matrix invView = XMMatrixInverse(nullptr, viewMatrix);
	ray.direction = point * invView;
	ray.direction.Normalize();

	return ray;
}

Vector3 Camera::WorldToScreenPoint(Vector3 worldPos)
{
	Vector3 screenPos;

	screenPos = XMVector3TransformCoord(worldPos, viewMatrix);
	screenPos = XMVector3TransformCoord(screenPos, Environment::GetInstance()->GetPersMatrix());

	screenPos = (screenPos + Vector3(1, 1, 1)) * 0.5f;

	screenPos.x *= WIN_WIDTH;
	screenPos.y *= WIN_HEIGHT;

	return screenPos;
}

bool Camera::ContainPoint(Vector3 point)
{
	for (UINT i = 0; i < 6; i++)
	{
		Vector3 dot = XMPlaneDotCoord(planes[i], point);

		if (dot.x < 0.0f)
			return false;
	}

	return true;
}

bool Camera::ContainSphere(Vector3 center, float radius)
{
	Vector3 edge;
	Vector3 dot;

	for (UINT i = 0; i < 6; i++)
	{
		edge.x = center.x - radius;
		edge.y = center.y - radius;
		edge.z = center.z - radius;

		dot = XMPlaneDotCoord(planes[i], edge);
		if (dot.x > 0.0f)
			continue;

		edge.x = center.x + radius;
		edge.y = center.y - radius;
		edge.z = center.z - radius;

		dot = XMPlaneDotCoord(planes[i], edge);
		if (dot.x > 0.0f)
			continue;

		edge.x = center.x - radius;
		edge.y = center.y + radius;
		edge.z = center.z - radius;

		dot = XMPlaneDotCoord(planes[i], edge);
		if (dot.x > 0.0f)
			continue;

		edge.x = center.x - radius;
		edge.y = center.y - radius;
		edge.z = center.z + radius;

		dot = XMPlaneDotCoord(planes[i], edge);
		if (dot.x > 0.0f)
			continue;


		edge.x = center.x + radius;
		edge.y = center.y + radius;
		edge.z = center.z - radius;

		dot = XMPlaneDotCoord(planes[i], edge);
		if (dot.x > 0.0f)
			continue;

		edge.x = center.x + radius;
		edge.y = center.y - radius;
		edge.z = center.z + radius;

		dot = XMPlaneDotCoord(planes[i], edge);
		if (dot.x > 0.0f)
			continue;

		edge.x = center.x - radius;
		edge.y = center.y + radius;
		edge.z = center.z + radius;

		dot = XMPlaneDotCoord(planes[i], edge);
		if (dot.x > 0.0f)
			continue;

		edge.x = center.x + radius;
		edge.y = center.y + radius;
		edge.z = center.z + radius;

		dot = XMPlaneDotCoord(planes[i], edge);
		if (dot.x > 0.0f)
			continue;


		return false;
	}

	return true;
}

void Camera::ApplyRecoil(float recoilAmount)
{
	rotation.x -= recoilAmount;
	//recoilStartRotationX = Transform::rotation.x;
	//isRecoiling = true;
}

void Camera::ApplyLRRecoil()
{
	rotation.y += Random(-0.05f, 0.05f);
}

void Camera::FreeMode()
{
	if (KEY_PRESS(VK_RBUTTON))
	{
		if (KEY_PRESS(VK_LSHIFT))
			moveSpeed = 50.0f;
		else
			moveSpeed = 7.0f;

		if (KEY_PRESS('A'))
			Transform::translation += Transform::Left()     * moveSpeed * Time::Delta();
														    
		if (KEY_PRESS('D'))								    
			Transform::translation += Transform::Right()    * moveSpeed * Time::Delta();

		if (KEY_PRESS('S'))
			Transform::translation += Transform::Backward() * moveSpeed * Time::Delta();

		if (KEY_PRESS('W'))
			Transform::translation += Transform::Forward()  * moveSpeed * Time::Delta();
														    
		if (KEY_PRESS('Q'))								    
			Transform::translation += Transform::Up()       * moveSpeed * Time::Delta();
														    
		if (KEY_PRESS('E'))								    
			Transform::translation += Transform::Down()     * moveSpeed * Time::Delta();


		Vector3 dir = mousePos - oldPos;

		Transform::rotation.y += dir.x * rotSpeed;
		Transform::rotation.x += dir.y * rotSpeed;

	}

	oldPos = mousePos;

	viewMatrix = XMMatrixInverse(nullptr, Transform::GetWorld());

	SetView();
}

void Camera::TargetMode(Mode mode)
{
	if (KEY_PRESS(VK_UP))
		height += moveSpeed * Time::Delta();

	if (KEY_PRESS(VK_DOWN))
		height -= moveSpeed * Time::Delta();

	switch (mode)
	{
	case Camera::MODE1:
	{
		destRotY = LERP(destRotY, target->rotation.y + XM_PI, rotDamping * Time::Delta());

		XMMATRIX rotMatrix = XMMatrixRotationY(destRotY + rotY);

		Vector3 forward = V_BACKWARD * rotMatrix;

		destination = target->GetGlobalPosition() + forward * distance + V_UP * height;

		Transform::translation = LERP(Transform::translation, destination, moveDamping * Time::Delta());

		viewMatrix = XMMatrixLookAtLH(Transform::translation, target->translation, V_UP);
	}
		break;
	case Camera::MODE2:
	{
		if (KEY_PRESS(VK_RBUTTON))
		{
			Vector3 dir = mousePos - oldPos;

			Transform::rotation.y += dir.x * rotSpeed * Time::Delta();
			Transform::rotation.x += dir.y * rotSpeed * Time::Delta();
		}

		oldPos = mousePos;
		

		destRotY = LERP(destRotY, Transform::rotation.y, rotDamping * Time::Delta());
		destRotX = LERP(destRotX, Transform::rotation.x, rotDamping * Time::Delta());

		XMMATRIX rotMatrix = XMMatrixRotationRollPitchYaw(destRotX, destRotY + rotY, 0.0f);

		Vector3 forward = V_FORWARD * rotMatrix;

		destination = target->GetGlobalPosition() + forward * distance;

		Transform::translation = LERP(Transform::translation, destination, moveDamping * Time::Delta());

		viewMatrix = XMMatrixLookAtLH(Transform::translation, target->translation, V_UP);

		viewMatrix *= XMMatrixTranslation(0, -height, 0);
	}
		break;
	case Camera::MODE3:
	{

		/*if (KEY_PRESS(VK_RBUTTON)) {
			// 마우스 오른쪽 버튼을 누른 상태에서 마우스의 움직임에 따라 카메라 회전
			Vector3 dir = mousePos - oldPos;
			Transform::rotation.y += dir.x * rotSpeed * Time::Delta();
			Transform::rotation.x += dir.y * rotSpeed * Time::Delta();

			target->rotation.y = Transform::rotation.y;
		}
		*/

		// 플레이어의 움직임에 따라 카메라 이동


		/*
		Transform::translation = target->GetGlobalPosition() - target->Backward() * distance + target->Up() * height;
		oldPos = mousePos;
		viewMatrix = XMMatrixInverse(nullptr, Transform::GetWorld());
		*/

		/*
		Vector3 cameraPosition = target->GetGlobalPosition() - target->Backward() * distance + target->Up() * height;
		Vector3 lookAtPosition = target->GetGlobalPosition();


		Transform::translation = cameraPosition;

		Vector3 upDirection = Vector3(0, 1, 0);
		viewMatrix = XMMatrixLookAtLH(cameraPosition, lookAtPosition, upDirection);

		oldPos = mousePos;
		*/


		cameraPosition = target->GetGlobalPosition() - target->Backward() * distance + target->Up() * height * 0.5f;


		// 카메라의 위아래 방향을 조절
		XMMATRIX pitchMatrix = XMMatrixRotationAxis(target->Right(), -Transform::rotation.x);
		XMVECTOR direction = XMVector3TransformNormal(target->Backward(), pitchMatrix);

		// 수정된 방향을 사용하여 lookAtPosition을 계산
		Vector3 adjustedDirection = Vector3(XMVectorGetX(direction), XMVectorGetY(direction), XMVectorGetZ(direction));
		//lookAtPosition = target->GetGlobalPosition() + adjustedDirection; // 카메라가 항상 캐릭터를 바라봅니다.
		lookAtPosition = cameraPosition + adjustedDirection;
		Transform::translation = cameraPosition;

		// upDirection 설정
		upDirection = target->Up();
		viewMatrix = XMMatrixLookAtLH(cameraPosition, lookAtPosition, upDirection);

		// 마우스의 이전 위치를 저장
		oldPos = mousePos;

	}
		break;
	default:
		break;
	}



	SetView();
}

void Camera::SetView()
{
	Transform::Update();

	//Vector3   eyePos = Transform::translation;
	//Vector3 focusPos = Transform::translation + Transform::Forward();
	//Vector3 upVector = Transform::Up();
	//
	//viewMatrix = XMMatrixLookAtLH(eyePos, focusPos, upVector);

	viewMatrix = XMMatrixInverse(nullptr, world);
	viewBuffer->SetData(viewMatrix, Transform::GetWorld());
	viewBuffer->SetVSBuffer(1);
}

void Camera::Save()
{
	BinaryWriter data(L"CameraData");

	data.WriteData(Transform::scale);
	data.WriteData(Transform::rotation);
	data.WriteData(Transform::translation);
}

void Camera::Load()
{
	BinaryReader data(L"CameraData");

	if (!data.Succeeded())
		return;

	Transform::scale       = data.ReadVector3();
	Transform::rotation    = data.ReadVector3();
	Transform::translation = data.ReadVector3();
}

void Camera::CalculateFrustum()
{
	XMFLOAT4X4 VP;
	XMStoreFloat4x4(&VP, viewMatrix * Environment::GetInstance()->GetPersMatrix());

	//Left
	a = VP._14 + VP._11;
	b = VP._24 + VP._21;
	c = VP._34 + VP._31;
	d = VP._44 + VP._41;

	planes[0] = XMVectorSet(a, b, c, d);

	//Right
	a = VP._14 - VP._11;
	b = VP._24 - VP._21;
	c = VP._34 - VP._31;
	d = VP._44 - VP._41;

	planes[1] = XMVectorSet(a, b, c, d);

	//Bottom
	a = VP._14 + VP._12;
	b = VP._24 + VP._22;
	c = VP._34 + VP._32;
	d = VP._44 + VP._42;

	planes[2] = XMVectorSet(a, b, c, d);

	//Top
	a = VP._14 - VP._12;
	b = VP._24 - VP._22;
	c = VP._34 - VP._32;
	d = VP._44 - VP._42;

	planes[3] = XMVectorSet(a, b, c, d);


	//Near
	a = VP._14 + VP._13;
	b = VP._24 + VP._23;
	c = VP._34 + VP._33;
	d = VP._44 + VP._43;

	planes[4] = XMVectorSet(a, b, c, d);

	//Far
	a = VP._14 - VP._13;
	b = VP._24 - VP._23;
	c = VP._34 - VP._33;
	d = VP._44 - VP._43;

	planes[5] = XMVectorSet(a, b, c, d);

	for (UINT i = 0; i < 6; i++)
	{
		planes[i] = XMPlaneNormalize(planes[i]);
	}
}

#include "Framework.h"
#include "CameraTP.h"

CameraTP::CameraTP()
{
	viewBuffer = new ViewBuffer();

	translation = { 60.0f, 100.0f, -80.0f };
	//rotation.x = 0.65f;
	rotation = { 0.0f,0.0f,0.0f };

	Load();
}

CameraTP ::~CameraTP()
{
	Save();


	delete viewBuffer;
}

void CameraTP::Update()
{
	TPMode();

	HWND hWnd = GetActiveWindow();
	RECT rect;
	GetWindowRect(hWnd, &rect);

	int windowCenterX = rect.left + (rect.right - rect.left) / 2;
	int windowCenterY = rect.top + (rect.bottom - rect.top) / 2;
	POINT currentMousePos;
	GetCursorPos(&currentMousePos);

	Vector3 mouseDelta;
	mouseDelta.x = static_cast<float>(currentMousePos.x - windowCenterX);
	mouseDelta.y = static_cast<float>(currentMousePos.y - windowCenterY);
	rotation.y += mouseDelta.x * rotSpeed;
	rotation.x += mouseDelta.y * rotSpeed;
	target->rotation.y = rotation.y;
	target->rotation.x = -rotation.x;
	rotation.x = max(rotation.x, minPitchAngle);
	rotation.x = min(rotation.x, maxPitchAngle);

	SetCursorPos(windowCenterX, windowCenterY);



	if (KEY_DOWN(VK_LBUTTON) && !isRecoiling)
	{
		isRecoiling = true;
		defaultRotation = rotation;
		recoilTimer = 0.0f;
	}

	if (isRecoiling)
	{
		float targetRotationX = defaultRotation.x;  // 기본 회전 각도로 복구
		rotation.x = LERP(rotation.x, targetRotationX, Time::Delta());

		rotation.y = LERP(rotation.y, defaultRotation.y, Time::Delta());

		recoilTimer += Time::Delta();
		if (recoilTimer >= recoilDuration)
			isRecoiling = false;
	}
	
}

void CameraTP::Debug()
{

	if (ImGui::TreeNode("CameraTP Option"))
	{
		Vector3 pos = translation;
		Vector3 rot = rotation;

		ImGui::Text("Camera Pos : %.3f, %.3f, %.3f", pos.x, pos.y, pos.z);
		ImGui::Text("Camera Rot : %.3f, %.3f, %.3f", rot.x, rot.y, rot.z);

		Transform::Debug();

		ImGui::SliderFloat("Height", &height, -10.0f, 100.0f);
		ImGui::SliderFloat("Distance", &distance, -10.0f, 100.0f);

		ImGui::SliderFloat("RotationY", &rotY, 0, XM_2PI);

		ImGui::SliderFloat("Camera MoveDamping", &moveDamping, 0.0f, 30.0f);
		ImGui::SliderFloat("Camera  RotDamping", &rotDamping, 0.0f, 30.0f);



		ImGui::Text("MouseWheel : %f", temp);

		ImGui::TreePop();
	}
}

Ray CameraTP::ScreenPointToRay(Vector3 screenPos)
{
	Ray ray;
	ray.origin = translation;

	///////////////InvViewport///////////////

	Vector3 point;

	point.x = +(2.0f * screenPos.x) / WIN_WIDTH - 1.0f;
	point.y = -(2.0f * screenPos.y) / WIN_HEIGHT + 1.0f;
	point.z = 1.0f; //FarZ


	/////////////InvProjection/////////////

	Matrix projection = Environment::GetInstance()->GetPersMatrix();

	XMFLOAT4X4 proj;

	XMStoreFloat4x4(&proj, projection);

	point.x /= proj._11;
	point.y /= proj._22;

	///////////////InvView//////////

	Matrix invView = GetWorld();

	ray.direction = point * invView;
	ray.direction.Normalize();

	return ray;
}

Ray CameraTP::ScreenPointToRayLookAt(Vector3 screenPos, const Matrix& viewMatrix, const Matrix& projectionMatrix)
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

Ray CameraTP::ScreenPointToRayLookAt(Vector3 screenPos)
{
	TPMode();

	Ray ray;
	ray.origin = translation;  // 카메라의 현재 위치를 광선의 시작점으로 합니다.

	// 화면 좌표를 NDC로 변환합니다.
	Vector3 point;
	point.x = (2.0f * screenPos.x) / WIN_WIDTH - 1.0f;
	point.y = -(2.0f * screenPos.y) / WIN_HEIGHT + 1.0f;
	point.z = 1.0f;  // Far plane을 사용합니다.

	// NDC 좌표를 투영 행렬을 역으로 적용합니다.
	XMFLOAT4X4 proj;
	XMStoreFloat4x4(&proj, Environment::GetInstance()->GetPersMatrix());
	point.x /= proj._11;
	point.y /= proj._22;

	Matrix invView = XMMatrixInverse(nullptr, viewMatrix);
	ray.direction = point * invView;
	ray.direction.Normalize();
	
	
	return ray;
}


Vector3 CameraTP::WorldToScreenPoint(Vector3 worldPos)
{
	Vector3 screenPos;

	screenPos = XMVector3TransformCoord(worldPos, viewMatrix);
	screenPos = XMVector3TransformCoord(screenPos, Environment::GetInstance()->GetPersMatrix());

	screenPos = (screenPos + Vector3(1, 1, 1)) * 0.5f;

	screenPos.x *= WIN_WIDTH;
	screenPos.y *= WIN_HEIGHT;

	return screenPos;
}

void CameraTP::ApplyRecoil(float recoilAmount)
{
	rotation.x -= recoilAmount;
}

void CameraTP::ApplyLRRecoil()
{
	rotation.y += Random(-0.05f, 0.05f);
}

Matrix CameraTP::GetViewMatrix()
{
	Vector3 position = this->translation; // 카메라의 현재 위치
	Vector3 direction = CalculateDirection(); // 카메라가 바라보는 방향을 계산하는 로직
	Vector3 target = position + direction; // 카메라가 바라보는 지점
	Vector3 up = Vector3(0.0f, 1.0f, 0.0f); // "위" 방향은 일반적으로 y축

	// 뷰 매트릭스 계산
	Matrix viewMatrix = XMMatrixLookAtLH(position, target, up);
	return viewMatrix;
}

Vector3 CameraTP::CalculateDirection()
{
	float pitch = rotation.x;
	float yaw = rotation.y;

	Vector3 direction;
	direction.x = cosf(pitch) * sinf(yaw);
	direction.y = sinf(pitch);
	direction.z = cosf(pitch) * cosf(yaw);

	return direction;
}

void CameraTP::FreeMode()
{
	if (KEY_PRESS(VK_RBUTTON))
	{
		moveSpeed = KEY_PRESS(VK_LSHIFT) ? 50.0f : 20.0f;

		if (KEY_PRESS('A'))
			translation += Left() * moveSpeed * Time::Delta();

		if (KEY_PRESS('D'))
			translation += Right() * moveSpeed * Time::Delta();

		if (KEY_PRESS('S'))
			translation += Backward() * moveSpeed * Time::Delta();

		if (KEY_PRESS('W'))
			translation += Forward() * moveSpeed * Time::Delta();

		if (KEY_PRESS('Q'))
			translation += Up() * moveSpeed * Time::Delta();

		if (KEY_PRESS('E'))
			translation += Down() * moveSpeed * Time::Delta();

		Vector3 dir = mousePos - oldPos;

		rotation.y += dir.x * rotSpeed * Time::Delta();
		rotation.x += dir.y * rotSpeed * Time::Delta();
		//Transform::rotation.x = max(-89.0f, min(89.0f, Transform::rotation.x));

	}

	oldPos = mousePos;

	viewMatrix = XMMatrixInverse(nullptr, GetWorld());

	SetView();
}

void CameraTP::TPMode()
{
	/*
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
	*/

	cameraPosition = target->GetGlobalPosition() - target->Backward() * distance + target->Up() * height * 0.5f;


	//cameraPosition = target->GetGlobalPosition() - target->Backward() * distance + target->Up() * height * 0.5f;

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

	SetView();
}

void CameraTP::SetView()
{
	Transform::Update();

	//Vector3   eyePos = Transform::translation;
//	Vector3 focusPos = Transform::translation + Transform::Forward();
	//Vector3 upVector = Transform::Up();
	
	//viewMatrix = XMMatrixLookAtLH(eyePos, focusPos, upVector);



	viewBuffer->SetData(viewMatrix, Transform::GetWorld());
	viewBuffer->SetVSBuffer(1);
}

void CameraTP::Save()
{
	BinaryWriter data(L"CameraData");

	data.WriteData(Transform::scale);
	data.WriteData(Transform::rotation);
	data.WriteData(Transform::translation);
}

void CameraTP::Load()
{
	BinaryReader data(L"CameraData");

	if (!data.Succeeded())
		return;

	Transform::scale = data.ReadVector3();
	Transform::rotation = data.ReadVector3();
	Transform::translation = data.ReadVector3();
}

#pragma once

class Swat;

class CameraFP : public Transform
{
public:
	CameraFP();
	~CameraFP();

public:
	void Update();

	void Debug();

	Ray ScreenPointToRay(Vector3 screenPos);
	Ray ScreenPointToRayLookAt(Vector3 screenPos, const Matrix& viewMatrix, const Matrix& projectionMatrix);
	Ray ScreenPointToRayLookAt(Vector3 screenPos);

	Vector3 WorldToScreenPoint(Vector3 worldPos);

	void SetTarget(Transform* target) { this->target = target; }

	Vector3 GetCameraPosition() const { return cameraPosition; }
	Vector3 GetLookAtPosition() const { return lookAtPosition; }
	Vector3 GetUpDirection() const { return upDirection; }

	void ApplyRecoil(float recoilAmount);
	void ApplyLRRecoil();

	bool& GetIsRecoiling() { return isRecoiling; }
	bool& IsSwatRilfe() { return isSwatRifle; }
	bool& IsSwatPistol() { return isSwatPistol; }
	bool& IsThrow() { return isThrow; }

	Matrix GetViewMatrix();
	Vector3 CalculateDirection();

private:
	void FreeMode();
	void FPMode();
	void SetView();

	void Save();
	void Load();
private:
	float moveSpeed = 20.0f;
	float  rotSpeed = 0.001f;

	ViewBuffer* viewBuffer;
	Matrix      viewMatrix;

	Vector3 oldPos;

	Transform* target;

	float distance = 60.0f;
	float height = 60.0f;

	Vector3 destination;
	Vector3 focusOffset;

	float rotY = 0.0f;
	float destRotY = 0.0f;
	float destRotX = 0.0f;

	float moveDamping = 5.0f;
	float  rotDamping = 1.0f;

	float temp = 0.0f;

	Vector3 cameraPosition;
	Vector3 lookAtPosition;
	Vector3 upDirection;

	bool isTargetMode = false;



	Vector3 savedRotation;
	bool isFirstSwitchToFreeMode = false;
	Vector3 defaultRotation;


	bool isRecoiling = false;
	float recoilTimer = 0.0f;
	const float recoilDuration = 0.5f;
	float recoilStartRotationX;

	bool isSwatRifle = true;
	bool isSwatPistol = false;
	bool isThrow = false;

	const float maxPitchAngle = XMConvertToRadians(45.0f);
	const float minPitchAngle = -XMConvertToRadians(45.0f);
};


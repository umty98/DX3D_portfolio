#pragma once


class Camera : public Transform
{
public:
	enum Mode
	{
		MODE1, MODE2, MODE3
	};

public:
	Camera();
	~Camera();

	void Update();

	void Debug();

	Ray ScreenPointToRay(Vector3 screenPos);
	Ray ScreenPointToRayLookAt(Vector3 screenPos, const Matrix& viewMatrix, const Matrix& projectionMatrix);

	Vector3 WorldToScreenPoint(Vector3 worldPos);

	void SetTarget(Transform* target) { this->target = target; }

	ViewBuffer* GetViewBuffer() { return viewBuffer; }

	bool ContainPoint(Vector3 point);
	bool ContainSphere(Vector3 center, float radius);

	Vector3 GetCameraPosition() const { return cameraPosition; }
	Vector3 GetLookAtPosition() const { return lookAtPosition; }
	Vector3 GetUpDirection() const { return upDirection; }

	Vector3& GetSwatRotation() { return swatRotation; }


	void SetView();
	//
	void ApplyRecoil(float recoilAmount);
	void ApplyLRRecoil();

	bool& GetIsRecoiling() { return isRecoiling; }


private:
	void   FreeMode();
	void TargetMode(Mode mode = MODE3);

	void Save();
	void Load();

	void CalculateFrustum();

private:
	float moveSpeed = 20.0f;
	float  rotSpeed = 0.001f;

	ViewBuffer* viewBuffer;
	Matrix      viewMatrix;
	
	Vector3 oldPos;

	Transform* target;

	float distance = 60.0f;
	float height   = 60.0f;

	Vector3 destination;
	Vector3 focusOffset;

	float rotY     = 0.0f;
	float destRotY = 0.0f;
	float destRotX = 0.0f;

	float moveDamping = 5.0f;
	float  rotDamping = 1.0f;

	float temp = 0.0f;

	XMVECTOR planes[6];
	float a, b, c, d;

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

	const float maxPitchAngle = XMConvertToRadians(45.0f);
	const float minPitchAngle = -XMConvertToRadians(45.0f);

	Vector3 swatRotation;

};

#pragma once

namespace MyMath
{
	float    Clamp(const float& value, float minValue, float maxValue);
	float Saturate(const float& value);

	float Max(const float& value1, const float& value2, const float& value3);
	float Min(const float& value1, const float& value2, const float& value3);

	float Distance(Vector3 v1, Vector3 v2);

	float Random(const float& min, const float& max);
	double Random(const double& min, const double& max);
	  int Random(const   int& min, const   int& max);

	float CalculateAngleBetweenVectors(Vector3& vec1, Vector3& vec2);

	Vector3 Random(Vector3 min, Vector3 max);

	Vector3 ClosestPointOnLine(Vector3 start, Vector3 end, Vector3 point);

	XMVECTOR QuaternionFromEuler(float pitch, float yaw, float roll);

	XMVECTOR ApplyQuaternion(const XMVECTOR& quat, const XMVECTOR& vec);

	XMVECTOR MultiplyQuaternion(const XMVECTOR& quat1, const XMVECTOR& quat2);

	XMMATRIX QuaternionToMatrix(const XMVECTOR& quat);

	XMVECTOR InverseQuaternion(const XMVECTOR& quat);

	Vector3 ExtractEulerAngleFromMatrix(const Matrix& viewMatrix);
	//
	vector<Vector3> CalculateTrajectory(Vector3 startPosition, Vector3 rotation, Vector3 forward, Vector3 up, float timeStep, float totalTime);

	Vector3 DirectionToRotation(Vector3 direction);
}


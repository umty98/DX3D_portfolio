#include "Framework.h"
#include "MyMath.h"

float MyMath::Clamp(const float& value, float minValue, float maxValue)
{
	return max(minValue, min(maxValue, value));
}

float MyMath::Saturate(const float& value)
{
	return Clamp(value, 0.0f, 1.0f);
}

float MyMath::Max(const float& value1, const float& value2, const float& value3)
{
	return max(max(value1, value2), value3);
}

float MyMath::Min(const float& value1, const float& value2, const float& value3)
{
	return min(min(value1, value2), value3);
}

float MyMath::Distance(Vector3 v1, Vector3 v2)
{
	return (v2 - v1).Length();
}

float MyMath::Random(const float& min, const float& max)
{
	float normal = ((float)rand() / RAND_MAX);

	return min + (max - min) * normal;
}

double MyMath::Random(const double& min, const double& max)
{
	return (double)rand() / (double)RAND_MAX * (max - min) + min;
}

int MyMath::Random(const int& min, const int& max)
{
	return min + rand() % (max - min);
}

float MyMath::CalculateAngleBetweenVectors(Vector3& vec1, Vector3& vec2)
{
	float dotProduct = Vector3::Dot(vec1, vec2);
	float magnitueProduct = vec1.Length() * vec2.Length();
	
	return acos(dotProduct / magnitueProduct);
}

Vector3 MyMath::Random(Vector3 min, Vector3 max)
{
	return Vector3(Random(min.x, max.x), Random(min.y, max.y), Random(min.z, max.z));
}

Vector3 MyMath::ClosestPointOnLine(Vector3 start, Vector3 end, Vector3 point)
{
	Vector3 line = end - start;

	float t = Vector3::Dot(line, point - start) / Vector3::Dot(line, line);
	t = Saturate(t);

	return start + line * t;
}

XMVECTOR MyMath::QuaternionFromEuler(float pitch, float yaw, float roll)
{
	return XMQuaternionRotationRollPitchYaw(pitch, yaw, roll);
}

XMVECTOR MyMath::ApplyQuaternion(const XMVECTOR& quat, const XMVECTOR& vec)
{
	return XMVector3Rotate(vec, quat);
}

XMVECTOR MyMath::MultiplyQuaternion(const XMVECTOR& quat1, const XMVECTOR& quat2)
{
	return XMQuaternionMultiply(quat1, quat2);
}

XMMATRIX MyMath::QuaternionToMatrix(const XMVECTOR& quat)
{
	return XMMatrixRotationQuaternion(quat);
}

XMVECTOR MyMath::InverseQuaternion(const XMVECTOR& quat)
{
	return XMQuaternionInverse(quat);
}

Vector3 MyMath::ExtractEulerAngleFromMatrix(const Matrix& viewMatrix)
{
	XMVECTOR quat = XMQuaternionRotationMatrix(viewMatrix);

	// 쿼터니언을 오일러 각도로 변환
	XMFLOAT3 angles;
	XMStoreFloat3(&angles, XMQuaternionRotationRollPitchYawFromVector(quat));

	return Vector3(angles.x, angles.y, angles.z);
}
//

vector<Vector3> MyMath::CalculateTrajectory(Vector3 startPosition, Vector3 rotation, Vector3 forward, Vector3 up, float timeStep, float totalTime)
{
	vector<Vector3> trajectoryPoints;

	XMVECTOR rotationQuaternion = XMQuaternionRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);

	XMVECTOR forwardVector = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&forward));
	XMVECTOR upVector = XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&up));

	XMVECTOR forwardWorld = XMVector3Rotate(forwardVector, rotationQuaternion);
	XMVECTOR upWorld = XMVector3Rotate(upVector, rotationQuaternion);

	XMFLOAT3 tempInitialVelocity;
	XMStoreFloat3(&tempInitialVelocity, forwardWorld * 20.0f + upWorld * 30.0f);
	Vector3 initialVelocityWorld = Vector3(tempInitialVelocity.x, tempInitialVelocity.y, tempInitialVelocity.z);

	Vector3 gravity = Vector3(0.0f, -9.8f, 0.0f);

	Vector3 position = startPosition;
	Vector3 velocity = initialVelocityWorld;

	for (float t = 0; t <= totalTime; t += timeStep) {
		velocity += gravity * timeStep;
		position += velocity * timeStep;
		trajectoryPoints.push_back(position);
	}

	return trajectoryPoints;
}

Vector3 MyMath::DirectionToRotation(Vector3 direction)
{
	Vector3 rotation;

	rotation.y = atan2(direction.x, direction.z);
	float horizontalDistance = sqrt(direction.x * direction.x + direction.z * direction.z);
	rotation.x = atan2(direction.y, horizontalDistance);

	rotation.z = 0.0f;

	return rotation;
}

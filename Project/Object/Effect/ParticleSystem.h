#pragma once

class ParticleSystem : public Transform
{
	struct ParticleData
	{
		bool isLoop      = false;
		bool isAdditive  = true;
		bool isBillboard = true;

		UINT count = 100;

		float duration = 1.0f;

		Vector3 minStartPos = {};
		Vector3 maxStartPos = {};

		Vector3 minVelocity = { -1, -1, -1 };
		Vector3 maxVelocity = { +1, +1, +1 };

		Vector3 minAcceleration = {};
		Vector3 maxAcceleration = {};

		Vector3 minStartScale = { 1, 1, 1 };
		Vector3 maxStartScale = { 1, 1, 1 };

		Vector3 minEndScale = { 1, 1, 1 };
		Vector3 maxEndScale = { 1, 1, 1 };

		float minSpeed = 1.0f;
		float maxSpeed = 3.0f;

		float minAngularVelocity = -10.0f;
		float maxAngularVelocity = +10.0f;

		float minStartTime = 0.0f;
		float maxStartTime = 0.0f;

		Vector4 startColor = { 1, 1, 1, 1 };
		Vector4   endColor = { 1, 1, 1, 1 };
	};

	struct ParticleInfo
	{
		Transform transform;

		Vector3     velocity;
		Vector3 acceleration;
		Vector3   startScale;
		Vector3     endScale;

		float speed = 1.0f;
		float angularVelocity = 0.0f;
		float startTime = 0.0f;
	};

	struct InstanceData
	{
		Matrix  transform;
		Vector4 color = { 1, 1, 1, 1 };
	};
public:
	ParticleSystem(string file);
	ParticleSystem(string file, Vector3 pos);//
	~ParticleSystem();
	
	void Update();
	void Render();

	void Play(Vector3 pos, Vector3 rot = Vector3());
	void PlayWP(Vector3 rot = Vector3());//
	void Stop();

	bool IsActive() { return quad->IsActive(); }

	vector<ParticleInfo> GetParticleInfo() { return particleInfos; }

private:
	void UpdatePhysics();
	//Vector3 CalculateBillboardRotation(Vector3 direction);
	Vector3 CalculateRotationFromDirection(Vector3 direction);
	void UpdateColor();

	void Init();

	void Load(string file);

private:
	Quad* quad;

	vector<InstanceData> instanceData;
	vector<ParticleInfo> particleInfos;

	VertexBuffer* instanceBuffer;

	ParticleData particleData;

	float     lifeTime = 0.0f;
	UINT     drawCount = 0;
	UINT particleCount = 100;

	Vector3 pos;

};

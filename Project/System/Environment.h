#pragma once

class Camera;
class CameraFP;
class CameraTP;

class Environment : public Singleton<Environment>
{
	friend class Singleton;
private:
	Environment();
	~Environment();

	void CreatePerspective();
	void CreateOrthographic();

	void DebugLight(int lightIndex);

public:
	void SetViewport(UINT width = WIN_WIDTH, UINT height = WIN_HEIGHT);
	void SetEnvironment();
	void PostSet();

	void PostRender();

	Matrix GetPersMatrix() { return persMatrix; }

	MatrixBuffer* GetPersBuffer() { return persBuffer; }

	Camera* GetMainCamera() { return mainCamera; }
	CameraFP* GetCameraFP() { return cameraFP; }
	CameraTP* GetCameraTP() { return cameraTP; }

	bool IsFPmode() { return isFPmode; }
	bool IsGameMode() { return isGameMode; }

	LightBuffer* GetLightBuffer() { return lightBuffer; }

private:
	MatrixBuffer*  persBuffer;
	Matrix         persMatrix;

	MatrixBuffer*  orthoBuffer;
	Matrix         orthoMatrix;

	ViewBuffer* UIViewBuffer;

	LightBuffer* lightBuffer;

	Camera* mainCamera;
	CameraFP* cameraFP;
	CameraTP* cameraTP;

	bool vKeyPressed = false;
	bool pKeyPressed = false;

	bool isGameMode = false;
	bool isFPmode = false;
	//bool isTPmode = false;

	D3D11_VIEWPORT viewPort;
};

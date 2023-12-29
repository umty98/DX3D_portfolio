#pragma once

class Time : public Singleton<Time>
{
	friend class Singleton;
private:
	Time();
	~Time();

public:
	void Update();
	void Render();

	static float Delta() { return timeElapsed; }

	UINT GetFPS() { return frameRate; }

	bool SetTimer(double& time, double updateTime);
	bool TimeDelay(double& time, double delaytime, bool& isRecoiling);

private:
	static float timeElapsed;

	float timeScale;

	INT64  curTick;
	INT64 lastTick;
	INT64 ticksPerSecond;

	UINT frameCount;
	UINT frameRate;

	float runningTime;

	float scanningRate;

	float oneSecCount;
};

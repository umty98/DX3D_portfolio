#pragma once

#define MAX_CHANNEL 10 //소리가 10개 이상 필요하면 숫자를 늘리면 됨

struct SoundInfo
{
	SoundInfo()
	{
	}

	~SoundInfo()
	{
		sound->release();
	}

	Sound* sound;
	Channel* channel;
};

class SoundManager
{
private:
	SoundManager();
	~SoundManager();

public:
	static void Create()
	{
		if (instance == nullptr)
			instance = new SoundManager;
	}

	static void Delete()
	{
		if (instance != nullptr)
			delete instance;
	}

	static SoundManager* GetInstance()
	{
		if (instance == nullptr)
			instance = new SoundManager;

		return instance;
	}

	void Update();

	void AddSound(string key, string path, bool stream);

	void   Play(string key, float volume = 1.0f);
	void   Stop(string key);
	void  Pause(string key);
	void Resume(string key);

private:
	static SoundManager* instance;

	map<string, SoundInfo*> sounds;

	System* system;
};

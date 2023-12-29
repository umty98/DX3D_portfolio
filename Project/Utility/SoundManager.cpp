#include "Framework.h"
#include "SoundManager.h"

SoundManager* SoundManager::instance = nullptr;

//struct Node
//{
//};
//
//Node* GetNode()
//{
//	return new Node();
//}
//
//Node* node = GetNode();
//
//void GetNode(Node** node)
//{
//	*node = new Node();
//}

SoundManager::SoundManager()
{
	System_Create(&system);
	system->init(MAX_CHANNEL, FMOD_INIT_NORMAL, nullptr);
}

SoundManager::~SoundManager()
{
	system->release();

	sounds.clear();
}

void SoundManager::Update()
{
	system->update();
}

void SoundManager::AddSound(string key, string path, bool stream)
{
	SoundInfo* info = new SoundInfo;

	if (stream)
	{
		system->createStream(path.c_str(), FMOD_LOOP_NORMAL, nullptr, &info->sound);
	}
	else
	{
		system->createSound(path.c_str(), FMOD_DEFAULT, nullptr, &info->sound);
	}

	sounds.emplace(key, info);
}

void SoundManager::Play(string key, float volume)
{
	if (sounds.count(key) == 0)
		return;

	system->playSound(sounds[key]->sound, nullptr, false, &sounds[key]->channel);

	sounds[key]->channel->setVolume(volume);
}

void SoundManager::Stop(string key)
{
	if (sounds.count(key) == 0)
		return;

	sounds[key]->channel->stop();
}

void SoundManager::Pause(string key)
{
	if (sounds.count(key) == 0)
		return;

	sounds[key]->channel->setPaused(true);
}

void SoundManager::Resume(string key)
{
	if (sounds.count(key) == 0)
		return;

	sounds[key]->channel->setPaused(false);
}

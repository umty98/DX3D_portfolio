#pragma once

class PosterManager
{
public:
	PosterManager(UINT poolCount, Terrain* terrain);
	~PosterManager();

	void Update();
	void Render();
	void PostRender();
	void Move();

	void SpawnPoster();

	void SpawnAllPosters();

	vector<Poster*>& GetPosters() { return posters; }

	void RemovePoster(Poster* poster);

	void SetAllPosterFalse();

	bool& GetIsPlay() { return isPlay; }
private:
	vector<Poster*> posters;
	Terrain* posterTerrain;
	float maxZ = 0;
	bool isPlay = false;

};


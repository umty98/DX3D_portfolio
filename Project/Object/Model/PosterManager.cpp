#include "Framework.h"
#include "PosterManager.h"


PosterManager::PosterManager(UINT poolCount, Terrain* terrain)
{
	srand(time(NULL));

	maxZ = terrain->GetY();

	posterTerrain = terrain;

	for (UINT i = 0; i < poolCount; i++)
	{
		float x = Random(5.0f, terrain->GetX() / 2-5.0f);
		float z = Random(5.0f, terrain->GetY()-5.0f);
		float y = terrain->GetHeight(Vector3(x, 0.0f, z));

		Poster* poster = new Poster;

		poster->GetMoveDirection() = (Random(0, 1000) > 500) ? -1.0f : 1.0f;   // 움직임 방향
		poster->GetMoveSpeed() = Random(10.0f, 50.0f);  // 움직임 속도
		//posters[i]->IsActive() = false;
		//posters[i]->GetTransform()->translation.x = 3 * i;
		//poster->GetTransform()->translation.x = 3 * i;
		poster->GetTransform()->translation = Vector3(x, y + 5.0f, z);
		poster->IsActive() = false;
		posters.push_back(poster);
	}
}

PosterManager::~PosterManager()
{
	for (Poster* p : posters)
		delete p;
	posters.clear();
}

void PosterManager::Update()
{
	
	static double time = 0.0;
	double updateTime = Random(1.0, 3.0);


	if (isPlay)
	{
		if (Time::GetInstance()->SetTimer(time, updateTime))
		{
			SpawnPoster();
		}
	}
	
	

	for (Poster* p : posters)
	{
		if(p->IsActive())
			p->Update();
	}

	Move();

}

void PosterManager::Render()
{

	for (Poster* p : posters)
	{
		if (p->IsActive())
		{
			p->Render();
		}

	}
}

void PosterManager::PostRender()
{
	for (Poster* p : posters)
		p->PostRender();
}

void PosterManager::Move()
{
	for (Poster* p : posters)
	{
		if (!p->IsActive())
			continue;

		// 현재 위치를 가져옴
		Vector3 pos = p->GetTransform()->translation;

		// 움직임 방향을 결정 (범위를 벗어나면 반대로)
		if (pos.z <= 5.0f || pos.z >= maxZ-5.0f)
		{
			p->GetMoveDirection() *= -1.0f;  // 방향 전환
		}

		// 위치 업데이트
		pos.z += p->GetMoveSpeed() * p->GetMoveDirection() * Time::Delta();
		p->GetTransform()->translation = pos;
	}
}

void PosterManager::SpawnPoster()
{
	for (Poster* p : posters)
	{
		if (!p->IsActive())
		{
			float x = Random(1.0f, posterTerrain->GetX() / 2-1);
			float z = Random(1.0f, posterTerrain->GetY()-1);
			float y = posterTerrain->GetHeight(Vector3(x, 0.0f, z));
			p->GetTransform()->translation = Vector3(x, y + 5.0f, z);
			p->GetPosterCollider()->SetColor(0, 1, 0);

			p->IsActive() = true;
			break;
		}
	}
}

void PosterManager::SpawnAllPosters()
{
	for (Poster* p : posters)
	{
		p->IsActive() = true;
	}

}

void PosterManager::RemovePoster(Poster* poster)
{
	auto iter = std::find(posters.begin(), posters.end(), poster);
	if (iter != posters.end())
	{
		delete* iter;
		posters.erase(iter);
	}
}

void PosterManager::SetAllPosterFalse()
{
	for (Poster* p : posters)
	{
		p->IsActive() = false;
	}
}

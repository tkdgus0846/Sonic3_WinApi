#pragma once



class RandomManager
{
	DECLARE_SINGLE(RandomManager)

public:
	void Init();
	void Update();
	void Release();

	Vec2 CreateRandPointInArea(const RECT& _rct);
};

#define RANDOM RandomManager::GetInstance()
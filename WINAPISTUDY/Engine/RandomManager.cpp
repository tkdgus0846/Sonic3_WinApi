#include "pch.h"
#include "RandomManager.h"

void RandomManager::Init()
{
	srand(static_cast<unsigned int>(time(0)));
}


void RandomManager::Update()
{
}

void RandomManager::Release()
{
}

Vec2 RandomManager::CreateRandPointInArea(const RECT& _rct)
{
	Vec2 resultPoint;

	LONG left = _rct.left;
	LONG right = _rct.right;
	LONG top = _rct.top;
	LONG bottom = _rct.bottom;


	resultPoint.x = left + (rand() % (right - left));
	resultPoint.y = top + (rand() % (bottom - top));

	return resultPoint;
}


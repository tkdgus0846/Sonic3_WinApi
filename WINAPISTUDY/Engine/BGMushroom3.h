#pragma once
#include "ImageObject.h"
class BGMushroom3 : public ImageObject
{
public:
	BGMushroom3(int ZOrder, const TCHAR* frameKey, int coordX, int coordY, const Vec2& Pos, const Vec2& Size, const Vec2& RenderSize, bool collisionOn = false, int splitMode = 0);

	virtual void Render() override;
	virtual void Late_Update() override;
};
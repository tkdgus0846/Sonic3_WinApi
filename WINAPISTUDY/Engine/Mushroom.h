#pragma once
#include "ImageObject.h"

#define MUSHROOM_JUMP_POWER 5.f;

class Mushroom : public ImageObject
{
public:
	Mushroom(int ZOrder, const TCHAR* frameKey, int coordX, int coordY, const Vec2& Pos, const Vec2& Size, const Vec2& RenderSize, bool collisionOn = false, int splitMode = 0);

	virtual void Render() override;
	virtual int Update() override;
	virtual void Late_Update() override;

public:
	virtual void Collision_Up(Collider* collider, const int& px, const int& py);
	virtual void Collision_Down(Collider* collider, const int& px, const int& py);
	virtual void Collision_Left(Collider* collider, const int& px, const int& py);
	virtual void Collision_Right(Collider* collider, const int& px, const int& py);

private:
	float m_fJumpPower;
};


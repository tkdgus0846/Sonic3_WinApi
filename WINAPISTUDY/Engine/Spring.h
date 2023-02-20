#pragma once
#include "ImageObject.h"

class Spring : public ImageObject
{
public:
	Spring(int ZOrder, const TCHAR* frameKey, int coordX, int coordY, const Vec2& Pos, const Vec2& Size, const Vec2& RenderSize, bool collisionOn = false, int Diagonal = 0);

	virtual void Render();
	virtual void Late_Update();

public:
	virtual void Collision_Up(Collider* collider, const int& px, const int& py);
	virtual void Collision_Down(Collider* collider, const int& px, const int& py);
	virtual void Collision_Left(Collider* collider, const int& px, const int& py);
	virtual void Collision_Right(Collider* collider, const int& px, const int& py);

private:
	void Move_Spring();

private:
	bool m_bActivated;
	float m_fTimeAccel;
	float m_fTimeSpeed;
	int m_Diagonal;

};


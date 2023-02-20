#pragma once
#include "ImageObject.h"

class Sign : public ImageObject
{
public:
	Sign(int ZOrder, const TCHAR* frameKey, int coordX, int coordY, const Vec2& Pos, const Vec2& Size, const Vec2& RenderSize, bool collisionOn = false, int splitMode = 0);
	virtual ~Sign();

	virtual void Render() override;
	virtual int Update() override;
	virtual void Late_Update() override;

public:
	virtual void Collision_Up(Collider* collider, const int& px, const int& py);
	virtual void Collision_Down(Collider* collider, const int& px, const int& py);
	virtual void Collision_Left(Collider* collider, const int& px, const int& py);
	virtual void Collision_Right(Collider* collider, const int& px, const int& py);

	

private:
	void Jump();
	void Move_Shine_Frame();

	FRAME m_ShineFrame;
	bool m_bShineFrameEnabled;
	DWORD m_ShineTime;
	DWORD m_ShineStartTime;
	float	m_fPower;

	DWORD m_CollisionTimer;
	bool m_bLand;
	bool m_bActEnded;

	int randPosX[12];
	int randPosY[12];
};


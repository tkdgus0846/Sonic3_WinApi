#pragma once
#include "ImageObject.h"
class DropRing :
    public ImageObject
{
public:
	DropRing(int ZOrder, const TCHAR* frameKey, int coordX, int coordY, const Vec2& Pos, const Vec2& Size, const Vec2& RenderSize, float jumpPower, float speed, const Vec2& randDir, bool collisionOn = false, int splitMode = 0 );
	virtual ~DropRing();

	virtual void Render() override;
	virtual int Update() override;
	virtual void Late_Update() override;

public:
	virtual void Collision_Up(Collider* collider, const int& px, const int& py);
	virtual void Collision_Down(Collider* collider, const int& px, const int& py);
	virtual void Collision_Left(Collider* collider, const int& px, const int& py);
	virtual void Collision_Right(Collider* collider, const int& px, const int& py);

	void	Jump();

private:
	void Move_Shine_Frame();

	FRAME m_ShineFrame;
	bool m_bShineFrameEnabled;
	DWORD m_ShineTime;
	DWORD m_ShineStartTime;

	LONG m_BlinkTime;
	DWORD m_BlinkStartTime;
	bool m_bBlink;

	float		m_fPower;
};


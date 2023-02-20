#pragma once
#include "ImageObject.h"

class Meteor : public ImageObject
{
public:
	Meteor(int ZOrder, const TCHAR* frameKey, int coordX, int coordY, const Vec2& Pos, const Vec2& Size, const Vec2& RenderSize, bool collisionOn = false, int splitMode = 0);

	virtual void Render() override;
	virtual int Update() override;
	virtual void Late_Update() override;

public:
	virtual void Collision_Up(Collider* collider, const int& px, const int& py);
	virtual void Collision_Down(Collider* collider, const int& px, const int& py);
	virtual void Collision_Left(Collider* collider, const int& px, const int& py);
	virtual void Collision_Right(Collider* collider, const int& px, const int& py);
	
	virtual void Hit_With_SkillRing();

private:
	void MeteorOutScreen();
	void RenderBigMeteorDestroyed();
	void RenderNormalMeteorDestroyed();
	void RenderSmallMeteorDestroyed();

	int m_RandomIndex;
	wstring m_FrameKeyArr[3];
	Vec2 m_RenderSizeArr[3];
	bool m_bDestroyed;

	
};


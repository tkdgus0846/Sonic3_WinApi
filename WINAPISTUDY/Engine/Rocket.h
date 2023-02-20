#pragma once
#include "ImageObject.h"

class Creature;

struct RocketInfo
{
	Vec2 Size;
	Vec2 RenderPos;
	Vec2 Offset;
};

class Rocket : public ImageObject
{
public:
	Rocket(int ZOrder, const Vec2& Pos, const Vec2& Dir, bool guided, Creature* target);
	virtual ~Rocket();

	virtual void Render() override;
	virtual int Update() override;
	virtual void Late_Update() override;

public:
	virtual void Collision_Up(Collider* collider, const int& px, const int& py);
	virtual void Collision_Down(Collider* collider, const int& px, const int& py);
	virtual void Collision_Left(Collider* collider, const int& px, const int& py);
	virtual void Collision_Right(Collider* collider, const int& px, const int& py);

	virtual void Hit_With_SkillRing();

	void	Change_Direction_ByTarget();
	void	 Change_Info_ByDirection();
	void	Change_Flame();
	void	ColliderOffset();
	void	Move_Explosion_Frame();

private:
	bool m_bGuided;
	bool m_bDestroyed;
	int m_FlameKeyIndex;
	Vec2 m_FlameOffsetArr[8];
	wstring m_FlameKeyArr[8];
	bool m_bRenderFlame;
	DWORD m_ChangeFlameKeyTime;
	Creature* m_pTarget;

	RocketInfo m_RocketInfo;
	pair<Vec2, Vec2>		m_ColOffset[CO_END];

	void RocketOutScreen();
};


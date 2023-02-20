#pragma once
#include "ImageObject.h"
class Creature;

class Bullet : public ImageObject
{
public:
	Bullet(int ZOrder, const Vec2& Pos, const Vec2& Dir, bool guided, Creature* target);
	virtual ~Bullet();

	virtual void Render() override;
	virtual int Update() override;
	virtual void Late_Update() override;

public:
	virtual void Collision_Up(Collider* collider, const int& px, const int& py);
	virtual void Collision_Down(Collider* collider, const int& px, const int& py);
	virtual void Collision_Left(Collider* collider, const int& px, const int& py);
	virtual void Collision_Right(Collider* collider, const int& px, const int& py);

private:
	void MissileOutScreen();

	bool m_bDestroyed;
};


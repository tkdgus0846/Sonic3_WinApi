#pragma once
#include "ImageObject.h"

enum ITEMSTATE
{
	IS_RING,
	IS_LIFE,
	IS_FIRE,
	IS_WATER,
	IS_END
};
class Item : public ImageObject
{
public:
	Item(int ZOrder, const TCHAR* frameKey, int coordX, int coordY, const Vec2& Pos, const Vec2& Size, const Vec2& RenderSize, bool collisionOn = false, int splitMode = 0);

	virtual void Render() override;
	virtual int Update() override;
	virtual void Late_Update() override;

public:
	virtual void Collision_Up(Collider* collider, const int& px, const int& py);
	virtual void Collision_Down(Collider* collider, const int& px, const int& py);
	virtual void Collision_Left(Collider* collider, const int& px, const int& py);
	virtual void Collision_Right(Collider* collider, const int& px, const int& py);

	void Activate(class Player* player);

private:
	bool m_bDestroyed;
	ITEMSTATE m_eItemState;
};


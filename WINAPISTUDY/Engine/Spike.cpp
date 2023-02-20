#include "pch.h"
#include "Spike.h"
#include "Creature.h"
#include "Collider.h"

Spike::Spike(int ZOrder, const TCHAR* frameKey, int coordX, int coordY, const Vec2& Pos, const Vec2& Size, const Vec2& RenderSize, bool collisionOn, int splitMode) :
	ImageObject(ZOrder, frameKey, coordX, coordY, Pos, Size, RenderSize, collisionOn, splitMode)
{
	m_eObstacleType = OT_SPIKE;
}

void Spike::Render()
{
	RENDER->TransparentImage
	(
		m_FrameKeyArr,
		m_Area.left,
		m_Area.top,
		m_Size.x,
		m_Size.y,
		(m_CoordX * 31.f),
		(m_CoordY * 0.f),
		m_RenderSize.x,
		m_RenderSize.y,
		RGB(255, 0, 255)
	);
}

void Spike::Collision_Up(Collider* collider, const int& px, const int& py)
{
	if (collider == nullptr) return;
	if (m_bCollisionOn == false) return;

	Creature* player = dynamic_cast<Creature*>(collider->GetOwner());
	if (player == nullptr) return;

	if (m_CoordX == 0)
	{
		player->GetDamaged(1);
	}
	else
	{
		Vec2 Pos = player->GetPos();

		if (player->GetJumping())
		{
			collider->SetInCollision(false);
		}

		if (player->GetFalling() || collider->GetInCollision() == true)
		{
			//cout << py << endl;
			collider->GetOwner()->SetPos(Vec2(Pos.x, Pos.y - py - 7.f));
			collider->SetInCollision(true);
			collider->GetOwner()->SetFalling(false);
			collider->GetOwner()->SetJumpTime(0.f);
			collider->GetOwner()->SetJumping(false);
		}
	}
}

// up 0 left 1 right 2
void Spike::Collision_Down(Collider* collider, const int& px, const int& py)
{
	if (collider == nullptr) return;
	if (m_bCollisionOn == false) return;

	Creature* player = dynamic_cast<Creature*>(collider->GetOwner());
	if (player == nullptr) return;

	Vec2 Pos = player->GetPos();
	player->SetPos(Vec2(Pos.x, Pos.y + py + 7.f));
}

void Spike::Collision_Left(Collider* collider, const int& px, const int& py)
{
	if (collider == nullptr) return;
	if (m_bCollisionOn == false) return;

	Creature* player = dynamic_cast<Creature*>(collider->GetOwner());
	if (player == nullptr) return;

	if (m_CoordX == 1)
	{
		player->GetDamaged(1);
	}
	else
	{
		Vec2 Pos = player->GetPos();
		player->SetPos(Vec2(Pos.x - px - 7.f, Pos.y));
		player->SetSpeed(0);
	}
}

void Spike::Collision_Right(Collider* collider, const int& px, const int& py)
{
	if (collider == nullptr) return;
	if (m_bCollisionOn == false) return;

	Creature* player = dynamic_cast<Creature*>(collider->GetOwner());
	if (player == nullptr) return;

	if (m_CoordX == 2)
	{
		player->GetDamaged(1);
	}
	else
	{
		Vec2 Pos = player->GetPos();
		player->SetPos(Vec2(Pos.x + px + 8.f, Pos.y));
		player->SetSpeed(0);
	}
}

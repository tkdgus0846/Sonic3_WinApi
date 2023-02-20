#include "pch.h"
#include "Item.h"
#include "../Client/Player.h"
#include "Collider.h"
#include "SoundManager.h"

Item::Item(int ZOrder, const TCHAR* frameKey, int coordX, int coordY, const Vec2& Pos, const Vec2& Size, const Vec2& RenderSize, bool collisionOn, int splitMode)
	:
	ImageObject(ZOrder, frameKey, coordX, coordY, Pos, Size, RenderSize, collisionOn, splitMode)
{
	m_Frame.iFrameStart = 0;
	m_Frame.iFrameEnd = 3;
	m_Frame.iMotion = coordY;
	m_Frame.dwTime = GetTickCount();
	m_Frame.dwSpeed = 50;
	m_Frame.repeat = true;

	m_bDestroyed = false;

	switch (coordY)
	{
	case 0:
		m_eItemState = IS_RING;
		break;
	case 1:
		m_eItemState = IS_LIFE;
		break;
	case 2:
		m_eItemState = IS_FIRE;
		break;
	case 3:
		m_eItemState = IS_WATER;
		break;
	default:
		m_eItemState = IS_RING;
		break;
	}
}

void Item::Render()
{
	if (m_bDestroyed == false)
	{
		RENDER->Animation_TransparentBlt
		(
			m_FrameKeyArr,
			m_Frame,
			m_Area.left,
			m_Area.top,
			m_Size.x,
			m_Size.y,
			m_RenderSize.x,
			m_RenderSize.y,
			RGB(255, 0, 255),
			0, 0
		);
	}
	else
	{
		RENDER->TransparentImage
		(
			m_FrameKeyArr,
			m_Area.left, m_Area.top,
			m_Size.x, m_Size.y,
			0, 5 * m_RenderSize.y,
			m_RenderSize.x, m_RenderSize.y,
			RGB(255, 0, 255)
		);
	}
	
}

int Item::Update()
{
	return 0;
}

void Item::Late_Update()
{
	Move_Frame();
}

void Item::Collision_Up(Collider* collider, const int& px, const int& py)
{
	if (m_bDestroyed == true) return;
	if (GetCollisionWithCollider() == false)
	{
		if (collider == nullptr) return;
		
		Player* player = dynamic_cast<Player*>(collider->GetOwner());
		if (player == nullptr) return;
		
		if (player->GetCurState() == PS_JUMP)
		{
			m_bDestroyed = true;
			STOP_PLAY_SOUND(L"StompingMonster.wav", CHANNELID::SOUND_OBJ, 1.0f);
			Activate(player);
			player->SetJumping(true);
			player->SetJumpTime(0.f);
			player->SetJumpPower(5.f);
			player->AddScore(2000);
		}
	}
}

void Item::Collision_Down(Collider* collider, const int& px, const int& py)
{
	return;
}

void Item::Collision_Left(Collider* collider, const int& px, const int& py)
{
	if (m_bDestroyed == true) return;
	Creature* player = dynamic_cast<Creature*>(collider->GetOwner());
	if (player == nullptr) return;

	Vec2 Pos = player->GetPos();
	player->SetPos(Vec2(Pos.x - px - 7.f, Pos.y));
	player->SetSpeed(0);
}

void Item::Collision_Right(Collider* collider, const int& px, const int& py)
{
	if (m_bDestroyed == true) return;
	Creature* player = dynamic_cast<Creature*>(collider->GetOwner());
	if (player == nullptr) return;

	Vec2 Pos = player->GetPos();
	player->SetPos(Vec2(Pos.x + px + 8.f, Pos.y));
	player->SetSpeed(0);
}

void Item::Activate(Player* player)
{
	switch (m_eItemState)
	{
	case IS_RING:
		player->AddRings(200);
		break;
	case IS_LIFE:
		player->AddLife(2);
		break;
	case IS_FIRE:
		break;
	case IS_WATER:
		break;
	default:
		break;
	}
}

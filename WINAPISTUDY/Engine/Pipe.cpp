#include "pch.h"
#include "Pipe.h"
#include "../Client/Player.h"
#include "Collider.h"
#include "SoundManager.h"

Pipe::Pipe(int ZOrder, const TCHAR* frameKey, int coordX, int coordY, const Vec2& Pos, const Vec2& Size, const Vec2& RenderSize, bool collisionOn, int splitMode) :
	ImageObject(ZOrder, frameKey, coordX, coordY, Pos, Size, RenderSize, collisionOn, splitMode)
{
	m_pPlayer = nullptr;
}

void Pipe::Render()
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

void Pipe::Collision_Up(Collider* collider, const int& px, const int& py)
{
	if (GetCollisionWithCollider() == false)
	{

		// 속도 처리.
		if (collider == nullptr) return;
		if (m_bCollisionOn == false) return;
		Player* player = dynamic_cast<Player*>(collider->GetOwner());
		if (player == nullptr) return;

		if (m_pPlayer == nullptr)
		{
			m_pPlayer = player;
			player->SetOnThePipe(true);
		}
			
		if (INPUT->Key_Pressing(VK_DOWN))
		{
			
			m_pPlayer->SetInToPipe(true);
			if (IS_PLAYING(CHANNELID::SOUND_SONIC) == FALSE)
			{
				PLAY_SOUND(L"PipeSound.wav", CHANNELID::SOUND_SONIC, 0.5f);
			}
		}

		if (m_pPlayer->GetInToPipe())
		{
			if (m_pPlayer->GetPos().y >= m_Pos.y - 30)
			{
				if (m_Pos.x >= -587 * 64 && m_Pos.x <= -574 * 64)
				{
					SCENE->ActClear();
					m_pPlayer->SetInToPipe(false);
					//SCENE->SelectScene("Scene03");
					return;
				}
				m_pPlayer->SetPos(Vec2(-701 * 64, 3 *  64));
				STOP_SOUND(CHANNELID::SOUND_BGM);
				PLAY_BGM(L"MarioBGM.wav", 1.0f);
				m_pPlayer->SetInToPipe(false);
				m_pPlayer->SetOutToPipe(true);
				m_pPlayer->SetMarioMode(true);
			}
		}

		if (m_pPlayer->GetActClear() == true) return;
		if (m_pPlayer->GetInToPipe() == false)
		{
			
			Vec2 playerPos = player->GetPos();
			player->SetPos({ playerPos.x,playerPos.y - py - 7 });
			collider->SetInCollision(true);
			collider->GetOwner()->SetFalling(false);
			collider->GetOwner()->SetJumpTime(0.f);
			collider->GetOwner()->SetJumping(false);
		}

		
		//collider->SetActive(false);
		//player->SetPos(Vec2(-200, 0));
	}
}

void Pipe::Collision_Down(Collider* collider, const int& px, const int& py)
{
	//if (m_pPlayer)
	//{
	//	Vec2 playerPos = m_pPlayer->GetPos();
	//	m_pPlayer->SetPos({ playerPos.x,playerPos.y - 8 });
	//	/*collider->SetInCollision(true);
	//	collider->GetOwner()->SetFalling(false);
	//	collider->GetOwner()->SetJumpTime(0.f);
	//	collider->GetOwner()->SetJumping(false);*/
	//}
	
}

void Pipe::Collision_Left(Collider* collider, const int& px, const int& py)
{
}

void Pipe::Collision_Right(Collider* collider, const int& px, const int& py)
{
}

#include "pch.h"
#include "SpikeBall.h"
#include "../Client/Player.h"
#include "Collider.h"
#include "CollisionManager.h"
#include "SoundManager.h"

// 90*64에 플레이어가 도착했을때 떨어트려서 쭉 굴려야한다 ~~~

SpikeBall::SpikeBall(int ZOrder, const TCHAR* frameKey, int coordX, int coordY, const Vec2& Pos, const Vec2& Size, const Vec2& RenderSize, bool collisionOn, int splitMode) :
	ImageObject(ZOrder, frameKey, coordX, coordY, Pos, Size, RenderSize, collisionOn, splitMode)
{
	m_ZOrder = 11;
	m_Frame.iFrameStart = 0;
	m_Frame.iFrameEnd = 2;
	m_Frame.iMotion = 0;
	m_Frame.dwTime = GetTickCount();
	m_Frame.dwSpeed = 100;
	m_Frame.repeat = true;

	m_dwSpikeBallSpeedTime = 100;
	m_dwSpikeBallSoundSpeed = 500;

	m_CurLine = nullptr;
	m_Dir = { 0,1 };
	m_fSpeed = 38.f;
	m_eDirection = OD_RIGHT;

	m_dwSpikeBallSoundTime = GetTickCount();
	m_bNextQuiz = true;

	m_dwNextQuizTime = GetTickCount();
	m_TimeAttackBarTime = GetTickCount();

	m_TimeAttackBarSpeed[0] = 1;
	m_TimeAttackBarSpeed[1] = 30;

	COLLISION->NewCollider(this, "SpikeBallCollider", { 0,0 }, m_Size);
	m_fAddSpeed = 0.f;
	m_TimeAttackBarSize = ATTACK_BAR_SIZE;
	Update_Rect();
}

SpikeBall::~SpikeBall()
{
	COLLISION->RemoveCollider(GetComponent("SpikeBallCollider"));
}

void SpikeBall::Render()
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

	if (m_CurLine)
	{
		for (int i = 0; i < FRAGMENT_NUM; i++)
		{
			RENDER->TransparentImage_NoScroll
			(
				L"FragmentEffect",
				m_FragmentInfo[i].pos.x, m_FragmentInfo[i].pos.y,
				13 * 3, 8 * 3,
				(m_FragmentInfo[i].coordX * 13), 0,
				13, 8,
				RGB(255, 0, 255)
			);
		}
		
	}

	Render_CommandQuiz();
	Render_TimeAttackBar();
}

int SpikeBall::Update()
{
	bool dead = ImageObject::Update();
	if (dead) return OBJ_DEAD;

	if (m_CurLine)
	{
		if (m_dwSpikeBallSoundTime + m_dwSpikeBallSoundSpeed <= GetTickCount())
		{
			STOP_PLAY_SOUND(L"SpikeBall.wav", CHANNELID::SOUND_OBJ, 0.4f);
			m_Frame.dwSpeed = m_dwSpikeBallSpeedTime;
			SetFragmentInfo();
			m_dwSpikeBallSoundTime = GetTickCount();
		}
		LINELAYER layer = m_CurLine->GetLayer();

		Vec2 left = m_CurLine->GetPoint(LINE_LEFT);
		Vec2 right = m_CurLine->GetPoint(LINE_RIGHT);
		Vec2 top = m_CurLine->GetPoint(LINE_TOP);
		Vec2 bottom = m_CurLine->GetPoint(LINE_BOTTOM);

		if (m_eDirection == OD_RIGHT)
		{
			if (m_CurLine->GetLineIncline() >= 8.f || m_CurLine->GetLineIncline() <= -8.f)
			{
				if (layer == LL_FORWARD)
					m_Dir = top - bottom;
				else if (layer == LL_REVERSE)
					m_Dir = bottom - top;
			}
			else
			{
				if (layer == LL_FORWARD)
					m_Dir = right - left;
				else if (layer == LL_REVERSE)
					m_Dir = left - right;
			}
			m_Dir.Normalize();
		}

		Key_Input();
	}

	m_Pos = m_Pos + m_Dir * m_fSpeed;

	Update_Rect();
}

void SpikeBall::Late_Update()
{
	Move_Frame();
	if (m_CurLine)
	{
		Fragment_Jump();
		Move_FragmentInfo();
		Generate_CommandQuiz();
		TimeLimit_CommandQuiz();
		
	}
}

void SpikeBall::Collision_Up(Collider* collider, const int& px, const int& py)
{
	if (GetCollisionWithCollider() == false)
	{
		Collision_Enter();
		// 속도 처리.
		if (collider == nullptr) return;
		if (m_bCollisionOn == false) return;
		Player* player = dynamic_cast<Player*>(collider->GetOwner());
		if (player == nullptr) return;
		player->SetPos(Vec2(79 * 64, -205 * 64));
		player->SetSpeed(0.f);

		STOP_SOUND(CHANNELID::SOUND_OBJ);
		PLAY_SOUND(L"ReturnPos2.wav", CHANNELID::SOUND_EFFECT, 1.0f);

		//player->GetDamaged(1);
	}
}

void SpikeBall::Collision_Down(Collider* collider, const int& px, const int& py)
{
	Collision_Up(collider, px, py);
}

void SpikeBall::Collision_Left(Collider* collider, const int& px, const int& py)
{
	Collision_Up(collider, px, py);
}

void SpikeBall::Collision_Right(Collider* collider, const int& px, const int& py)
{
	Collision_Up(collider, px, py);
}

void SpikeBall::Fragment_Jump()
{
	for (int i = 0; i < FRAGMENT_NUM; i++)
	{
		bool& bJumping = m_FragmentInfo[i].jumping;
		float& fallingSpeed = m_FragmentInfo[i].fallingSpeed;
		float& power = m_FragmentInfo[i].power;
		float& jumpTime = m_FragmentInfo[i].jumpTime;
		Vec2& pos = m_FragmentInfo[i].pos;

		if (bJumping)
		{
			fallingSpeed = power * jumpTime - (6.8f * jumpTime * jumpTime) * 0.5f;

			if (fallingSpeed <= -15.f) fallingSpeed = -15.f;
			pos.y -= fallingSpeed;
			
			jumpTime += 0.2f;
		}
		else
		{
			fallingSpeed = -10.f;
			pos.y -= fallingSpeed;
		}
	}
	
}

void SpikeBall::SetFragmentInfo()
{
	for (int i = 0; i < FRAGMENT_NUM; i++)
	{
		// power 3~8
		m_FragmentInfo[i].power = 5; // rand() % 3 + 1;
		m_FragmentInfo[i].dir.x = rand() % 10 - 6;
		m_FragmentInfo[i].dir.y = rand() % 4 - 4;
		m_FragmentInfo[i].dir.Normalize();

		// speed 5~10
		m_FragmentInfo[i].speed = rand() % 10 + 20;
		m_FragmentInfo[i].coordX = rand() % 4;
		m_FragmentInfo[i].jumpTime = 0.f;
		m_FragmentInfo[i].fallingSpeed = 0.f;
		m_FragmentInfo[i].jumping = true;

		RECT rct = { 200, 200, 300, 300 };
		m_FragmentInfo[i].pos = RANDOM->CreateRandPointInArea(rct);
		
	}
}

void SpikeBall::Move_FragmentInfo()
{
	for (int i = 0; i < FRAGMENT_NUM; i++)
	{
		m_FragmentInfo[i].pos = m_FragmentInfo[i].pos + m_FragmentInfo[i].dir * m_FragmentInfo[i].speed;
	}
}

void SpikeBall::Render_CommandQuiz()
{
	// 30 30 2 2

	if (m_CommandQuiz.empty()) return;

	for (int i = 0; i < m_CommandQuiz.size(); i++)
	{
		wstring frameKey;

		if (m_CommandQuiz[i].state == 0) frameKey = L"BoxFont_Black";
		else if (m_CommandQuiz[i].state == -1) frameKey = L"BoxFont_Red";
		else if (m_CommandQuiz[i].state == 1) frameKey = L"BoxFont_Green";

		RENDER->TransparentImage_NoScroll
		(
			frameKey,
			m_CommandQuiz[i].pos.x, m_CommandQuiz[i].pos.y,
			30 * 2, 30 * 2,
			m_CommandQuiz[i].renderPos.x, m_CommandQuiz[i].renderPos.y,
			30, 30,
			RGB(255, 0, 255)
		);
	}

}

void SpikeBall::Render_TimeAttackBar()
{
	RENDER->SetPen(RGB(0x5c, 0x94, 0xfc));
	RENDER->SetBrush(RGB(0x5c, 0x94, 0xfc));
	RENDER->DrawRect(250, 50, 250 + m_TimeAttackBarSize, 80);
	RENDER->SetPen(RGB(0, 0, 0), PENTYPE::Solid, 4);
	RENDER->SetBrush(RGB(1,2,3),BRUSHTYPE::Null);
	RENDER->DrawRect(250, 50, 250 + ATTACK_BAR_SIZE, 80);
}

void SpikeBall::Generate_CommandQuiz()
{
	if (m_bNextQuiz == false) return;
	
	if (m_dwNextQuizTime + 500 <= GetTickCount())
	{
		m_CommandQuiz.clear();
		m_CurQuizIndex = 0;
		m_bNextQuiz = false;
		int QuizSize;
		int randNum = rand() % 2;

		switch (randNum)
		{
		case 0:
			QuizSize = 4;
			m_TimeAttackBarSpeedIndex = 0;
			break;
		case 1:
			m_TimeAttackBarSpeedIndex = 1;
			QuizSize = 8;
			break;
		}

		Vec2 StartPos = { 100,100 };

		for (int i = 0; i < QuizSize; i++)
		{
			int randAlpha = rand() % 4;
			Vec2 renderPos;

			// 0 0 32 0
			switch (randAlpha)
			{
			case 0:
				renderPos = { 288,32 };
				m_CommandQuiz.push_back({ 'W', 0, StartPos, renderPos });
				break;
			case 1:
				renderPos = { 0,0 };
				m_CommandQuiz.push_back({ 'A', 0, StartPos, renderPos });
				break;
			case 2:
				renderPos = { 160,32 };
				m_CommandQuiz.push_back({ 'S', 0, StartPos, renderPos });
				break;
			case 3:
				renderPos = { 96,0 };
				m_CommandQuiz.push_back({ 'D', 0, StartPos, renderPos });
				break;
			}

			StartPos.x = StartPos.x + 80;
		}

		m_TimeAttackBarTime = GetTickCount();
		m_TimeAttackBarSize = 400;
	}
	

	
	
}

void SpikeBall::TimeLimit_CommandQuiz()
{
	if (m_bNextQuiz == true) return;

	if (m_TimeAttackBarTime + m_TimeAttackBarSpeed[m_TimeAttackBarSpeedIndex] <= GetTickCount())
	{
		m_TimeAttackBarSize -= 5;
		if (m_TimeAttackBarSize < 0)
		{
			m_TimeAttackBarSize = 0;
			
			NextQuiz(false);

			STOP_PLAY_SOUND(L"WrongChoice.wav", CHANNELID::SOUND_SUSTAIN, 1.0f);
		}
		m_TimeAttackBarTime = GetTickCount();
	}
}

void SpikeBall::Key_Input()
{
	if (m_bNextQuiz == true) return;
	if (m_CommandQuiz.empty()) return;

	if (INPUT->Key_Up(VK_W))
	{
		if (VK_W == m_CommandQuiz[m_CurQuizIndex].alpha)
		{
			m_CommandQuiz[m_CurQuizIndex].state = 1;
			m_CurQuizIndex += 1;
			STOP_PLAY_SOUND(L"CorrectChoice.wav", CHANNELID::SOUND_SUSTAIN, 1.0f);
			NextQuiz(true);

		}
		else
		{
			m_CommandQuiz[m_CurQuizIndex].state = -1;
			STOP_PLAY_SOUND(L"WrongChoice.wav", CHANNELID::SOUND_SUSTAIN, 1.0f);
			NextQuiz(false);
		}
		
	}
	else if (INPUT->Key_Up(VK_S))
	{
		if (VK_S == m_CommandQuiz[m_CurQuizIndex].alpha)
		{
			m_CommandQuiz[m_CurQuizIndex].state = 1;
			m_CurQuizIndex += 1;
			STOP_PLAY_SOUND(L"CorrectChoice.wav", CHANNELID::SOUND_SUSTAIN, 1.0f);
			NextQuiz(true);
		}
		else
		{
			m_CommandQuiz[m_CurQuizIndex].state = -1;
			STOP_PLAY_SOUND(L"WrongChoice.wav", CHANNELID::SOUND_SUSTAIN, 1.0f);
			NextQuiz(false);
		}
	}
	else if (INPUT->Key_Up(VK_A))
	{
		if (VK_A == m_CommandQuiz[m_CurQuizIndex].alpha)
		{
			m_CommandQuiz[m_CurQuizIndex].state = 1;
			m_CurQuizIndex += 1;
			STOP_PLAY_SOUND(L"CorrectChoice.wav", CHANNELID::SOUND_SUSTAIN, 1.0f);
			NextQuiz(true);
		}
		else
		{
			m_CommandQuiz[m_CurQuizIndex].state = -1;
			STOP_PLAY_SOUND(L"WrongChoice.wav", CHANNELID::SOUND_SUSTAIN, 1.0f);
			NextQuiz(false);
		}
	}
	else if (INPUT->Key_Up(VK_D))
	{
		if (VK_D == m_CommandQuiz[m_CurQuizIndex].alpha)
		{
			m_CommandQuiz[m_CurQuizIndex].state = 1;
			m_CurQuizIndex += 1;
			STOP_PLAY_SOUND(L"CorrectChoice.wav", CHANNELID::SOUND_SUSTAIN, 1.0f);
			NextQuiz(true);
		}
		else
		{
			m_CommandQuiz[m_CurQuizIndex].state = -1;
			STOP_PLAY_SOUND(L"WrongChoice.wav", CHANNELID::SOUND_SUSTAIN, 1.0f);
			NextQuiz(false);
		}
	}
	
}

void SpikeBall::NextQuiz(bool success)
{
	if (success)
	{
		if (m_CurQuizIndex == m_CommandQuiz.size())
		{
			m_bNextQuiz = true;
			m_fAddSpeed -= 0.1f;
			//if (m_fAddSpeed < -10.f) m_fAddSpeed = -10.f;
			m_fAddSpeed = 0.f;
			m_dwNextQuizTime = GetTickCount();
		}
	}
	else
	{
		m_bNextQuiz = true;
		m_fAddSpeed += 1.f;
		m_dwNextQuizTime = GetTickCount();
	}
	
}

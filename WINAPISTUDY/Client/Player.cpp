#include "pch.h"
#include "Player.h"
#include "Collider.h"
#include "LineManager.h"
#include "ScrollManager.h"
#include <SoundManager.h>
#include "DropRing.h"
#include "SkillRing.h"

// 관성 받을때 떨어짐 
// 충돌 범위 문제


Player::Player(const Vec2& _pos)
{
	m_bInToPipe = false;
	m_bOnThePipe = false;
	m_bOutToPipe = false;
	m_bMarioMode = false;
	m_bFlyAwayEnding = false;

	m_dwChangeColorTime = GetTickCount();
	m_SuperColor = 0;
	m_bSuperMode = false;
	m_bConvertingSuperMode = false;

	m_bActClear = false;
	m_bBreakSoundPlayed = false;
	m_HP = 1;
	m_Rings = 0;
	m_Life = 10;
	m_Score = 0;
	m_bInvincible = false;

	m_eCurState = PS_IDLE;
	m_ePreState = PS_END;
	m_eDirection = OD_RIGHT;

	m_bDecel = false;

	m_ObjID = OBJ_PLAYER;
	m_Pos = _pos;
	m_Size.x = 150.f;
	m_Size.y = 150.f;

	m_CurLine = nullptr;
	
	m_fSpeed = 0.f;
	m_bFalling = true;
	m_bDescenting = false;
	m_bCrouch = false;
	m_bCanPush = false;
	m_bAttachToSide = false;
	m_bCharging = 0;
	m_Dir = { 1,0 };

	m_CurLine = nullptr;

	m_bSloping = false;

	m_bTmpCircle = false;

	m_FrameKey = L"SonicR0";
	m_ZOrder = 2;

	SetIsInWindow(TRUE);
	SetName("Player");
	SetFlying(false);

	m_ColOffset[CO_D].first = Vec2(0, 17);
	m_ColOffset[CO_D].second = Vec2(50, 50);

	m_ColOffset[CO_DR].first = Vec2(20, 20);
	m_ColOffset[CO_DR].second = Vec2(50, 50);

	m_ColOffset[CO_DL].first = Vec2(-20, 20);
	m_ColOffset[CO_DL].second = Vec2(50, 50);

	m_ColOffset[CO_R] .first = Vec2(30, -15);
	m_ColOffset[CO_R].second = Vec2(50, 50);

	m_ColOffset[CO_L] .first = Vec2(-25, -15);
	m_ColOffset[CO_L].second = Vec2(50, 50);

	m_ColOffset[CO_UL].first = Vec2(-30, -30);
	m_ColOffset[CO_UL].second = Vec2(50, 50);

	m_ColOffset[CO_UR].first = Vec2(30, -30);
	m_ColOffset[CO_UR].second = Vec2(50, 50);

	m_ColOffset[CO_U] .first = Vec2(0, -30);
	m_ColOffset[CO_U].second = Vec2(50, 50);

	m_eColliderOffset = CO_D;

	COLLISION->NewCollider(this, "PlayerLineCollider", m_ColOffset[CO_D].first, m_ColOffset[CO_D].second);
	COLLISION->NewCollider(this, "PlayerBodyCollider", Vec2(0, 0), Vec2(60, 70));
	COLLISION->NewCollider(this, "PlayerShootingCollider", Vec2(20, 0), Vec2(40, 40));

	m_bArtJump = false;

	m_bBlink = false;
	m_BlinkTime = 100;

	m_SparkleTime = GetTickCount();
	m_dwTempRectTime = GetTickCount();
	m_dwTempRectTime2 = GetTickCount();

	m_bReleaseSkill = false;
	m_bChargingSkill = false;
	m_bReleaseSkillAfter = false;

	m_SpendRingsSkill = 0;
}

Player::Player(const Vec2& pos, int life, int rings, int score) : Player(pos)
{
	Init();
	m_Life = life;
	m_Rings = rings;
	m_Score = score;
}



Player::~Player()
{
	COLLISION->RemoveCollider(GetComponent("PlayerLineCollider"));
	COLLISION->RemoveCollider(GetComponent("PlayerBodyCollider"));
	COLLISION->RemoveCollider(GetComponent("PlayerShootingCollider"));
}

void Player::Init()
{	
	m_fJumpTime = 0.f;
	m_bJumping = false;
	m_fPower = PLAYER_JUMP_POWER;
}

int Player::Update()
{
	if (m_IsDead) return OBJ_DEAD;

	if (m_bFlyAwayEnding == true)
	{
		m_Pos.x += 10.f;
	}
	
	if (m_bInToPipe)
	{
		m_Pos.y += 0.8f;
	}
	cout << GetRings() << endl;
	//cout << m_bInvincible<< " " << m_bArtJump << endl;

	/*Collider* col = dynamic_cast<Collider*>(GetComponent("PlayerBodyCollider"));
	if (col)
	{
		cout << col->GetArea().left << " " << col->GetArea().top << endl;
	}*/
	if (m_bSuperMode == false)
		Moving();

	if (m_bSuperMode == true && m_bActClear == true)
	{
		m_Pos.x += 1.f;
	}

	if (m_bInvincible == false && m_bActClear == false)
	{
		if (m_bSuperMode == false)
			Key_Input();
		else
			Super_Key_Input();
	}
		
	else if (m_bInvincible == true)
	{
		if (m_BlinkStartTime + m_BlinkTime <= GetTickCount())
		{

			m_BlinkTime -= 5;
			m_BlinkStartTime = GetTickCount();
			if (m_bBlink == true) m_bBlink = false;
			else m_bBlink = true;

			//if (m_bSuperMode == true) m_Pos.x -= 10.f;

			if (m_BlinkTime < 0)
			{
				m_bBlink = false;
				m_bInvincible = false;
				m_bBlink = false;
				m_BlinkTime = 100;
			}
		}
	}

	if (SCROLL_LOCK_STATE)
	{
		if (m_Area.left + (int)SCROLLX < -50.f)
		{
			cout << m_Area.left << endl;
			m_Pos.x = m_Area.left + m_Size.x / 2 + 1.f;
			m_Dir.x *= -1.f;
			m_eDirection = OD_RIGHT;
		}
		if (m_Area.right + (int)SCROLLX > 850.f)
		{
			m_Pos.x = m_Area.right - m_Size.x / 2 - 1.f;
			m_Dir.x *= -1.f;
			m_eDirection = OD_LEFT;
		}
	}

	GameObject::Update_Rect();
	return OBJ_NOEVENT;
}

void Player::Render()
{
	int      ScrollX = (int)SCROLL->GetScrollX();
	int      ScrollY = (int)SCROLL->GetScrollY();
	/*RENDER->SetPen(RGB(0, 0, 0));
	RENDER->SetBrush(RGB(255, 120, 0));
	RENDER->DrawRegularRect(Vec2(x,m_Pos.y), PLAYER_DIAMETER);*/

	if (m_bBlink == false)
	{
		if (m_bSuperMode == false)
		{
			RENDER->Animation_TransparentBlt
			(
				m_FrameKey,
				m_Frame,
				m_Area.left,
				m_Area.top,
				m_Size.x,
				m_Size.y,
				64,
				64,
				RGB(255, 0, 255),
				0,
				0
			);
		}
		else
		{
			Super_Render();
		}
		
		
	}
	else
	{
		RENDER->Animation_TransparentBlt
		(
			m_FrameKey,
			m_Frame,
			m_Area.left,
			m_Area.top,
			0,
			0,
			64,
			64,
			RGB(255, 0, 255),
			0,
			0
		);
	}
	
}

void Player::Late_Update()
{

	Motion_Change();
	ChangeCurState_BoolVar_Speed();
	Offset();

	if (m_bSuperMode == false)
	{
		if (m_fSpeed > 100.f && m_eCurState == PS_RUN)
		{
			m_Frame.dwSpeed = 2;
		}
		if (m_fSpeed > 60.f && m_eCurState == PS_RUN)
		{
			m_Frame.dwSpeed = 40;
		}
		else if (m_fSpeed > 30.f && m_eCurState == PS_RUN)
		{
			m_Frame.dwSpeed = 80;
		}
		
		// 점프 함수를 들어가게되면 falling 이 true가된다. (중력적용)
		Jump();

		Deceleration();

		ColliderOffset();
		SelectFrameKey();
	}
	else
	{
		Super_SelectFrameKey();
		Super_ChangeColor();
		TempRectSetting();
		ReleaseSkillAfterCheck();
	}
	
	Move_Frame();
}

void Player::Offset()
{
	int      iMaxOffSetX = 450;
	int		 iMinOffsetX = 350;
	int		 iMaxOffsetY = 350;
	int		 iMinOffsetY = 250;

	int      iScrollX = (int)SCROLL->GetScrollX();
	int		 iScrollY = (int)SCROLL->GetScrollY();

	//cout << m_fFallingSpeed<<" "<<iScrollY << " " << m_Pos.x + iScrollX << " " << m_Pos.y + iScrollY << endl;

	if (m_bSuperMode == false)
	{
		if (iMaxOffSetX < m_Pos.x + iScrollX)
			SCROLL->AdjustScrollX((iMaxOffSetX - m_Pos.x));

		if (iMinOffsetX > m_Pos.x + iScrollX)
			SCROLL->AdjustScrollX((iMinOffsetX - m_Pos.x));

		if (iMaxOffsetY < m_Pos.y + iScrollY)
			SCROLL->AdjustScrollY((iMaxOffsetY - m_Pos.y));

		if (iMinOffsetY > m_Pos.y + iScrollY)
			SCROLL->AdjustScrollY((iMinOffsetY - m_Pos.y));
	}
	else
	{
		/*if (iMaxOffSetX < m_Pos.x + iScrollX)
			SCROLL->AdjustScrollX((iMaxOffSetX - m_Pos.x));

		if (iMinOffsetX > m_Pos.x + iScrollX)
			SCROLL->AdjustScrollX((iMinOffsetX - m_Pos.x));*/

		/*if (iMaxOffsetY < m_Pos.y + iScrollY)
			SCROLL->AdjustScrollY((iMaxOffsetY - m_Pos.y));

		if (iMinOffsetY > m_Pos.y + iScrollY)
			SCROLL->AdjustScrollY((iMinOffsetY - m_Pos.y));*/
	}
	
}

bool Player::GetDamaged(int dmg)
{
	if (GetInvincible() == false)
	{
		
		m_bInvincible = true;
		m_dwInvincibleTime = GetTickCount();

		
		if (m_bSuperMode == false)
		{
			STOP_PLAY_SOUND(L"DropCoin.wav", CHANNELID::SOUND_SONIC, 1.0f);
			DropRings();

			SetJumping(true);
			SetJumpTime(0.f);
			

			if (GetObjectDir() == OD_RIGHT)
			{
				SetDir({ -1,0 });
				SetSpeed(8.f);
			}
			else
			{
				SetDir({ 1,0 });
				SetSpeed(8.f);
			}
		}
		m_BlinkStartTime = GetTickCount();
		
		return true;
	}
	else
	{
		return false;
	}
	
}

void Player::DropRings()
{
	
	int Rings = GetRings();

	// 16 16
	Scene* scene = SCENE->GetCurrentScene();
	if (scene == nullptr) return;

	Vec2 newPos = { m_Pos.x, m_Pos.y - 200 };

	for (int i = 0; i < Rings; i++)
	{
		
		int randDirX = rand() % 6 - 3;
		int randDirY = rand() % 6 - 3;
		int randSpeed = rand() % 10 - 5;
		Vec2 randDir = { (float)randDirX,0 };
		int randPower = rand() % 10 + 5;
		scene->AddGameObject(OBJ_OBSTACLE, new DropRing(4, L"Ring2", 0, 0, newPos, { 16 * 2,16 * 2 }, { 16,16 }, randPower, randSpeed, randDir, true));
		
	}
	

	SetRings(0);
}

void Player::ShowNowState()
{
	switch(m_eCurState)
	{
	case PS_IDLE:
		cout << "IDLE" << endl;
		break;
	case PS_WALK:
		cout << "WALK" << endl;
		break; 
	case PS_CROUCH:
		cout << "CROUCH" << endl;
		break;
	case PS_RUN:
		cout << "RUN" << endl;
		break;
	case PS_JUMP:
		cout << "JUMP" << endl;
		break;
	case PS_DECEL:
		cout << "DECEL" << endl;
		break;
	case PS_DEAD:
		cout << "DEAD" << endl;
		break;
	case PS_END:
		cout << "END" << endl;
		break;
	}
}

void Player::Jump()
{
	
	// 현재 밟고있는 라인이 없다면 y값을 초기화해주면 안됨.
	/*if (m_CurLine) m_Pos.y = m_CurLine->GetPoint(LINE_LEFT).y;*/
	if (m_bActClear == true) return;
	if (m_bInToPipe == true) return;
	if (m_bJumping)
	{
		m_fFallingSpeed = m_fPower * m_fJumpTime - (6.8f * m_fJumpTime * m_fJumpTime) * 0.5f;
		
		if (m_fFallingSpeed < 0.f)
		{
			m_bFalling = true;
			//m_Dir = { 0,-1 };
		}
			

		if (m_fFallingSpeed <= -15.f) m_fFallingSpeed = -15.f;
		m_Pos.y -= m_fFallingSpeed;
		//m_Pos = m_Pos + m_Dir * m_fFallingSpeed;
		m_fJumpTime += 0.2f;
	}
	else
	{
		m_fFallingSpeed = -10.f;
		m_Pos.y -= m_fFallingSpeed;
		
		m_bFalling = true;
	}
		

}

void Player::Key_Input()
{
	if (!m_bJumping && !m_bFalling)
	{
		if (INPUT->Key_Pressing(VK_SPACE))
		{
			if (INPUT->Key_Pressing(VK_DOWN) && m_fSpeed <= 3.f)
			{
				PLAY_SOUND(L"Ready.wav", CHANNELID::SOUND_SONIC, 1.0f);
				m_bCharging = 1;
				return;
			}
			else
			{
				STOP_SOUND(CHANNELID::SOUND_SONIC);

				if (m_bMarioMode == false)
					PLAY_SOUND(L"Jump.wav", CHANNELID::SOUND_SONIC, 1.f);
				else
					PLAY_SOUND(L"MarioJump.wav", CHANNELID::SOUND_SONIC, 1.f);
				m_bJumping = true;
				m_bFalling = false;
				m_bCharging = 0;
			}
		}
	}

	if (INPUT->Key_Up(VK_DOWN))
	{
		m_bCrouch = false;

		if (m_bCharging == 1)
		{
			STOP_PLAY_SOUND(L"ChargingRelease.wav", CHANNELID::SOUND_SONIC, 1.0f);
			m_bCharging = 2;
			m_fSpeed = PLAYER_MAX_SPEED + 20;
		}	
		else
			m_bCharging = 0;
	}

	if (m_bCharging == 3)
	{
		SetDirectionVector();
		if (m_fSpeed > 0.f)
			m_fSpeed -= 0.2f;
		else if (m_fSpeed <= 0.f)
		{
			m_fSpeed = 0.f;
			m_bCharging = 0;
		}

		return;
	}

	if (INPUT->Key_Pressing(VK_DOWN))
	{
		
		m_bCrouch = true;

		if (m_fSpeed > 0.f)
			m_fSpeed -= 0.05f;
		else if (m_fSpeed < 0.f)
			m_fSpeed = 0.f;
		
	}

	if (INPUT->Key_Up(VK_LEFT))
	{
		m_bCanPush = false;
		m_bAttachToSide = false;

		if (m_fSpeed < 9.f)
		{
			m_fSpeed = 0.f;
		}
		else
		{

		}
	}

	if (INPUT->Key_Up(VK_RIGHT))
	{
		m_bCanPush = false;
		m_bAttachToSide = false;

		if (m_fSpeed < 9.f)
		{
			m_fSpeed = 0.f;
		}
		{

		}

	}

	if (m_bCrouch == false && m_bDecel == false)
	{
		if (INPUT->Key_Pressing(VK_LEFT))
		{
			if (m_eDirection == OD_RIGHT)
			{
				if (m_fSpeed < 0.f)
				{
					m_fSpeed *= -1.f;
				}
				else if (m_fSpeed >= 9.f)
				{
					if (m_Dir.x > 0.f)
					{
						m_bDecel = true;
						m_bBreakSoundPlayed = false;
						return;
					}
				}
			}

			
			m_eDirection = OD_LEFT;
			SetDirectionVector();
			if (!m_bJumping && !m_bFalling)
				m_bCanPush = true;
			

			if (m_fSpeed >= PLAYER_MAX_SPEED)
			{
				m_fSpeed -= 0.1f;
				if (m_fSpeed < PLAYER_MAX_SPEED)
					m_fSpeed = PLAYER_MAX_SPEED;
			}
			else
			{
				m_fSpeed += 0.2f;
				if (m_fSpeed > PLAYER_MAX_SPEED)
					m_fSpeed = PLAYER_MAX_SPEED;

			}
		}
		else if (INPUT->Key_Pressing(VK_RIGHT))
		{
			if (m_eDirection == OD_LEFT)
			{
				if (m_fSpeed < 0.f)
				{
					m_fSpeed *= -1.f;
				}
				else if (m_fSpeed >= 9.f)
				{
					if (m_Dir.x < 0.f)
					{
						m_bDecel = true;
						m_bBreakSoundPlayed = false;
						return;
					}
				}
			}

			m_eDirection = OD_RIGHT;

			SetDirectionVector();
			if (!m_bJumping && !m_bFalling)
				m_bCanPush = true;

			if (m_fSpeed >= PLAYER_MAX_SPEED)
			{
				m_fSpeed -= 0.1f;
				if (m_fSpeed < PLAYER_MAX_SPEED)
					m_fSpeed = PLAYER_MAX_SPEED;
			}
			else
			{
				m_fSpeed += 0.2f;
				if (m_fSpeed > PLAYER_MAX_SPEED)
					m_fSpeed = PLAYER_MAX_SPEED;

			}
		}
	}
	

	
	
}

void Player::Deceleration()
{
	if (m_bCrouch) return;
	if (m_bDecel)
	{
		if (m_fSpeed >= 0.f)
		{
			m_fSpeed -= 0.4f;
		}	
		else
		{
			m_fSpeed = 0.f;
			m_bDecel = false;
		}
			
	}
	else
	{
		// 속도가 15가 넘어갔을때 감속.
		if (!m_bSloping)
		{
			if (m_fSpeed > 0.f)
				m_fSpeed -= 0.1f;
			else
				m_fSpeed = 0.f;
		}
	}
	
}

void Player::Super_Key_Input()
{
	if (INPUT->Key_Pressing(VK_LEFT))
	{
		m_Pos.x = m_Pos.x - 8.f;
		
	}
	if (INPUT->Key_Pressing(VK_RIGHT))
	{
		m_Pos.x = m_Pos.x + 8.f;
	}
	if (INPUT->Key_Pressing(VK_DOWN))
	{
		m_Pos.y = m_Pos.y + 8.f;
	}
	if (INPUT->Key_Pressing(VK_UP))
	{
		m_Pos.y = m_Pos.y - 8.f;
	}

	if (INPUT->Key_Up(VK_SPACE))
	{
		if (m_SpendRingsSkill > 0)
			STOP_PLAY_SOUND(L"ReleaseSkill.wav", CHANNELID::SOUND_SONIC, 0.6f);
		Scene* curScene = SCENE->GetCurrentScene();
		if (curScene)
		{
			curScene->AddGameObject(OBJ_SKILL, new SkillRing(m_SpendRingsSkill, m_Pos));
		}
		m_bChargingSkill = false;
		m_bReleaseSkill = true;
		m_SpendRingsSkill = 0;
	}
	if (INPUT->Key_Down(VK_SPACE))
	{
		
	}
	if (INPUT->Key_Pressing(VK_SPACE))
	{
		if (GetRings() > 0)
		{
			m_SpendRingsSkill += 1;
			SubRings(1);
		}

		if (IS_PLAYING(CHANNELID::SOUND_SONIC) == FALSE)
			STOP_PLAY_SOUND(L"SkillReady.wav", CHANNELID::SOUND_SONIC, 0.6f);
			
		m_bChargingSkill = true;
		m_bReleaseSkill = false;
	}
}

void Player::Super_SelectFrameKey()
{
	if (m_bConvertingSuperMode == true)
	{
		m_FrameKey = L"SuperSonicConvert";
		return;
	}

	if (m_bChargingSkill == false && m_bReleaseSkill == false)
	{
		switch (m_SuperColor)
		{
		case 0:
			m_FrameKey = L"SuperSonicFlyRight_1";
			break;
		case 1:
			m_FrameKey = L"SuperSonicFlyRight_2";
			break;
		case 2:
			m_FrameKey = L"SuperSonicFlyRight_3";
			break;
		case 3:
			m_FrameKey = L"SuperSonicFlyRight_4";
			break;
		case 4:
			m_FrameKey = L"SuperSonicFlyRight_5";
			break;
		case 5:
			m_FrameKey = L"SuperSonicFlyRight_6";
			break;
		}
	}
	else
	{
		switch (m_SuperColor)
		{
		case 0:
			m_FrameKey = L"Skill1";
			break;
		case 1:
			m_FrameKey = L"Skill2";
			break;
		case 2:
			m_FrameKey = L"Skill3";
			break;
		case 3:
			m_FrameKey = L"Skill4";
			break;
		case 4:
			m_FrameKey = L"Skill5";
			break;
		case 5:
			m_FrameKey = L"Skill6";
			break;
		}
	}
	
	
}

void Player::Motion_Change()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case PS_CONVERTINGSUPER:
			m_Frame.iFrameStart = 0;
			m_Frame.iFrameEnd = 13;
			m_Frame.iMotion = 0;
			m_Frame.dwSpeed = 30;
			m_Frame.repeat = false;
			m_Frame.dwTime = GetTickCount();
			break;
		case PS_SUPER:
			m_Frame.iFrameStart = 0;
			m_Frame.iFrameEnd = 1;
			m_Frame.iMotion = 0;
			m_Frame.dwSpeed = 200;
			m_Frame.repeat = true;
			m_Frame.dwTime = GetTickCount();
			break;
		case PS_IDLE:
			m_Frame.iFrameStart = 0;
			m_Frame.iFrameEnd = 0;
			m_Frame.iMotion = 0;
			m_Frame.dwSpeed = 250;
			m_Frame.repeat = false;
			m_Frame.dwTime = GetTickCount();
			break;

		case PS_WALK:
			m_Frame.iFrameStart = 0;
			m_Frame.iFrameEnd = 7;
			m_Frame.iMotion = 1;
			m_Frame.dwSpeed = 150;
			m_Frame.repeat = true;
			m_Frame.dwTime = GetTickCount();
			break;
		case PS_SKILL:
			m_Frame.iFrameStart = 1;
			m_Frame.iFrameEnd = 3;
			m_Frame.iMotion = 0;
			m_Frame.dwSpeed = 80;
			m_Frame.repeat = false;
			m_Frame.dwTime = GetTickCount();
			break;

		case PS_RUN:
			m_Frame.iFrameStart = 0;
			m_Frame.iFrameEnd = 3;
			m_Frame.iMotion = 2;
			m_Frame.dwSpeed = 120;
			m_Frame.repeat = true;
			m_Frame.dwTime = GetTickCount();
			break;
		case PS_DECEL:
			m_Frame.iFrameStart = 0;
			m_Frame.iFrameEnd = 3;
			m_Frame.iMotion = 9;
			m_Frame.dwSpeed = 120;
			m_Frame.repeat = false;
			m_Frame.dwTime = GetTickCount();
			break;
		case PS_CHARGING:
			m_Frame.iFrameStart = 0;
			m_Frame.iFrameEnd = 5;
			m_Frame.iMotion = 4;
			m_Frame.dwSpeed = 120;
			m_Frame.repeat = true;
			m_Frame.dwTime = GetTickCount();
			break;
		case PS_CROUCH:
			m_Frame.iFrameStart = 0;
			m_Frame.iFrameEnd = 1;
			m_Frame.iMotion = 7;
			m_Frame.dwSpeed = 180;
			m_Frame.repeat = false;
			m_Frame.dwTime = GetTickCount();
			break;
		case PS_WALLPUSH:
			m_Frame.iFrameStart = 0;
			m_Frame.iFrameEnd = 2;
			m_Frame.iMotion = 8;
			m_Frame.dwSpeed = 180;
			m_Frame.repeat = true;
			m_Frame.dwTime = GetTickCount();
			break;
		case PS_JUMP:
			m_Frame.iFrameStart = 0;
			m_Frame.iFrameEnd = 7;
			m_Frame.iMotion = 3;
			m_Frame.dwSpeed = 50;
			m_Frame.repeat = true;
			m_Frame.dwTime = GetTickCount();
			break;
		case PS_DAMAGED:
			m_Frame.iFrameStart = 0;
			m_Frame.iFrameEnd = 1;
			m_Frame.iMotion = 10;
			m_Frame.dwSpeed = 200;
			m_Frame.repeat = true;
			m_Frame.dwTime = GetTickCount();
			break;
		case PS_DEAD:
			m_Frame.iFrameStart = 0;
			m_Frame.iFrameEnd = 0;
			m_Frame.iMotion = 0;
			m_Frame.dwSpeed = 200;
			m_Frame.dwTime = GetTickCount();
			break;
		case PS_ARTJUMP:
			m_Frame.iFrameStart = 0;
			m_Frame.iFrameEnd = 0;
			m_Frame.iMotion = 0;
			m_Frame.dwSpeed = 200;
			m_Frame.dwTime = GetTickCount();
			break;
		case PS_ACTCLEAR:
			m_Frame.iFrameStart = 0;
			m_Frame.iFrameEnd = 2;
			m_Frame.iMotion = 1;
			m_Frame.dwSpeed = 200;
			m_Frame.repeat = true;
			m_Frame.dwTime = GetTickCount();
			break;
		default:
			m_Frame.iFrameStart = 0;
			m_Frame.iFrameEnd = 0;
			m_Frame.iMotion = 0;
			m_Frame.dwSpeed = 200;
			m_Frame.dwTime = GetTickCount();
			break;
		}

		m_ePreState = m_eCurState;
	}
}

void Player::ColliderOffset()
{
	Collider* collider = dynamic_cast<Collider*>(GetComponent("PlayerLineCollider"));
	if (collider)
	{
		collider->SetOffset(m_ColOffset[m_eColliderOffset]);
	}
}

void Player::SetDirectionVector()
{
	if (m_CurLine)
	{
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
		}
		if (m_eDirection == OD_LEFT)
		{
			if (m_CurLine->GetLineIncline() >= 8.f || m_CurLine->GetLineIncline() <= -8.f)
			{
				if (layer == LL_FORWARD)
					m_Dir = bottom - top;
				else if (layer == LL_REVERSE)
					m_Dir = top - bottom;
			}
			else
			{
				if (layer == LL_FORWARD)
					m_Dir = left - right;
				else if (layer == LL_REVERSE)
					m_Dir = right - left;
			}
		}
		m_Dir.Normalize();
		
	}
	else
	{
		// 방향에 해당 선의 벡터 성분이 남아있을때는 아래와 같이 실행하지 않음.
		Vec2 left = { -1,0 };
		Vec2 right = { 1,0 };
		m_bArtJump = false;
		m_fPower = PLAYER_JUMP_POWER;
		//if (m_bFalling)
		//{
		//	// 현재 내 방향 성분을 가져가면서 
		//	if (m_eDirection == OD_RIGHT)
		//		m_Dir.x += 0.1f;
		//	else if (m_eDirection == OD_LEFT)
		//		m_Dir.x -= 0.1f;

		//	m_fSpeed -= 0.2f;
		//	if (m_fSpeed < 0.f) m_fSpeed = 0.f;
		//	cout << m_fSpeed << " " << m_Dir.x << "," << m_Dir.y << endl;
		//	m_Dir.Normalize();

		//	return;
		//}
		
		if (m_bFalling && m_Dir != left && m_Dir != right)
		{
			m_fSpeed -= 0.3f;
			if (m_fSpeed < 0.f) m_fSpeed = 0.f;
			return;
		}
		if (m_bCharging == 3) return;
		if (m_eDirection == OD_RIGHT)
			m_Dir = { 1, 0 };
		else if (m_eDirection == OD_LEFT)
			m_Dir = { -1,0 };
	}
}

void Player::Super_ChangeColor()
{
	if (m_bConvertingSuperMode == true) return;

	if (m_dwChangeColorTime + 30 <= GetTickCount())
	{
		m_SuperColor++;
		if (m_SuperColor == 6) m_SuperColor = 0;
		m_dwChangeColorTime = GetTickCount();
	}
}

void Player::Super_Render()
{
	if (m_bConvertingSuperMode)
	{
		int xSize = 28;
		if (m_Frame.iFrameStart == 0) xSize = 23;
		
		static int xStart = 0;
		static int iFrameStart = 0;

		if (m_Frame.iFrameStart != iFrameStart)
		{
			iFrameStart = m_Frame.iFrameStart;
			
			if (iFrameStart == 1)
				xStart += 23 + 5;
			else
				xStart += xSize + 5;
		}
		RENDER->TransparentImage
		(
			m_FrameKey,
			m_Area.left + 40,
			m_Area.top + 20,
			m_Size.x - 85,
			m_Size.y - 45,
			xStart, 0,
			xSize, 51,
			RGB(255, 0, 255)
		);

		if (m_Frame.iFrameStart == 13)
		{
			m_bConvertingSuperMode = false;
			PLAY_SOUND(L"Transformation.wav", CHANNELID::SOUND_SONIC, 1.0f);
			return;
		}
	}
	else
	{
		if (m_bChargingSkill == false && m_bReleaseSkill == false)
		{
			RENDER->Animation_TransparentBlt
			(
				m_FrameKey,
				m_Frame,
				m_tmpRect2.left + 40,
				m_tmpRect2.top + 40,
				m_Size.x - 70,
				m_Size.y - 70,
				37,
				36,
				RGB(255, 0, 255),
				5,
				0
			);

			RENDER->Animation_TransparentBlt
			(
				m_FrameKey,
				m_Frame,
				m_tmpRect.left + 40,
				m_tmpRect.top + 40,
				m_Size.x - 70,
				m_Size.y - 70,
				37,
				36,
				RGB(255, 0, 255),
				5,
				0
			);

			RenderSparkle();

			RENDER->Animation_TransparentBlt
			(
				m_FrameKey,
				m_Frame,
				m_Area.left + 40,
				m_Area.top + 40,
				m_Size.x - 70,
				m_Size.y - 70,
				37,
				36,
				RGB(255, 0, 255),
				5,
				0
			);
		}
		else if (m_bReleaseSkill == true)
		{
			// 30 37 32
			int RendCX = 0;
			int RendX = 0;
			if (m_Frame.iFrameStart == 1)
			{
				RendX = 36;
				RendCX = 30;
			}
			else if (m_Frame.iFrameStart == 2)
			{
				RendX = 74;
				RendCX = 37;
			}
			else if (m_Frame.iFrameStart == 3)
			{
				RendX = 119;
				RendCX = 32;
			}
			RENDER->TransparentImage
			(
				m_FrameKey,
				m_tmpRect2.left + 40,
				m_tmpRect2.top + 30,
				m_Size.x - 80,
				m_Size.y - 35,
				RendX, 0,
				RendCX, 61,
				RGB(255, 0, 255)
			);

			RENDER->TransparentImage
			(
				m_FrameKey,
				m_tmpRect.left + 40,
				m_tmpRect.top + 30,
				m_Size.x - 80,
				m_Size.y - 35,
				RendX, 0,
				RendCX, 61,
				RGB(255, 0, 255)
			);
			RenderSparkle();
			
			RENDER->TransparentImage
			(
				m_FrameKey,
				m_Area.left + 40,
				m_Area.top + 30,
				m_Size.x - 80,
				m_Size.y - 35,
				RendX, 0,
				RendCX, 61,
				RGB(255, 0, 255)
			);

			if (m_Frame.iFrameStart == 3 && m_bReleaseSkillAfter == false)
			{
				m_bReleaseSkillAfter = true;
				m_dwReleaseSkillAfterTime = GetTickCount();
			}
		}
		else if (m_bChargingSkill == true)
		{
			RENDER->TransparentImage
			(
				m_FrameKey,
				m_tmpRect2.left + 40,
				m_tmpRect2.top + 30,
				m_Size.x - 85,
				m_Size.y - 35,
				0, 0,
				28, 61,
				RGB(255, 0, 255)
			);
			RENDER->TransparentImage
			(
				m_FrameKey,
				m_tmpRect.left + 40,
				m_tmpRect.top + 30,
				m_Size.x - 85,
				m_Size.y - 35,
				0, 0,
				28, 61,
				RGB(255, 0, 255)
			);
			RenderSparkle();
			RENDER->TransparentImage
			(
				m_FrameKey,
				m_Area.left + 40,
				m_Area.top + 30,
				m_Size.x - 85,
				m_Size.y - 35,
				0, 0,
				28, 61,
				RGB(255, 0, 255)
			);

			
		}

		

		
	}
	
	
		
}

void Player::RenderSparkle()
{
	if (m_SparkleTime + 50 <= GetTickCount())
	{
		m_RenderSparklePosition[0].x = rand() % 100 + 10;
		m_RenderSparklePosition[0].y = rand() % 100 + 10;
		m_RenderSparklePosition[1].x = rand() % 100 + 10;
		m_RenderSparklePosition[1].y = rand() % 100 + 10;
		m_RenderSparklePosition[2].x = rand() % 100 + 10;
		m_RenderSparklePosition[2].y = rand() % 100 + 10;
		m_SparkleTime = GetTickCount();
	}
	RENDER->TransparentImage
	(
		L"Sparkle",
		m_Area.left + m_RenderSparklePosition[0].x,
		m_Area.top + m_RenderSparklePosition[0].y,
		16 * 2,
		16 * 2,
		0, 8,
		16, 16,
		RGB(255, 0, 255)
	);

	RENDER->TransparentImage
	(
		L"Sparkle",
		m_Area.left + m_RenderSparklePosition[1].x,
		m_Area.top + m_RenderSparklePosition[1].y,
		16 * 2,
		16 * 2,
		24, 8,
		16, 16,
		RGB(255, 0, 255)
	);

	RENDER->TransparentImage
	(
		L"Sparkle",
		m_Area.left + m_RenderSparklePosition[2].x,
		m_Area.top + m_RenderSparklePosition[2].y,
		32 * 2,
		32 * 2,
		48, 0,
		32, 32,
		RGB(255, 0, 255)
	);
}

void Player::TempRectSetting()
{
	if (m_dwTempRectTime + 100 <= GetTickCount())
	{
		m_tmpRect = m_Area;
		m_dwTempRectTime = GetTickCount();
	}

	if (m_dwTempRectTime2 + 200 <= GetTickCount())
	{
		m_tmpRect2 = m_Area;
		m_dwTempRectTime2 = GetTickCount();
	}
}

void Player::ReleaseSkillAfterCheck()
{
	if (m_bReleaseSkillAfter == true)
	{
		if (m_dwReleaseSkillAfterTime + 500 <= GetTickCount())
		{
    		m_bReleaseSkillAfter = false;
			m_bReleaseSkill = false;
		}
	}
}

void Player::SelectFrameKey()
{
	if (m_bActClear)
	{
		m_FrameKey = L"SonicL";
		return;
	}
	if (m_bArtJump)
	{
		if (m_eDirection == OD_LEFT)
		{
			m_FrameKey = L"SonicL";
		}
		else if (m_eDirection == OD_RIGHT)
		{
			m_FrameKey = L"SonicR";
		}
		return;
	}

	if (!m_CurLine)
	{
		m_eColliderOffset = CO_D;

		if (m_eDirection == OD_LEFT)
		{
			m_FrameKey = L"SonicL0";
		}
		else if (m_eDirection == OD_RIGHT)
		{
			m_FrameKey = L"SonicR0";
		}
	}
	else
	{
		LINELAYER layer = m_CurLine->GetLayer();
		float curAngle = m_CurLine->GetAngle();
		float absCurAngle = fabsf(curAngle);
		
		if (curAngle >= 30.f && curAngle <= 80.f)
		{
			if (layer == LL_FORWARD)
			{
				m_eColliderOffset = CO_DL;

				if (m_eDirection == OD_LEFT)
				{
					m_FrameKey = L"SonicL315";
				}
				else if (m_eDirection == OD_RIGHT)
				{
					m_FrameKey = L"SonicR315";
				}
			}
			else if (layer == LL_REVERSE)
			{
				m_eColliderOffset = CO_UR;
				if (m_eDirection == OD_LEFT)
				{
					m_FrameKey = L"SonicL135";
				}
				else if (m_eDirection == OD_RIGHT)
				{
					m_FrameKey = L"SonicR135";
				}
			}
			
		}
		else if (curAngle >= -80.f && curAngle <= -30.f)
		{
			if (layer == LL_FORWARD)
			{
				m_eColliderOffset = CO_DR;

				if (m_eDirection == OD_LEFT)
				{
					m_FrameKey = L"SonicL45";
				}
				else if (m_eDirection == OD_RIGHT)
				{
					m_FrameKey = L"SonicR45";
				}
			}
			else if (layer == LL_REVERSE)
			{
				m_eColliderOffset = CO_UL;
				if (m_eDirection == OD_LEFT)
				{
					m_FrameKey = L"SonicL225";
				}
				else if (m_eDirection == OD_RIGHT)
				{
					m_FrameKey = L"SonicR225";
				}
			}
			
		}
		else if (absCurAngle >= 80.f && absCurAngle <= 90.f)
		{
			if (layer == LL_FORWARD)
			{
				m_eColliderOffset = CO_R;
				//cout << "적용됐다!!" << endl;
				if (m_eDirection == OD_LEFT)
				{
					m_FrameKey = L"SonicL90";
				}
				else if (m_eDirection == OD_RIGHT)
				{
					m_FrameKey = L"SonicR90";
				}
			}
			else if (layer == LL_REVERSE)
			{
				m_eColliderOffset = CO_L;
				if (m_eDirection == OD_LEFT)
				{
					m_FrameKey = L"SonicL270";
				}
				else if (m_eDirection == OD_RIGHT)
				{
					m_FrameKey = L"SonicR270";
				}
			}
			
		}
		else // 가로선
		{
			if (layer == LL_FORWARD)
			{
				m_eColliderOffset = CO_D;

				if (m_eDirection == OD_LEFT)
				{
					m_FrameKey = L"SonicL0";
				}
				else if (m_eDirection == OD_RIGHT)
				{
					m_FrameKey = L"SonicR0";
				}
			}
				
			else if (layer == LL_REVERSE)
			{
				m_eColliderOffset = CO_U;

				if (m_eDirection == OD_LEFT)
				{
					m_FrameKey = L"SonicL180";
				}
				else if (m_eDirection == OD_RIGHT)
				{
					m_FrameKey = L"SonicR180";
				}
			}
				
			
			
		}
	}
	
}

void Player::ChangeCurState_BoolVar_Speed()
{
	if (m_bConvertingSuperMode == true)
	{
		m_eCurState = PS_CONVERTINGSUPER;
		return;
	}
	if (m_bReleaseSkill == true)
	{
		m_eCurState = PS_SKILL;
		return;
	}
	if (m_bSuperMode == true)
	{
		m_eCurState = PS_SUPER;
		return;
	}

	if (m_bActClear)
	{
		m_eCurState = PS_ACTCLEAR;
		return;
	}
	if (m_bInvincible)
	{
		m_eCurState = PS_DAMAGED;
		return;
	}
	if (m_bArtJump)
	{
		m_eCurState = PS_ARTJUMP;
		return;
	}

	if (m_bJumping || m_bFalling)
	{
		m_eCurState = PS_JUMP;
	}
	else
	{
		if (m_bCanPush && m_bAttachToSide)
		{
			m_eCurState = PS_WALLPUSH;
			m_fSpeed = 0.f;
			return;
		}
		if (m_bCharging >= 2)
		{
			m_eCurState = PS_JUMP;
			m_bCharging = 3;
			return;
		}
		if (m_bCharging == 1)
		{
			m_eCurState = PS_CHARGING;
			return;
		}
		if (m_bCrouch)
		{
			if (m_fSpeed > 0.f)
				m_eCurState = PS_JUMP;
			else if (m_fSpeed == 0.f)
				m_eCurState = PS_CROUCH;
			return;
		}

		if (m_bDecel)
		{
			m_eCurState = PS_DECEL;

			if (m_bBreakSoundPlayed == false)
			{
				STOP_SOUND(CHANNELID::SOUND_SONIC);
				PLAY_SOUND(L"Break.wav", CHANNELID::SOUND_SONIC, 1.f);
				m_bBreakSoundPlayed = true;
			}
			
			return;
		}

		

		if (m_fSpeed >= PLAYER_MAX_SPEED && m_bCanPush)
			m_eCurState = PS_RUN;
		else if (m_fSpeed > 0 && m_bCanPush)
			m_eCurState = PS_WALK;
		else if (m_fSpeed == 0)
			m_eCurState = PS_IDLE;
	}
}

void Player::Moving()
{
	// 타일일 경우.
	m_bSloping = false;
	// 라인일 경우.
	if (m_CurLine)
	{
		LINELAYER layer = m_CurLine->GetLayer();
		
		float angle = m_CurLine->GetAngle();
		float absAngle = fabsf(angle);
		float slopeAmount = sinf(absAngle * PI / 180.f) * 0.2f;

		if (absAngle < 80.f)
		{
			if (layer == LL_FORWARD)
			{
				if (m_eDirection == OD_LEFT) // m_eDirection == OD_LEFT 였다 원래.
				{
					if (angle > 0)
					{
						if (absAngle > 15.f)
						{
							m_fSpeed -= slopeAmount;
							m_bSloping = true;
						}
						else
							m_bSloping = false;
					}
					else
					{
						// 가속
						if (absAngle > 15.f)
						{
							m_fSpeed += slopeAmount;
							m_bSloping = true;
						}
						else
							m_bSloping = false;

					}
				}
				else if (m_eDirection == OD_RIGHT) // m_eDirection == PD_RIGHT 였다 원래.
				{
					if (angle > 0)
					{
						// 가속
						if (absAngle > 15.f)
						{
							m_fSpeed += slopeAmount;
							m_bSloping = true;
						}
						else
							m_bSloping = false;
					}
					else
					{
						// 감속
						if (absAngle > 15.f)
						{
							m_fSpeed -= slopeAmount;
							m_bSloping = true;
						}
						else
							m_bSloping = false;
					}
				}
			}
		}
		
		


		
		
	}
	else
	{

	}
	// 타일이랑 충돌하고 있을 경우
	
	m_Pos = m_Pos + (m_Dir * m_fSpeed);
	
}

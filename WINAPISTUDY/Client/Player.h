#pragma once
#include "Creature.h"

#define PLAYER_MAX_SPEED 30.f
#define PLAYER_JUMP_POWER 15.f

enum PLAYERSTATE
{
	PS_IDLE,
	PS_WALK,
	PS_CROUCH,
	PS_RUN,
	PS_JUMP,
	PS_DECEL,
	PS_CHARGING,
	PS_DEAD,
	PS_DAMAGED,
	PS_WALLPUSH,
	PS_ARTJUMP,
	PS_ACTCLEAR,
	PS_SUPER,
	PS_CONVERTINGSUPER,
	PS_SKILL,
	PS_END
};





class Player : public Creature
{
private:
	

public:
	Player(const Vec2& _pos);
	Player(const Vec2& pos, int life, int rings, int score);

	virtual ~Player();

	void	Init()						override;
	int		Update()					override;
	void	Render()					override;
	void	Late_Update()				override;
	void	Offset();

	virtual bool GetDamaged(int dmg)	override;
	virtual void DropRings()			override;

private:
	void	ShowNowState();
	void	Jump();
	void	Key_Input();
	void	Deceleration();

	void	Super_Key_Input();
	void	Super_SelectFrameKey();

	void	Motion_Change();
	void	ColliderOffset();
	void	SetDirectionVector();

	void	Super_ChangeColor();
	void	Super_Render();

	void	RenderSparkle();
	void	TempRectSetting();
	void	ReleaseSkillAfterCheck();
	
	//vector<wstring> m_FrameKeyList[2];

	// GameObject을(를) 통해 상속됨
public:
	virtual void SelectFrameKey() override;
	void ChangeCurState_BoolVar_Speed();
	void Moving();

private:
	RECT m_tmpRect;
	RECT m_tmpRect2;
	PLAYERSTATE m_eCurState;
	PLAYERSTATE m_ePreState;

	DWORD		m_dwChangeColorTime;
	DWORD		m_dwTempRectTime;
	DWORD		m_dwTempRectTime2;

	int			m_SuperColor;
	bool		m_bConvertingSuperMode;
	bool		m_bSuperMode;
	bool		m_bChargingSkill;
	bool		m_bReleaseSkill;
	bool		m_bReleaseSkillAfter;

	DWORD		m_dwReleaseSkillAfterTime;
	int			m_SpendRingsSkill;

	

	float		m_fPower;
	bool		m_bArtJump;

	COLLIDER_OFFSET m_eColliderOffset;


	bool		m_bDecel;
	bool		m_bCrouch;
	bool		m_bCanPush;
	bool		m_bAttachToSide;
	bool		m_bSloping;
	

	bool		m_bBreakSoundPlayed;

	// 0: 아예 안누른상태, 1: 누르고있는상태, 2: 눌렀다가 뗀 상태, 3: 떼고나서 진행하는 상태
	int			m_bCharging;

	// 임시변수. 나중에 지울수도 있음.
	bool		m_bTmpCircle;

	

	LONG m_BlinkTime;
	DWORD m_BlinkStartTime;
	bool m_bBlink;
	bool m_bActClear;

	LONG m_SparkleTime;
	Vec2 m_RenderSparklePosition[3];

	pair<Vec2,Vec2>		m_ColOffset[CO_END];

	int m_Life;
	int m_Score;

	bool m_bOnThePipe;
	bool m_bInToPipe;
	bool m_bOutToPipe;
	bool m_bMarioMode;

	bool m_bFlyAwayEnding;

public:
	//bool GetActClear() const { return m_bActClear; }
	void SetFlyAwayEnding(bool state) { m_bFlyAwayEnding = state; }
	bool GetMarioMode() const { return m_bMarioMode; }
	void SetMarioMode(bool state) { m_bMarioMode = state; }
	bool GetOutToPipe() const { return m_bOutToPipe; }
	void SetOutToPipe(bool state) { m_bOutToPipe = state; }
	bool GetInToPipe() const { return m_bInToPipe; }
	void SetInToPipe(bool state) { m_bInToPipe = state; }
	bool GetOnThePipe() const { return m_bOnThePipe; }
	void SetOnThePipe(bool state) { m_bOnThePipe = state; }
	void SetSuperMode() 
	{ 
		m_bSuperMode = true; 
		m_bConvertingSuperMode = true; 
	}
	void SetActClear(bool state)		{ m_bActClear = state; }
	bool GetActClear() const			{ return m_bActClear; }
	bool GetCanPush() const				{ return m_bCanPush; }
	void SetCanPush(bool state)			{ m_bCanPush = state; }
	bool GetAttachToSide() const		{ return m_bAttachToSide; }
	void SetAttachToSide(bool state)	{ m_bAttachToSide = state; }
	int GetCharging() const				{ return m_bCharging; }
	PLAYERSTATE GetCurState() const		{ return m_eCurState; }
	void SetCurState(PLAYERSTATE state) { m_eCurState = state; }
	int GetLife() const					{ return m_Life; }
	void SubLife()						{ --m_Life; }
	void AddLife(int life = 1)			{ m_Life += life; }
	int GetScore() const				{ return m_Score; }
	
	void AddScore(int amount)			{ m_Score += amount; }
	void SubScore(int amount)
	{
		m_Score -= amount;
		if (m_Score < 0) m_Score = 0;
	}
	void SetJumpPower(float power) { m_fPower = power; }
	float GetJumpPower() const { return m_fPower; }
	void SetArtJump(bool state) { m_bArtJump = state; }
	bool GetArtJump() const { return m_bArtJump; }


	/*void AddPrevLine(Line* line) { m_PrevLineList.push_back(line); }
	list<Line*>* GetPrevLineList()  { return &m_PrevLineList; }
	bool FindLine(Line* line) 
	{ 
		auto it = find(m_PrevLineList.begin(), m_PrevLineList.end(), line);
		return it != m_PrevLineList.end();
	}
	void ClearPrevLineList() { m_PrevLineList.clear(); }*/

	COLLIDER_OFFSET GetColOffset() const { return m_eColliderOffset; }

};


#pragma once
#include "Creature.h"

class Scene;

class DoomsDay : public Creature
{
public:
	DoomsDay(const Vec2& pos, Scene* curScene, Creature* target);
	virtual ~DoomsDay();

	// GameObject을(를) 통해 상속됨
	virtual void Render() override;
	virtual void Init() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void SelectFrameKey() override;

	void Eject_Missile();
	void Eject_Rocket();
private:
	void Move_DoomsDay_Frame();
	void Missile_Barrel_Select();
	void Setting_Missile_Info();

	void Move_BoomEffect_Frame();
	void Move_DeathEffect_Frame();
	void RandomPos_Effect();
	void DoomsDayOutScreen();
	

private:
	Creature*	m_pTarget;
	Scene*		m_pCurScene;
	Vec2		m_MissileStandard;

	Vec2		m_EjectRocketPos;
	Vec2		m_EjectMissilePosArr[8];
	Vec2		m_MissileBarrelPosArr[3];
	Vec2		m_MissileDir;

	DWORD		m_MissileTime1;
	DWORD		m_MissileTime2;
	DWORD		m_MissileTime3;

	DWORD		m_RocketTime;

	DWORD		m_BlinkStartTime;
	LONG		m_BlinkTime;
	bool		m_bBlink;

	GameObject* m_CurRocket;
	FRAME m_BoomEffectFrame;
	FRAME m_DeathEffectFrame;

	pair<Vec2,bool> m_DeathRandomPos[12];
	DWORD m_DeathRandomTime;

	int m_AngleIndex;
	int m_CoordX;
	int m_CoordY;
	bool m_bGenTime;
	bool m_bGetDamaged;
	bool m_bDeath;

public:
	void SetTarget(Creature* target) { m_pTarget = target; }
	void SetCurScene(Scene* scene) { m_pCurScene = scene; }

	// Creature을(를) 통해 상속됨
	virtual bool GetDamaged(int dmg) override;
	virtual void DropRings() override;
};


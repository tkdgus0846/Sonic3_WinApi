#pragma once
#include "GameObject.h"

class Creature : public GameObject
{
public:
	virtual bool GetDamaged(int dmg) PURE;
	virtual void DropRings() PURE;
	
protected:
	int m_HP;
	bool m_bInvincible;
	DWORD m_dwInvincibleTime;
	int m_Rings;
	

public:
	int GetHP() const { return m_HP; }
	void SetHP(int hp) { m_HP = hp; }
	void SubHP(int hp) { m_HP -= hp; if (m_HP < 0) m_HP = 0; }
	bool GetInvincible() const { return m_bInvincible; }
	void SetInvincible(bool state) { m_bInvincible = state; }
	int GetRings() const { return m_Rings; }
	void SetRings(int newRings) { m_Rings = newRings; }
	void AddRings(int addring) { m_Rings += addring; }
	void SubRings(int subring) { m_Rings -= subring; if (m_Rings < 0) m_Rings = 0; }
	
};


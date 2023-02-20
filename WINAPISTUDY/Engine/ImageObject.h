#pragma once
#include "GameObject.h"

class Collider;

enum OBSTACLETYPE
{
	OT_SPIKE,
	OT_ROLLER,
	OT_SPRING,
	OT_SIGN,
	OT_RING,
	OT_RING2,
	OT_BG_MUSHROOM,
	OT_DROPMUSHROOM,
	OT_MUSHROOM,
	OT_ITEM,
	OT_SPIKEBALL,
	OT_END
};

class ImageObject : public GameObject
{
public:
	ImageObject() {}
	virtual ~ImageObject() {  }
	ImageObject(int ZOrder, const TCHAR* frameKey, int coordX, int coordY, const Vec2& Pos, const Vec2& Size, const Vec2& RenderSize, bool collisionOn = false, int splitMode = 0);

	// 씬의 로드 부분에서 m_RenderQueue 에 집어넣어주면 되지 않을까?
	virtual void Render() override;
	virtual void Init() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void SelectFrameKey() override;

	virtual void Collision_Up(Collider* collider, const int& px, const int& py);
	virtual void Collision_Down(Collider* collider, const int& px, const int& py);
	virtual void Collision_Left(Collider* collider, const int& px, const int& py);
	virtual void Collision_Right(Collider* collider, const int& px, const int& py);
	virtual void Hit_With_SkillRing() {}
	void Collision_Enter() 
	{ 
		if (m_bCollisionWithCollider == false)
			m_bCollisionWithCollider = true;
	}
	void Collision_Exit() 
	{
		if (m_bCollisionWithCollider == true)
			m_bCollisionWithCollider = false;
	}

protected:
	int m_CoordX;
	int m_CoordY;
	TCHAR m_FrameKeyArr[32];
	Vec2 m_RenderSize;
	bool m_bCollisionOn;
	int m_SplitMode;
	OBSTACLETYPE m_eObstacleType;

	bool m_bCollisionWithCollider;

public:
	int GetCoordX() const { return m_CoordX; }
	void SetCoordX(int coordX) { m_CoordX = coordX; }
	int GetCoordY() const { return m_CoordY; }
	void SetCoordY(int coordY) { m_CoordY = coordY; }
	TCHAR* GetFrameKeyArr() { return m_FrameKeyArr; }
	bool GetCollisionOn() const { return m_bCollisionOn; }
	void SetCollisionOn(bool collisionOn) { m_bCollisionOn = collisionOn; }
	bool GetCollisionWithCollider() const { return m_bCollisionWithCollider; }
	void SetCollisionWithCollider(bool col) { m_bCollisionWithCollider = col; }
	
	int GetSplitMode() const { return m_SplitMode; }

	OBSTACLETYPE GetObstacleType() const { return m_eObstacleType; }

	
};


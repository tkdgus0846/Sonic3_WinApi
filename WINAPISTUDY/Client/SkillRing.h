#pragma once
#include "GameObject.h"

struct SkillRingInfo
{
	Vec2 pos;
	Vec2 dir;
	float speed;
};

class SkillRing : public GameObject
{
public:
	SkillRing(int ringNum, const Vec2& pos);
	virtual ~SkillRing();

public:
	// GameObject을(를) 통해 상속됨
	virtual void Render() override;
	virtual void Init() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void SelectFrameKey() override;

private:
	void SelectRingsPos();
	void RingsOutWindow();
	void CollisionWithObstacles();

private:
	vector<SkillRingInfo> m_RingsInfo;
};


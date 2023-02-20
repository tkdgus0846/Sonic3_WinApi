#pragma once

#include "Scene.h"

class Scene01 : public Scene
{
public:
	virtual void Init();
	virtual void Update();
	virtual void Late_Update();
	virtual void Render();
	virtual void Release();

	
private:
	DWORD dwTime;
	float m_fEndPointX;
	float m_fEndPointY;

	float m_fNextStageX;
	float m_fNextStageY;

	bool m_bSign;

	// Scene을(를) 통해 상속됨
	virtual void SelectFrameKey() override;

	// Scene을(를) 통해 상속됨
	
};


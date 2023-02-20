#pragma once
#include "Scene.h"

class Scene02 : public Scene
{
public:
	virtual void Init();
	virtual void Update();
	virtual void Late_Update();
	virtual void Render();
	virtual void Release();

private:

	// Scene을(를) 통해 상속됨
	virtual void SelectFrameKey() override;
	bool m_bSpikeBallSpawned;
	class SpikeBall* m_SpikeBall;

	float m_fEndPointX;
	float m_fEndPointY;

	bool m_bSign;
};


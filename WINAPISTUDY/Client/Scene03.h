#pragma once
#include "Scene.h"
class Scene03 : public Scene
{
public:
	virtual void Init();
	virtual void Update();
	virtual void Late_Update();
	virtual void Render();
	virtual void Release();
    virtual void SelectFrameKey() override;

private:
	void BackgroundRender();
	void RandomMeteorGenerate();
	void SpawnBoss();
	void SpawnRings(int y = 0);
	void SpawnRocket();
	void Pattern1();
	void Pattern2();
	void PatternSelect();
	void DrawRectangleEndingScene();
	void StageStart();
	void FlyAwaySonicByeBye();
	void EndingSongDownVolume();

private:
	DWORD m_dwRandomMeteorTime;
	DWORD m_dwSpawnRingsTime;
	DWORD m_dwSpawnRocketTime;
	DWORD m_dwPattern1Time;
	DWORD m_dwPatternSelectTime;
	DWORD m_dwBossGenTime;
	DWORD m_dwDrawRectangleTime;
	DWORD m_dwStageStartTime;
	DWORD m_dwStageClearSceneTime;
	DWORD m_dwEndingSongDownVolume;

	int m_iPattern1Y;
	bool m_bPattern1State;
	float m_EndingSongVolume;
	
	bool m_bPattern1;
	bool m_bBossSpawned;
	bool m_bRectangleDrawFinished;
	bool m_bStageStart;
	bool m_bEndingCutScene;

	int m_topY;
	int m_bottomY;

	class DoomsDay* m_pBoss;
};


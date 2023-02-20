#pragma once

class GameObject;

struct LessRQ
{
	bool operator ()(IRender* a, IRender* b)
	{
		return a->GetZOrder() > b->GetZOrder();
	}
};

class Scene
{
public:
	Scene();
	virtual ~Scene();
	void AddGameObject(int ObjID, GameObject* NewObj);
	list<GameObject*>* GetGameObjectslist(int ObjID);
	void RemoveGameObjectsList(int ObjID);

public:
	virtual void Init();
	virtual void Update();
	virtual void Late_Update();
	virtual void Render();
	virtual void Release();
	void	ActClear();
	void    ActClearEnded();

protected:
	void			Move_Frame();
	void			Move_StartFrame();

	virtual void	SelectFrameKey() PURE;
	void			RenderScoreRings();
	void			RenderSonicLife();
	void			RenderBigNumber(int num, float x, float y);
	void			RenderSmallNumber(int num, float x, float y);
	void			RenderStageStart(int stageNum);
	void			RenderStageEnd(int stageNum);

	
	
protected:
	// �켱���� ť�� ���鶧 ������ �����̳�
	// ����Ʈ���ٰ� ����ټ��� �ְ� ���Ϳ��� ��
	priority_queue<IRender*,vector<IRender*>,LessRQ> m_RenderQueue;
	list<GameObject*> m_GameObjectList[OBJ_END];

	FRAME		m_Frame;
	wstring		m_FrameKey;

	FRAME		m_StartFrame;
	DWORD		m_dwStartTime;
	bool		m_bStartFrameEnded;
	bool		m_bActClear;
	

	
};



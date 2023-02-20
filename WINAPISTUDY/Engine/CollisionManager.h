#pragma once

class Collider;
class GameObject;

template<typename T> class Component;

enum COLLIDER_OFFSET
{
	CO_D,
	CO_DR,
	CO_DL,
	CO_R,
	CO_L,
	CO_UL,
	CO_UR,
	CO_U,
	CO_END
};

class CollisionManager
{
	DECLARE_SINGLE(CollisionManager)

private:
	void RemoveCollider(Collider* collider);
	void RenderCollider(Component<GameObject>* comp);
public:
	/* 콜라이더를 리스트에 추가*/
	void AddCollider(Collider* collider);
	/* 새로운 콜라이더를 만들고 리스트에 추가*/
	void NewCollider(GameObject* ThisObject, string Name, const Vec2& offset, const Vec2& size);
	void RemoveCollider(Component<GameObject>* collider);
	Collider* GetCollider(string name);

	void Render();
	void Late_Update();
	void Release();

	void EnableAllCollider();
	void DisableAllCollider();

	bool CollisionRectCheck(Collider* collider);
	
	Collider* CollisionRectReturn(Collider* collider);

	void Collision_Object_Line();
	void Collision_Player_Line();
	void Collision_With_Tile();
	void Collision_With_Obstacle(string colliderName, list<GameObject*>* imgObjList);
	void Collision_With_ObstacleCollider(string colliderNameSrc, list<GameObject*>* imgObjList);

	bool IsRectInLine(const RECT* rect, Line* line, float* fX, float* fY);
	bool IsRectInTile(const Vec2& pos, const Vec2& size, const class Tile* tile, float* pX, float* pY);
	bool IsRectInObstacle(const Vec2& pos, const Vec2& size, class ImageObject* obstacle, float* pX, float* pY);
	bool IsRectInCollider(const Vec2& pos, const Vec2& size, class Collider* obsCol, float* pX, float* pY);

private:
	void SetPlayerNewPos_LineCollision(class Player* player, const Vec2& changePos);

private:
	list<Collider*> m_ColliderList;
	bool m_CollidersActiveState = false;

public:
	bool GetCollidersActiveState() const { return m_CollidersActiveState; }
	int	GetCollidersNum() const { return m_ColliderList.size(); }
};

#define COLLISION CollisionManager::GetInstance()


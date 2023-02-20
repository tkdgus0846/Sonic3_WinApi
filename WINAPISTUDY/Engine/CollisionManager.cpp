#include "pch.h"
#include "CollisionManager.h"
#include "Collider.h"
#include "LineManager.h"
#include "TileManager.h"
#include "../Client/Player.h"
#include "ImageObject.h"
#include "ScrollManager.h"
#include "SpikeBall.h"

void CollisionManager::AddCollider(Collider* collider)
{
	m_ColliderList.push_back(collider);
	collider->SetActive(TRUE);
}

void CollisionManager::NewCollider(GameObject* ThisObject, string Name, const Vec2& offset, const Vec2& size)
{
	Collider* collider = new Collider(Name, offset, size);
	collider->SetOwner(ThisObject);
	AddCollider(collider);
	ThisObject->AddComponent(collider);
}

void CollisionManager::RemoveCollider(Collider* collider)
{
	if (collider)
	{
		collider->SetActive(false);
		m_ColliderList.remove(collider);
	}
	else
	{
		cout << "콜라이더가 존재하지 않아서 삭제 실패!" << endl;
	}
}

void CollisionManager::RemoveCollider(Component<GameObject>* collider)
{
	RemoveCollider(dynamic_cast<Collider*>(collider));
}

Collider* CollisionManager::GetCollider(string name)
{
	list<Collider*>::iterator it = m_ColliderList.begin();
	for (; it != m_ColliderList.end(); it++)
	{
		if ((*it)->GetName() == name)
		{
			break;
		}
	}

	if (it == m_ColliderList.end()) return nullptr;
	return (*it);
}

void CollisionManager::Render()
{
	for (Collider* item : m_ColliderList)
	{
		if (item)
		{
			item->Render();

		}
			
	}
}

void CollisionManager::Late_Update()
{
	for (Collider* item : m_ColliderList)
	{
		if (item)
			item->Update();
	}
}

void CollisionManager::Release()
{
	for (auto iter = m_ColliderList.begin(); iter != m_ColliderList.end();)
	{
		if (*iter)
		{
			Safe_Delete(&*iter);
			iter = m_ColliderList.erase(iter);
		}
	}
}

void CollisionManager::EnableAllCollider()
{
	m_CollidersActiveState = true;
	for (auto it : m_ColliderList)
	{
		(*it).SetActive(true);
	}
}

void CollisionManager::DisableAllCollider()
{
	m_CollidersActiveState = false;
	for (auto it : m_ColliderList)
	{
		(*it).SetActive(false);
	}
}

void CollisionManager::RenderCollider(Component<GameObject>* comp)
{
	Collider* c = dynamic_cast<Collider*>(comp);
	if (c)
	{
		c->Render();
	}
}

bool CollisionManager::CollisionRectCheck(Collider* collider)
{
	RECT myArea = collider->GetOwner()->GetArea();

	for (Collider* it : m_ColliderList)
	{
		if (collider == it) continue;
		if (it == nullptr) continue;
		
		RECT otherArea = it->GetOwner()->GetArea();
		RECT interArea;
		if (IntersectRect(&interArea, &myArea, &otherArea))
		{
			return true;
		}
	}
	return false;
}

Collider* CollisionManager::CollisionRectReturn(Collider* collider)
{
	RECT myArea = collider->GetOwner()->GetArea();

	for (Collider* it : m_ColliderList)
	{
		if (collider == it) continue;
		if (it == nullptr) continue;

		RECT otherArea = it->GetOwner()->GetArea();
		RECT interArea;
		if (IntersectRect(&interArea, &myArea, &otherArea))
		{
			return it;
		}
	}
	return nullptr;
}

void CollisionManager::Collision_Object_Line()
{
	for (Collider* it : m_ColliderList)
	{
		if (it->GetName() == "DropRingCollider" || it->GetName() == "SignCollider" || it->GetName() == "SpikeBallCollider")
		{
			RECT myRect = it->GetArea();
			Vec2 myPos = it->GetOwner()->GetPos();

			Line* targetLine = nullptr;

			vector<Line*>* lineList = LINE_MANAGER->GetLineVec();
			if (lineList == nullptr) return;
			if (lineList->empty()) return;

			for (Line* line : *lineList)
			{
				if (IsRectInLine(&myRect, line, &myPos.x, &myPos.y))
				{
					targetLine = line;
					break;
				}
				if (targetLine) break;
			}

			if (targetLine)
			{
				Vec2 newPos;
				if (it->GetName() == "DropRingCollider")
				{
					newPos = { myPos.x,myPos.y - 10 };
					it->GetOwner()->SetSpeed(0.f);
				}
				if (it->GetName() == "SignCollider")
				{
					newPos = { myPos.x,myPos.y - 50 };
					it->GetOwner()->SetSpeed(0.f);
				}
				if (it->GetName() == "SpikeBallCollider")
				{
					newPos = { myPos.x,myPos.y - 85 };
					it->GetOwner()->SetCurLine(targetLine);
				}
				
				
				it->GetOwner()->SetPos(newPos);
				it->GetOwner()->SetFalling(false);
				it->SetInCollision(true);
			}

			
		}
	}
}

void CollisionManager::Collision_Player_Line()
{
	Collider* collider = GetCollider("PlayerLineCollider");

	if (collider == nullptr) return;
	if (!collider->GetActive()) return;

	RECT myRect = collider->GetArea();
	Vec2 myPos = collider->GetOwner()->GetPos();

	Line* targetLine = nullptr;
	int index = 0;

	Player* player = dynamic_cast<Player*>(collider->GetOwner());

	vector<Line*>* lineList = LINE_MANAGER->GetLineVec();
	if (lineList->empty()) return;

	if (player->GetObjectDir() == OD_RIGHT)
	{
		for (auto riter = lineList->rbegin(); riter != lineList->rend(); riter++)
		{
			if (IsRectInLine(&myRect, *riter, &myPos.x, &myPos.y))
			{
				if (player->GetCurLine() == nullptr)
				{
					targetLine = *riter;
					break;
				}
				if (ZLAYER(player->GetCurLine()) == ZLAYER(*riter))
				{
					targetLine = *riter;
					break;
				}
				else if ((ZLAYER(player->GetCurLine()) != ZLAYER(*riter)) &&
					(abs(player->GetCurLine()->GetLineNum()-(*riter)->GetLineNum()) == 1))
				{
					targetLine = *riter;
					break;
				}
				
				
			}
		}
		
	}
	else if (player->GetObjectDir() == OD_LEFT)
	{
		for (auto line : *lineList)
		{
			if (IsRectInLine(&myRect, line, &myPos.x, &myPos.y))
			{
				if (player->GetCurLine() == nullptr)
				{
					targetLine = line;
					break;
				}
				if (ZLAYER(player->GetCurLine()) == ZLAYER(line))
				{
					targetLine = line;
					break;
				}
				else if ((ZLAYER(player->GetCurLine()) != ZLAYER(line)) &&
					(abs(player->GetCurLine()->GetLineNum() - (line)->GetLineNum()) == 1))
				{
					targetLine = line;
					break;
				}
			}
		}
	}

	


	/*if (targetLine.size() > 1 && player)
	{
		list<Line*>* linelist = nullptr;
		linelist = player->GetPrevLineList();
		for (auto lines : *linelist)
		{
			for (auto it = targetLine.begin(); it != targetLine.end();)
			{
				if (*it == lines)
				{
					it = targetLine.erase(it);
				}
				else
					it++;
			}
		}
	}
	else if (targetLine.size() == 1 && player)
	{
		if (player->FindLine(targetLine.front()) == false)
			player->AddPrevLine(targetLine.front());
	}*/
	// 지금 이 코드는 라인의 교차점에서 충돌을 감지하지 못하게하는 기능이 돼버렸다.


	// 감지 하자마자 떨어진다.
	
	if (targetLine)
	{
		if (player)
		{
			player->SetCurLine(targetLine);
		}

		if (collider->GetOwner()->GetJumping() || collider->GetOwner()->GetDescenting())
		{
			collider->SetInCollision(false);
		}

		if (collider->GetOwner()->GetFalling() || collider->GetInCollision() == true)
		{
			Vec2 newPos = { myPos.x,myPos.y - 40 };

			if (player)
			{
				/*if (player->GetColOffset() == CO_UR)
					cout << player->GetColOffset() << endl;*/
				player->SelectFrameKey();
				SetPlayerNewPos_LineCollision(player, myPos);
			}
			else
			{
				collider->GetOwner()->SetPos(newPos);
			}
			collider->SetInCollision(true);
			collider->GetOwner()->SetFalling(false);
			collider->GetOwner()->SetJumpTime(0.f);
			collider->GetOwner()->SetJumping(false);
			collider->GetOwner()->SetFallingSpeed(0.f);


		}
	}
	else
	{
		static int num = 0;
		/*cout << num++ << " NO 감지!! " << endl;*/
		if (player)
		{
			player->SetCurLine(nullptr);
			//player->ClearPrevLineList();
		}

	}
}

void CollisionManager::Collision_With_Tile()
{
	for (auto collider : m_ColliderList)
	{
		if (!collider->GetActive()) continue;
		if (collider->GetName() == "PlayerLineCollider" || collider->GetName() == "SpikeBallCollider")
		{
			continue;
		}

		RECT myRect = collider->GetArea();
		Vec2 myPos = collider->GetOwner()->GetPos();
		Vec2 mySize = collider->GetSize();

		list<Tile*>* tileList = TILE->GetTileList();
		for (auto tile : *tileList)
		{
			float px, py;
			if (IsRectInTile(myPos, mySize, tile, &px, &py))
			{
				// 상하 충돌
				if (px > py)
				{
					// 상 충돌
					if (myPos.y < tile->GetPos().y)
					{
						//cout << "상충돌!!" << endl;
						
						if (collider->GetOwner()->GetJumping())
						{
							collider->SetInCollision(false);
						}

						if (collider->GetOwner()->GetFalling() || collider->GetInCollision() == true)
						{
							//cout << py << endl;
							collider->GetOwner()->SetPos(Vec2(myPos.x, myPos.y - py - 9.5f));
							collider->SetInCollision(true);
							collider->GetOwner()->SetFalling(false);
							collider->GetOwner()->SetJumpTime(0.f);
							collider->GetOwner()->SetJumping(false);
							Player* player = dynamic_cast<Player*>(collider->GetOwner());
							if (player)
							{
								player->SetInvincible(false);
							}
							
						}
						
					}
					else // 하 충돌
					{
						/*_obj->Set_Y(py);*/
						//cout << "하충돌!!" << endl;
						collider->GetOwner()->SetPos(Vec2(myPos.x, myPos.y + py));

					}

				}
				else   // 좌우 충돌
				{
					// 좌 충돌
					if (myPos.x < tile->GetPos().x)
					{
						//cout << "좌충돌!!" << endl;
						collider->GetOwner()->SetPos(Vec2(myPos.x - px, myPos.y));
						//collider->GetOwner()->GetDir();
						Player* player = dynamic_cast<Player*>(collider->GetOwner());
						if (player)
						{
							// GetFalling이 true가 나온다 ..
							if (player->GetCanPush() && player->GetDir().x == 1 && player->GetFalling() == false && player->GetJumping() == false)
							{
								player->SetAttachToSide(true);
							}
							else if (player->GetCurState() == PS_JUMP)
							{
								player->SetDir({ -1,0 });
								player->SetAttachToSide(false);
							}
							else
							{
								player->SetAttachToSide(false);
								//collider->GetOwner()->SetDir({ -1,0 });
								collider->GetOwner()->SetPos(Vec2(myPos.x - px - 8.f, myPos.y));
								player->SetSpeed(0);
							}
						}
						else
						{
							collider->GetOwner()->SetDir({ -1,0 });
						}
						
					}
					else // 우 충돌
					{
						cout << "우충돌!!" << endl;
						collider->GetOwner()->SetPos(Vec2(myPos.x + px, myPos.y));
						Player* player = dynamic_cast<Player*>(collider->GetOwner());
						if (player)
						{
							if (player->GetCanPush() && player->GetDir().x == -1 && player->GetFalling() == false && player->GetJumping() == false)
							{
								player->SetAttachToSide(true);
							}
							else if (player->GetCurState() == PS_JUMP)
							{
								player->SetDir({ 1,0 });
								player->SetAttachToSide(false);
							}
							else
							{
								player->SetAttachToSide(false);
								//collider->GetOwner()->SetDir({ 1,0 });
								collider->GetOwner()->SetPos(Vec2(myPos.x + px + 8.f, myPos.y));
								player->SetSpeed(0);
							}
						}
						else
						{
							collider->GetOwner()->SetDir({ 1,0 });
						}
					}
				}
			}
		}

		
	}
}

void CollisionManager::Collision_With_Obstacle(string colliderName, list<GameObject*>* obstacleList)
{
	Collider* collider = GetCollider(colliderName);
	if (collider == nullptr) return;

	for (auto obj : *obstacleList)
	{
		ImageObject* obsObj = dynamic_cast<ImageObject*>(obj);
		if (obsObj == nullptr) continue;
		if (obsObj->GetComponentNum() != 0) continue;

		Vec2 myPos = collider->GetOwner()->GetPos();
		Vec2 mySize = collider->GetSize();

		float px, py;

		if (IsRectInObstacle(myPos, mySize, obsObj, &px, &py))
		{
			// 상하 충돌
			if (px > py)
			{
				// 상 충돌
				if (myPos.y < obsObj->GetPos().y)
				{
					obsObj->Collision_Up(collider, px, py);

				}
				else // 하 충돌
				{
					obsObj->Collision_Down(collider, px, py);
				}
			}
			else   // 좌우 충돌
			{
				// 좌 충돌
				if (myPos.x < obsObj->GetPos().x)
				{
					obsObj->Collision_Left(collider, px, py);
				}
				else // 우 충돌
				{
					obsObj->Collision_Right(collider, px, py);
				}
			}
		}
		else
		{
			obsObj->Collision_Exit();
		}
	}
}

void CollisionManager::Collision_With_ObstacleCollider(string colliderNameSrc, list<GameObject*>* imgObjList)
{
	Collider* collider = GetCollider(colliderNameSrc);
	if (collider == nullptr) return;

	for (auto obj : *imgObjList)
	{
		Collider* obsCol; 
		obsCol = dynamic_cast<Collider*>(obj->GetComponent("SignCollider"));
		if (obsCol == nullptr)
			obsCol = dynamic_cast<Collider*>(obj->GetComponent("DropRingCollider"));
		if (obsCol == nullptr)
			obsCol = dynamic_cast<Collider*>(obj->GetComponent("RocketCollider"));
		if (obsCol == nullptr)
			obsCol = dynamic_cast<Collider*>(obj->GetComponent("SpikeBallCollider"));

		if (obsCol == nullptr) continue;

		ImageObject* obsObj = dynamic_cast<ImageObject*>(obj);
		if (obsObj == nullptr) continue;

		Vec2 myPos = collider->GetPos();
		Vec2 mySize = collider->GetSize();

		float px, py;

		if (IsRectInCollider(myPos, mySize, obsCol, &px, &py))
		{
			// 상하 충돌
			if (px > py)
			{
				// 상 충돌
				if (myPos.y < obsObj->GetPos().y)
				{
					obsObj->Collision_Up(collider, px, py);

				}
				else // 하 충돌
				{
					obsObj->Collision_Down(collider, px, py);
				}
			}
			else   // 좌우 충돌
			{
				// 좌 충돌
				if (myPos.x < obsObj->GetPos().x)
				{
					obsObj->Collision_Left(collider, px, py);
				}
				else // 우 충돌
				{
					obsObj->Collision_Right(collider, px, py);
				}
			}
		}
		else
		{
			obsObj->Collision_Exit();
		}
	}
}

bool CollisionManager::IsRectInLine(const RECT* rect, Line* line, float* fX, float* fY)
{
	bool result = false;

	LONG cx = (rect->right - rect->left) / 2;
	LONG x = rect->left + cx;

	float incline = line->GetLineIncline();

	

	float x1 = line->GetPoint(LINE_LEFT).x;
	float y1 = line->GetPoint(LINE_LEFT).y;
	float x2 = line->GetPoint(LINE_RIGHT).x;
	float y2 = line->GetPoint(LINE_RIGHT).y;

	float topY = line->GetPoint(LINE_TOP).y;
	float bottomY = line->GetPoint(LINE_BOTTOM).y;

	if (rect->right < x1 || rect->left > x2) return false;
	if (rect->bottom < topY || rect->top > bottomY) return false;
	

	float leftY = incline * (rect->left - x1) + y1;
	float rightY = incline * (rect->right - x1) + y1;

	float topX = (rect->top - y1) / incline + x1;
	float bottomX = (rect->bottom - y1) / incline + x1;

	//cout << "topX: " << topX << " bottomX: " << bottomX << " leftY: " << leftY << " rightY: " << rightY << endl;

	if ((leftY >= rect->top && leftY <= rect->bottom) || 
		(topX >= rect->left && topX <= rect->right))
	{
		result = true;
	}

	if ((rightY >= rect->top && rightY <= rect->bottom) ||
		(bottomX >= rect->left && bottomX <= rect->right))
	{
		
		result = true;
	}

	// 기울기 수치 때문에 비정상적으로 fy값이 커져서 땅으로 꺼져버린다. 다른 방법을 찾아야 할듯. 기울기가 큰 직선을 만났을때 어떻게 처리해야할까 ?
	if (result)
	{
		if (incline >= 8.f)
		{
			*fX = x1;
		}
		else if (incline <= -8.f)
		{
			*fX = x1;
		}
		else 
		{
			*fY = incline * (x - x1) + y1;
		}
		
		
	}
	
	return result;
}

bool CollisionManager::IsRectInTile(const Vec2& pos, const Vec2& size, const Tile* tile, float* pX, float* pY)
{
	float	fX = fabs(pos.x - tile->GetPos().x);
	float	fY = fabs(pos.y - tile->GetPos().y);

	float	fRadiusX = (size.x + tile->GetSize().x) * 0.5f;
	float	fRadiusY = (size.y + tile->GetSize().y) * 0.5f;

	if ((fRadiusX > fX) && (fRadiusY > fY))
	{
		*pX = fRadiusX - fX;
		*pY = fRadiusY - fY;

		return true;
	}

	return false;
}

bool CollisionManager::IsRectInObstacle(const Vec2& pos, const Vec2& size, ImageObject* obstacle, float* pX, float* pY)
{
	float	fX = fabs(pos.x - obstacle->GetPos().x);
	float	fY = fabs(pos.y - obstacle->GetPos().y);

	float	fRadiusX = (size.x + obstacle->GetSize().x) * 0.5f;
	float	fRadiusY = (size.y + obstacle->GetSize().y) * 0.5f;

	if ((fRadiusX > fX) && (fRadiusY > fY))
	{
		*pX = fRadiusX - fX;
		*pY = fRadiusY - fY;

		return true;
	}

	return false;
}

bool CollisionManager::IsRectInCollider(const Vec2& pos, const Vec2& size, Collider* obsCol, float* pX, float* pY)
{
	float	fX = fabs(pos.x - obsCol->GetPos().x);
	float	fY = fabs(pos.y - obsCol->GetPos().y);

	float	fRadiusX = (size.x + obsCol->GetSize().x) * 0.5f;
	float	fRadiusY = (size.y + obsCol->GetSize().y) * 0.5f;

	if ((fRadiusX > fX) && (fRadiusY > fY))
	{
		*pX = fRadiusX - fX;
		*pY = fRadiusY - fY;

		return true;
	}

	return false;
}

void CollisionManager::SetPlayerNewPos_LineCollision(Player* player, const Vec2& changePos)
{
	Vec2 newPos;
	COLLIDER_OFFSET offset = player->GetColOffset();
	switch (offset)
	{
	case CO_D:
		newPos = { changePos.x,changePos.y - 40 };
		player->SetPos(newPos);
		break;
	case CO_DR:
		newPos = { changePos.x,changePos.y - 40 };
		player->SetPos(newPos);
		break;
	case CO_DL:
		newPos = { changePos.x,changePos.y - 40 };
		player->SetPos(newPos);
		break;
	case CO_R:
		newPos = { changePos.x - 40,changePos.y};
		player->SetPos(newPos);
		break;
	case CO_L:
		newPos = { changePos.x + 40,changePos.y};
		player->SetPos(newPos);
		break;
	case CO_UL:
		newPos = { changePos.x,changePos.y + 40 };
		player->SetPos(newPos);
		break;
	case CO_UR:
		newPos = { changePos.x,changePos.y + 40 };
		player->SetPos(newPos);
		break;
	case CO_U:
		newPos = { changePos.x,changePos.y + 40 };
		player->SetPos(newPos);
		break;
	case CO_END:
		break;
	default:
		break;
	}
	
}

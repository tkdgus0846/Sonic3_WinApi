#pragma once
#include "GameObject.h"

template<typename T>
class AbstractFactory
{
private:
	AbstractFactory() {}
	~AbstractFactory() {}
public:
	static GameObject* Create()
	{
		GameObject* obj = new T();
		obj->Init();
	}
	static GameObject* Create(Vec2 Vec)
	{
		GameObject* obj = new T(Vec);
		obj->Init();
		return obj;
	}
	static GameObject* Create(Vec2 vec, int ZOrder)
	{
		GameObject* obj = new T(vec);
		obj->Init();
		obj->SetZOrder(ZOrder);
		return obj;
	}
	
};

#define FACTORY(T) AbstractFactory<T>
#define FACTORY_CREATE(T,Pos)	AbstractFactory<T>::Create(Pos)
#define FACTORY_CREATE_WITH_ZORDER(T,Pos,ZOrder) AbstractFactory<T>::Create(Pos,ZOrder);

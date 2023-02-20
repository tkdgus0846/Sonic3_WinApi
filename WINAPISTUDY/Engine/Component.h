#pragma once

template<typename T>
class Component
{
public:
	Component() :
		m_Name("None"),
		m_Owner(nullptr),
		m_Active(TRUE)
	{}
	Component(string Name) :
		m_Name(Name),
		m_Owner(nullptr),
		m_Active(TRUE)
	{
	}
	virtual ~Component() {}

public:
	//void			ComponentRender() PURE;

public:
	inline void		SetOwner(T* Owner)		{ m_Owner = Owner; }
	inline T*		GetOwner() const		{ return m_Owner; }
	inline void		SetName(string Name)	{ m_Name = Name; }
	inline string	GetName() const			{ return m_Name; }
	virtual void	SetActive(bool Active)	{ m_Active = Active; }
	bool	GetActive() const		{ return m_Active; }

private:
	string		m_Name;
	T*			m_Owner;
	bool		m_Active;
};

template<typename T>
class Composite : public Component<T>
{
private:
	
public:
	Composite() {}
	virtual ~Composite()
	{
		for (auto it = m_ChildList.begin(); it != m_ChildList.end();)
		{
			if ((*it) != nullptr)
				delete (*it);
			*it = nullptr;
			it = m_ChildList.erase(it);
		}
	}
	void AddComponent(Component<T>* NewComp)
	{
		m_ChildList.push_back(NewComp);
	}
	
	bool RemoveComponent(string Name)
	{
		for (auto it = m_ChildList.begin(); it != m_ChildList.end(); it++)
		{
			if ((*it)->GetName() == Name)
			{
				if (*it != nullptr )
					delete (*it);
				m_ChildList.erase(it);
				return true;
			}
		}
		return false;
	}
	Component<T>* GetComponent(string Name)
	{
		for (Component<T>* it : m_ChildList)
		{
			if (it->GetName() == Name)
				return (it);
		}
		return nullptr;
	}
	int GetComponentNum()
	{
		return m_ChildList.size();
	}
	void ComponentRender()
	{
		for (Component<T>* item : m_ChildList)
		{
			item->ComponentRender();
		}
	}
	void PrintChildList()
	{
		for (Component<T>* it : m_ChildList)
		{
			cout << it->GetName() << endl;
		}
	}

protected:
	list<Component<T>*> m_ChildList;
};

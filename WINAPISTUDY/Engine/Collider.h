#pragma once

class Collider : public Component<GameObject>
{
public:
	Collider();
	Collider(string _Name, const Vec2& offsetPos, const Vec2& size);

public:
	void Update();
	void Render();
	
	void OnCollisionEnter(Collider* Other);
	void OnCollisionExit();

private:
	void Update_Rect();

private:
	COLORREF	m_Color;
	Vec2		m_OffsetPos;
	Vec2		m_Pos;
	Vec2		m_Size;
	RECT		m_Area;
	bool		m_InCollision;

public:
	bool GetInCollision() const		{ return m_InCollision; }
	bool SetInCollision(bool state)	{ return m_InCollision = state; }
	RECT GetArea()			const	{ return m_Area; }
	void SetOffsetPos(const Vec2& vec)	{ m_OffsetPos = vec; }
	Vec2 GetOffsetPos()			const	{ return m_OffsetPos; }
	void SetSize(const Vec2& size)	{ m_Size = size; }
	Vec2 GetSize()			const	{ return m_Size; }
	Vec2 GetPos()			const	{ return m_Pos; }

	void SetOffset(const pair<Vec2, Vec2>& offset)
	{
		SetOffsetPos(offset.first);
		SetSize(offset.second);
	}
};


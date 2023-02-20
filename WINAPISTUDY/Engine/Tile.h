#pragma once

class Tile
{
public:
	Tile() {}
	Tile(const Vec2& pos, const Vec2& size);
	Tile(const RECT& rect, const Vec2& size);
	~Tile();


	void Render();
	void Update();

private:
	Vec2 m_Pos;
	Vec2 m_Size;
	RECT m_Area;

public:
	RECT GetArea()			const	{ return m_Area; }
	void SetPos(Vec2 vec)			{ m_Pos = vec; }
	Vec2 GetPos()			const	{ return m_Pos; }
	void SetSize(Vec2 size)			{ m_Size = size; }
	Vec2 GetSize() const			{ return m_Size; }
	RECT GetRect() const			{ return m_Area; }
	void SetRect(const RECT& rct)	{ m_Area = rct; }
};


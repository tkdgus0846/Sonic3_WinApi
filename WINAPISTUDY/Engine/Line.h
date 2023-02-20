#pragma once

enum LineDir
{
	LINE_LEFT, LINE_RIGHT, LINE_BOTTOM, LINE_TOP, LINE_END
};

enum LINELAYER
{
	LL_FORWARD, LL_REVERSE,  LL_END
};

class Line
{
public:
	Line() {}
	Line(const Vec2& p1, const Vec2& p2, LINELAYER layer, int LineNum);
	Vec2 GetPoint(LineDir dir) const { return point[dir]; }
	void Render();
	void Update();

	float GetLineIncline();
	float GetRadian();
	float GetAngle();

	Vec2 GetDirection();

	LINELAYER	GetLayer() const { return m_eLayer; }
	void		SetLayer(LINELAYER layer) { m_eLayer = layer; }

private:
	Vec2 point[LINE_END];
	LINELAYER m_eLayer;
	float m_fIncline;
	float m_fRadian;
	float m_fAngle;
	Vec2  m_Direction;
	int	m_LineNum;

public:
	int GetLineNum() const { return m_LineNum; }
	void SetLineNum(int lineNum) { m_LineNum = lineNum; }
};


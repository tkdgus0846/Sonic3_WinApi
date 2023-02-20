#include "pch.h"
#include "RenderManager.h"
#include "ResourceManager.h"
#include "ScrollManager.h"

void RenderManager::Init(HWND hwnd)
{
	m_Hwnd = hwnd;
	m_HDC = GetDC(m_Hwnd);
	m_MemDC = CreateCompatibleDC(m_HDC);

	m_PenType = PENTYPE::Solid;
	m_PenWidth = 2;
	m_PenColor = RGB(0, 0, 0);
	m_CurPen = CreatePen((int)m_PenType, m_PenWidth, m_PenColor);

	m_BrushType = BRUSHTYPE::Solid;
	m_BrushColor = RGB(0xff, 0xff, 0xff);
	m_CurBrush = CreateSolidBrush(m_BrushColor);

	m_BMP = CreateCompatibleBitmap(m_HDC, WINDOW_WIDTH, WINDOW_HEIGHT);

	HBITMAP hOldBitmap = static_cast<HBITMAP>(SelectObject(m_MemDC, m_BMP));
	DeleteObject(hOldBitmap);

	hFont = 0;
	textSize = 10;
	textColor = RGB(0, 0, 0);
	textAlign = TEXTALIGN::Center;
	textBackMode = TEXTBACKMODE::Null;
	textBackColor = RGB(255, 255, 255);
}

void RenderManager::Release()
{
	DeleteObject(m_MemDC);
	ReleaseDC(m_Hwnd, m_HDC);
	DeleteObject(m_CurPen);
	DeleteObject(m_CurBrush);
	DeleteObject(m_BMP);

	m_HDC = 0;
	m_MemDC = 0;
}

void RenderManager::SetBrush(COLORREF color, BRUSHTYPE brushType)
{
	if (color == m_BrushColor && brushType == m_BrushType) return;

	m_BrushColor = color;
	m_BrushType = brushType;

	DeleteObject(m_CurBrush);

	switch (m_BrushType)
	{
	case BRUSHTYPE::Solid:
		m_CurBrush = CreateSolidBrush(m_BrushColor);
		break;
	case BRUSHTYPE::Null:
		m_CurBrush = static_cast<HBRUSH>(GetStockObject(NULL_BRUSH));
		break;
	default:
		m_CurBrush = CreateSolidBrush(m_BrushColor);
		break;
	}
}

void RenderManager::SetPen(COLORREF color, PENTYPE penType, int width)
{
	if (color == m_PenColor && penType == m_PenType && width == m_PenWidth) return;

	m_PenColor = color;
	m_PenType = penType;
	m_PenWidth = width;

	DeleteObject(m_CurPen);
	m_CurPen = CreatePen((int)m_PenType, m_PenWidth, m_PenColor);
}

void RenderManager::SetText(int size, COLORREF color, TEXTALIGN align)
{
	// 선택하는 텍스트가 현재 텍스트와 동일할 경우 새로 만들지 않음
	if (textSize == size && textColor == color && textAlign == align)
		return;

	textSize = size;
	textColor = color;
	textAlign = align;

	DeleteObject(hFont);
	hFont = CreateFont(size, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET,
		0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("굴림"));
	SelectObject(m_MemDC, hFont);

	SetTextColor(m_MemDC, color);

	switch (align)
	{
	case TEXTALIGN::Top:
		SetTextAlign(m_MemDC, TA_TOP);
		break;
	case TEXTALIGN::Bottom:
		SetTextAlign(m_MemDC, TA_BOTTOM);
		break;
	case TEXTALIGN::Left:
		SetTextAlign(m_MemDC, TA_LEFT);
		break;
	case TEXTALIGN::Right:
		SetTextAlign(m_MemDC, TA_RIGHT);
		break;
	case TEXTALIGN::Center:
		SetTextAlign(m_MemDC, TA_CENTER);
		break;
	default:
		SetTextAlign(m_MemDC, TA_TOP);
		break;
	}
}

void RenderManager::SetTextBackMode(TEXTBACKMODE mode, COLORREF color)
{
	// 선택하는 텍스트 배경이 현재 텍스트와 동일할 경우 새로 만들지 않음
	if (textBackMode == mode && textBackColor == color)
		return;

	switch (mode)
	{
	case TEXTBACKMODE::Null:
		SetBkMode(m_MemDC, TRANSPARENT);
		break;
	case TEXTBACKMODE::Solid:
		SetBkMode(m_MemDC, OPAQUE);
		break;
	default:
		SetBkMode(m_MemDC, TRANSPARENT);
		break;
	}
}

void RenderManager::BeginDraw()
{
	PatBlt(m_MemDC, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, BLACKNESS);
}

void RenderManager::EndDraw()
{
	BitBlt(m_HDC, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, m_MemDC, 0, 0, SRCCOPY);
}

void RenderManager::DrawLine(float x1, float y1, float x2, float y2)
{
	HPEN OldPen = (HPEN)SelectObject(m_MemDC, m_CurPen);
	HBRUSH OldBrush = (HBRUSH)SelectObject(m_MemDC, m_CurBrush);

	MoveToEx(m_MemDC, x1, y1, nullptr);
	LineTo(m_MemDC, x2, y2);

	SelectObject(m_MemDC, OldPen);
	SelectObject(m_MemDC, OldBrush);
}

void RenderManager::DrawCircle(int x, int y, int radius)
{
	DrawEllipse(x - radius, y - radius, x + radius, y + radius);
}

void RenderManager::DrawCircle(Vec2 pos, int radius)
{
	DrawCircle(pos.x, pos.y, radius);
}

void RenderManager::DrawEllipse(int left, int top, int right, int bottom)
{
	HPEN OldPen = (HPEN)SelectObject(m_MemDC, m_CurPen);
	HBRUSH OldBrush = (HBRUSH)SelectObject(m_MemDC, m_CurBrush);

	Ellipse(m_MemDC, left, top, right, bottom);

	SelectObject(m_MemDC, OldPen);
	SelectObject(m_MemDC, OldBrush);
}

void RenderManager::DrawRect(int left, int top, int right, int bottom)
{
	HPEN OldPen = (HPEN)SelectObject(m_MemDC, m_CurPen);
	HBRUSH OldBrush = (HBRUSH)SelectObject(m_MemDC, m_CurBrush);

	Rectangle(m_MemDC, left, top, right, bottom);

	SelectObject(m_MemDC, OldPen);
	SelectObject(m_MemDC, OldBrush);
}

void RenderManager::DrawRegularRect(Vec2 pos, int width)
{
	int HalfWidth = width / 2;
	DrawRect(pos.x - HalfWidth, pos.y - HalfWidth, pos.x + HalfWidth, pos.y + HalfWidth);
}

void RenderManager::DrawPolyBezier(const POINT* apt, DWORD cpt)
{
	bool Success = PolyBezier(m_MemDC, apt, cpt);
	//cout << Success << endl;
}

void RenderManager::Text(int x, int y, const wstring& text)
{
	TextOut(m_MemDC, (int)x, (int)y, text.c_str(), (int)text.size());
}

void RenderManager::BitBltImage(const wstring& key, int x, int y, int cx, int cy, int x1, int y1)
{
	int      ScrollX = (int)SCROLLX;
	int      ScrollY = (int)SCROLLY;

	HDC MemDC = FIND_IMAGE(key);
	BitBlt
	(
		m_MemDC,
		x + ScrollX,
		y + ScrollY,
		cx,
		cy,
		MemDC,
		x1,
		y1,
		SRCCOPY
	);
}

void RenderManager::TransparentImage(const wstring& key, int xoriginDest, int yoriginDest, int wDest, int hDest, int xoriginSrc, int yoriginSrc, int wSrc, int hSrc, COLORREF color)
{
	int      ScrollX = (int)SCROLLX;
	int      ScrollY = (int)SCROLLY;

	HDC MemDC = FIND_IMAGE(key);
	GdiTransparentBlt
	(
		m_MemDC,
		xoriginDest + ScrollX,
		yoriginDest + ScrollY,
		wDest,
		hDest,
		MemDC,
		xoriginSrc,
		yoriginSrc,
		wSrc,
		hSrc,
		color
	);
}

void RenderManager::TransparentImage_NoScroll(const wstring& key, int xoriginDest, int yoriginDest, int wDest, int hDest, int xoriginSrc, int yoriginSrc, int wSrc, int hSrc, COLORREF color)
{
	HDC MemDC = FIND_IMAGE(key);
	GdiTransparentBlt
	(
		m_MemDC,
		xoriginDest,
		yoriginDest,
		wDest,
		hDest,
		MemDC,
		xoriginSrc,
		yoriginSrc,
		wSrc,
		hSrc,
		color
	);
}

void RenderManager::StretchImage_NoScroll(const wstring& key, int xDest, int yDest, int wDest, int hDest, int xSrc, int ySrc, int wSrc, int hSrc)
{
	HDC MemDC = FIND_IMAGE(key);
	StretchBlt
	(
		m_MemDC,
		xDest,
		yDest,
		wDest,
		hDest,
		MemDC,
		xSrc,
		ySrc,
		wSrc,
		hSrc,
		SRCCOPY
	);
}

void RenderManager::StretchImage(const wstring& key, int xDest, int yDest, int wDest, int hDest, int xSrc, int ySrc, int wSrc, int hSrc)
{
	int      ScrollX = (int)SCROLLX;
	int      ScrollY = (int)SCROLLY;

	HDC MemDC = FIND_IMAGE(key);
	StretchBlt
	(
		m_MemDC,
		xDest + ScrollX,
		yDest + ScrollY,
		wDest,
		hDest,
		MemDC,
		xSrc,
		ySrc,
		wSrc,
		hSrc,
		SRCCOPY
	);
}

void RenderManager::StretchImage_Background(const wstring& key, int xDest, int yDest, int wDest, int hDest, int xSrc, int ySrc, int wSrc, int hSrc)
{
	int      ScrollX = (int)SCROLLX;
	int      ScrollY = (int)SCROLLY;

	/*cout << "x값:" << xSrc << " 스크롤 값:"<< ScrollX << " 값: " << xSrc - ScrollX * 0.1f << endl;*/
	HDC MemDC = FIND_IMAGE(key);
	StretchBlt
	(
		m_MemDC,
		xDest,
		yDest,
		wDest,
		hDest,
		MemDC,
		xSrc - ScrollX * 0.1f ,
		ySrc - ScrollY * 0.01f,
		wSrc,
		hSrc,
		SRCCOPY
	);
}

void RenderManager::Animation_BitBlt(const wstring& key, const FRAME& frame, int printX, int printY, int printCX, int printCY, int imageIntervalX, int imageIntervalY)
{
	BitBltImage
	(
		key,
		printX,
		printY,
		printCX,
		printCY,
		frame.iFrameStart * (printCX + imageIntervalX),
		frame.iMotion * (printCY + imageIntervalY)
	);
}

void RenderManager::Animation_StretchBlt(const wstring& key, const FRAME& frame, int printX, int printY, int printCX, int printCY, int originCX, int originCY, int imageIntervalX, int imageIntervalY)
{
	StretchImage
	(
		key,
		printX,
		printY,
		printCX,
		printCY,
		frame.iFrameStart * (originCX + imageIntervalX),
		frame.iMotion * (originCY + imageIntervalY),
		originCX,
		originCY
	);
}

void RenderManager::Animation_TransparentBlt(const wstring& key, const FRAME& frame, int printX, int printY, int printCX, int printCY, int originCX, int originCY, COLORREF color, int imageIntervalX, int imageIntervalY)
{
	TransparentImage
	(
		key,
		printX,
		printY,
		printCX,
		printCY,
		frame.iFrameStart * (originCX + imageIntervalX),
		frame.iMotion * (originCY + imageIntervalY),
		originCX,
		originCY,
		color
	);
}

void RenderManager::Animation_TransparentBlt_NoScroll(const wstring& key, const FRAME& frame, int printX, int printY, int printCX, int printCY, int originCX, int originCY, COLORREF color, int imageIntervalX, int imageIntervalY)
{
	TransparentImage_NoScroll
	(
		key,
		printX,
		printY,
		printCX,
		printCY,
		frame.iFrameStart * (originCX + imageIntervalX),
		frame.iMotion * (originCY + imageIntervalY),
		originCX,
		originCY,
		color
	);
}



#pragma once

enum class PENTYPE { Solid = PS_SOLID, Dash = PS_DASH, Dot = PS_DOT, Null = PS_NULL };
enum class BRUSHTYPE { Solid = BS_SOLID, Null = BS_NULL };
enum class TEXTALIGN { Top, Bottom, Left, Right, Center };
enum class TEXTBACKMODE { Solid, Null };

class RenderManager
{
	DECLARE_SINGLE(RenderManager)

public:

	void Init(HWND hwnd);
	void Release();

	void SetBrush(COLORREF color, BRUSHTYPE brushType = BRUSHTYPE::Solid);
	void SetPen(COLORREF color, PENTYPE penType = PENTYPE::Solid, int width = 2);
	void SetText(int size = 10, COLORREF color = RGB(0, 0, 0), TEXTALIGN align = TEXTALIGN::Center);
	void SetTextBackMode(TEXTBACKMODE mode = TEXTBACKMODE::Null, COLORREF color = RGB(255, 255, 255));

	void BeginDraw();
	void EndDraw();
	void DrawLine(float x1, float y1, float x2, float y2);
	//void DrawLine(class LINE line);
	void DrawCircle(int x, int y, int radius);
	void DrawCircle(Vec2 pos, int radius);
	void DrawEllipse(int left, int top, int right, int bottom);
	void DrawRect(int left, int top, int right, int bottom);
	void DrawRegularRect(Vec2 pos, int width);
	void DrawPolyBezier(const POINT* apt, DWORD cpt);
	void Text(int x, int y, const wstring& text);

	void BitBltImage(const wstring& key, int x, int y, int cx, int cy, int x1, int y1);
	void TransparentImage(const wstring& key, int xoriginDest, int yoriginDest, int wDest, int hDest, int xoriginSrc, int yoriginSrc, int wSrc, int hSrc, COLORREF color);
	void TransparentImage_NoScroll(const wstring& key, int xoriginDest, int yoriginDest, int wDest, int hDest, int xoriginSrc, int yoriginSrc, int wSrc, int hSrc, COLORREF color);
	void StretchImage_NoScroll(const wstring& key, int xDest, int yDest, int wDest, int hDest, int xSrc, int ySrc, int wSrc, int hSrc);
	void StretchImage(const wstring& key, int xDest, int yDest, int wDest, int hDest, int xSrc, int ySrc, int wSrc, int hSrc);
	void StretchImage_Background(const wstring& key, int xDest, int yDest, int wDest, int hDest, int xSrc, int ySrc, int wSrc, int hSrc);

	void Animation_BitBlt(const wstring& key, const FRAME& frame, int printX, int printY, int printCX, int printCY, int imageIntervalX, int imageIntervalY);
	void Animation_StretchBlt(const wstring& key, const FRAME& frame, int printX, int printY, int printCX, int printCY, int originCX, int originCY, int imageIntervalX, int imageIntervalY);
	void Animation_TransparentBlt(const wstring& key, const FRAME& frame, int printX, int printY, int printCX, int printCY, int originCX, int originCY, COLORREF color, int imageIntervalX, int imageIntervalY);
	void Animation_TransparentBlt_NoScroll(const wstring& key, const FRAME& frame, int printX, int printY, int printCX, int printCY, int originCX, int originCY, COLORREF color, int imageIntervalX, int imageIntervalY);
	


private:
	PAINTSTRUCT m_PS;
	HWND		m_Hwnd;		// 윈도우 핸들

	HDC			m_HDC;		// 프론트
	HDC			m_MemDC;	// 백
	HBITMAP		m_BMP;		// 백 비트맵

	// 펜
	HPEN		m_CurPen;
	int			m_PenWidth;
	COLORREF	m_PenColor;
	PENTYPE		m_PenType;

	// 브러쉬
	HBRUSH		m_CurBrush;
	BRUSHTYPE	m_BrushType;
	COLORREF	m_BrushColor;

	// 텍스트
	HFONT			hFont;
	int				textSize;
	COLORREF		textColor;
	TEXTALIGN		textAlign;
	TEXTBACKMODE	textBackMode;
	COLORREF		textBackColor;

public:
	HDC	Get_MemDC() const { return m_MemDC; }
	
};

#define RENDER	RenderManager::GetInstance()
#define MAINDC	RENDER->Get_MemDC()


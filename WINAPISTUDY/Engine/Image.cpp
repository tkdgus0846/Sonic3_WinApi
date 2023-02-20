#include "pch.h"
#include "Image.h"

Image::Image()
{
	m_hBitmap = 0;
	m_hDC = 0;
}

Image::~Image()
{
	DeleteDC(m_hDC);
	DeleteObject(m_hBitmap);
}

void Image::Load(const wstring& path)
{
	m_hBitmap = (HBITMAP)LoadImage(
		nullptr,
		path.c_str(),
		IMAGE_BITMAP, // 이미지 타입, 비트맵 이미지로 지정
		0, 0,		  // 이미지의 X, Y 크기, 0을 주면 이미지 크기로
		LR_CREATEDIBSECTION | LR_LOADFROMFILE);

	m_hDC = CreateCompatibleDC(MAINDC);
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(m_hDC, m_hBitmap);
	DeleteObject(hOldBitmap);
}

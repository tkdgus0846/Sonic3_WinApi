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
		IMAGE_BITMAP, // �̹��� Ÿ��, ��Ʈ�� �̹����� ����
		0, 0,		  // �̹����� X, Y ũ��, 0�� �ָ� �̹��� ũ���
		LR_CREATEDIBSECTION | LR_LOADFROMFILE);

	m_hDC = CreateCompatibleDC(MAINDC);
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(m_hDC, m_hBitmap);
	DeleteObject(hOldBitmap);
}

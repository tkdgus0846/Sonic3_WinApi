#pragma once
#include "Resource.h"

class Image : public Resource
{
public:
	Image();
	virtual ~Image();

public:
	void Load(const wstring& path);

	HDC Get_MemDC() const { return m_hDC; }

private:
	HBITMAP		m_hBitmap;
	

	HDC			m_hDC;
};


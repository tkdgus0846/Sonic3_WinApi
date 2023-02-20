#pragma once
#include "Resource.h"

class Image;



class Animation : public Resource
{
public:
	Animation();
	virtual ~Animation();

public:
	void Create(Image* image, int iFrameStart, int iFrameEnd, int iMotion, DWORD dwSpeed, DWORD dwTime, bool repeat = true);
	void Create(Image* image, const FRAME& frame, bool repeat = true);

private:
	Image*				image;		// 애니메이션 이미지
	FRAME				frame;
	bool				repeat;		// 애니메이션 반복여부
};


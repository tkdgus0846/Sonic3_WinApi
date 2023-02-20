#include "pch.h"
#include "Animation.h"

Animation::Animation()
{
}

Animation::~Animation()
{
}

void Animation::Create(Image* image, int iFrameStart, int iFrameEnd, int iMotion, DWORD dwSpeed, DWORD dwTime, bool repeat)
{
	this->image = image;
	this->repeat = repeat;

	frame.iFrameStart = iFrameStart;
	frame.iFrameEnd = iFrameEnd;
	frame.iMotion = iMotion;
	frame.dwSpeed = dwSpeed;
	frame.dwTime = dwTime;
}

void Animation::Create(Image* image, const FRAME& frame, bool repeat)
{
	Create(image, { frame.iFrameStart, frame.iFrameEnd, frame.iMotion, frame.dwSpeed, frame.dwTime }, repeat);
}

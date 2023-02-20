#pragma once

struct Vec2
{
	Vec2() : x(0), y(0) {}
	Vec2(float _x, float _y) : x(_x), y(_y) {}
	float x, y;

	void Normalize()
	{
		float d = sqrtf(x * x) + sqrtf(y * y);
		float x1 = x / d;
		float y1 = y / d;

		x = (x1);
		y = (y1);
	}
	float GetIncline()
	{
		return y/x;
	}
	float GetAngle()
	{
		float m_fIncline = y / x;
		return atanf(m_fIncline) * 180.f / PI;
	}

	Vec2 operator + (const Vec2& _vec)
	{
		return Vec2(x + _vec.x, y + _vec.y);
	}
	Vec2 operator - (const Vec2& _vec)
	{
		return Vec2(x - _vec.x, y - _vec.y);
	}

	Vec2 operator * (float f)
	{
		return Vec2(x * f, y * f);
	}

	bool operator ==(const Vec2& vec)
	{
		return (x == vec.x) && (y == vec.y);
	}

	bool operator != (const Vec2& vec)
	{
		return (x != vec.x) || (y != vec.y);
	}

};

typedef struct tagFrame
{
	int		iFrameStart;
	int		iFrameEnd;
	int		iMotion;

	DWORD	dwSpeed;
	DWORD	dwTime;

	bool    repeat;

}FRAME;

enum OBJID
{
	OBJ_PLAYER, OBJ_MISSILE, OBJ_MONSTER, OBJ_IMAGE, OBJ_OBSTACLE, OBJ_SKILL, OBJ_END
};

enum CHANNELID { SOUND_BGM, SOUND_EFFECT, SOUND_SONIC, SOUND_OBJ, SOUND_SUSTAIN, SOUND_EJECT, MAXCHANNEL };

enum EDITERSELECTMODE
{
	DRAW,
	ERASE,
	END
};
#pragma once

#include "Component.h"
#include "IRender.h"
#include "Line.h"

enum OBJECTDIRECTION
{
	OD_LEFT,
	OD_RIGHT,
	OD_END
};

class GameObject abstract : public Composite<GameObject>, public IRender
{
public:
	GameObject();
	virtual ~GameObject();

	virtual void	Init()				PURE;
	virtual int		Update()			PURE;
	//virtual void	Render()			PURE;
	virtual void	Late_Update()		PURE;

	virtual bool	IsObjectInWindow();
	bool IsObjectInWindow_Scroll();

	void			Update_Rect();
	virtual void	LineCollisionEvent() {};

protected:
	void			Move_Frame();
	virtual void	SelectFrameKey() PURE;

protected:
	RECT m_Area;
	Vec2 m_Pos;
	Vec2 m_Size;
	Vec2 m_Dir;

	INT m_Diameter;
	
	float m_fSpeed;
	float m_fFallingSpeed;
	float m_fJumpSpeed;

	bool m_IsInWindow;
	bool m_IsDead;
	bool m_IsFlying;
	
	Line* m_CurLine;

	bool	m_bJumping;
	float	m_fJumpTime;
	bool	m_bFalling;
	bool	m_bDescenting;

	FRAME		m_Frame;
	OBJID		m_ObjID;
	wstring		m_FrameKey;

	OBJECTDIRECTION m_eDirection;

public:
	float GetFallingSpeed() const		{ return m_fFallingSpeed; }
	void SetFallingSpeed(float speed)	{ m_fFallingSpeed = speed; }
	RECT GetArea()			const		{ return m_Area; }
	bool GetIsInWindow()	const		{ return m_IsInWindow; }
	void SetPos(Vec2 vec)				{ m_Pos = vec; }
	Vec2 GetPos()			const		{ return m_Pos; }
	void SetSize(const Vec2& size)		{ m_Size = size; }
	Vec2 GetSize()			const		{ return m_Size; }
	void SetDir(const Vec2& dir)		{ m_Dir = dir; }
	Vec2 GetDir()			const		{ return m_Dir; }
	void SetIsInWindow(bool IsInWindow)
	{ 
		m_IsInWindow = IsInWindow; 
	}
	void SetDead()					{ m_IsDead = TRUE; }
	void SetFlying(bool state)		{ m_IsFlying = state; }
	bool GetFlying() const			{ return m_IsFlying; }
	Line* GetCurLine() const		{ return m_CurLine; }
	void SetCurLine(Line* line)		{ m_CurLine = line; }
	bool GetJumping()	const		{ return m_bJumping; }
	void SetJumping(bool state)		{ m_bJumping = state; }
	float GetJumpTime()	const		{ return m_fJumpTime; }
	void SetJumpTime(float fTime)	{ m_fJumpTime = fTime; }
	bool GetFalling() const			{ return m_bFalling; }
	void SetFalling(bool state)		{ m_bFalling = state; }
	bool GetDescenting() const		{ return m_bDescenting; }
	void SetDescenting(bool state)	{ m_bDescenting = state; }
	wstring GetFrameKey() const		{ return m_FrameKey; }
	void SetFrameKey(const wstring& frameKey) { m_FrameKey = frameKey; }
	
	bool GetDead() const			{ return m_IsDead; }

	float GetSpeed() const			{ return m_fSpeed; }
	void SetSpeed(float speed)		{ m_fSpeed = speed; }
	void AddSpeed(float speed)		
	{ 
		if (speed > 0.f)
			m_fSpeed += speed; 
	}
	void SubSpeed(float speed) 
	{ 
		if (speed > 0.f)
			m_fSpeed -= speed; 

		if (m_fSpeed < 0.f)
			m_fSpeed = 0.f;
	}
	OBJECTDIRECTION GetObjectDir() const { return m_eDirection; }
	void SetObjectDir(OBJECTDIRECTION obd) { m_eDirection = obd; }
};


#pragma once
class ScrollManager
{
	DECLARE_SINGLE(ScrollManager)
public:
	void Init();
	void Update();

	float GetScrollX() const { return m_fScrollX; }
	void SetScrollX(float fX) { if (!m_bScrollLock) m_fScrollX += fX; }
	float GetScrollY() const { return m_fScrollY; }
	void SetScrollY(float fY) { if (!m_bScrollLock) m_fScrollY += fY; }

	void AdjustScrollX(float fX) { if (!m_bScrollLock) m_fScrollX = fX; }
	void AdjustScrollY(float fY) { if (!m_bScrollLock) m_fScrollY = fY; }

	void ScrollLock()
	{
		m_bScrollLock = true;
	}
	void ScrollUnLock()
	{
		m_bScrollLock = false;
	}
	bool GetScrollLock() const { return m_bScrollLock; }

	void ScrollClear() 
	{ 
		m_bScrollLock = false;
		m_fScrollX = 0.f; 
		m_fScrollY = 0.f;
	}

	void SetAutoScroll(bool state) { m_bAutoScroll = state; }
	

	void ScrollShake();
	void ScrollIntenseShake();

private:
	float m_fScrollX;
	float m_fScrollY;

	bool m_bScrollLock;
	DWORD m_dwShakeTime;
	DWORD m_dwIntenseShakeTime;
	LONG m_dwShakeSpeed;
	LONG m_dwIntenseShakeSpeed;
	bool m_bScrollShake;
	bool m_bScrollIntenseShake;
	int m_Shake;
	bool m_bAutoScroll;
	bool m_bIntenseShakeFinished;
public:
	bool GetIntenseShakeFinished() { return m_bIntenseShakeFinished; }

};

#define SCROLL				ScrollManager::GetInstance()
#define SCROLLX				SCROLL->GetScrollX()
#define SCROLLY				SCROLL->GetScrollY()
#define SET_SCROLLX(value)	SCROLL->SetScrollX(value)
#define SET_SCROLLY(value)	SCROLL->SetScrollY(value)
#define SCROLL_LOCK_STATE	SCROLL->GetScrollLock()
#define SCROLL_LOCK			SCROLL->ScrollLock()
#define SCROLL_UNLOCK		SCROLL->ScrollUnLock()
#define SCROLL_SHAKE		SCROLL->ScrollShake()
#define SCROLL_INTENSE_SHAKE SCROLL->ScrollIntenseShake()


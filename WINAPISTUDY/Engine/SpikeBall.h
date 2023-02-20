#pragma once
#include "ImageObject.h"

class Line;

#define FRAGMENT_NUM 20
#define ATTACK_BAR_SIZE 400

struct Fragment_Info
{
	float		power;
	Vec2		dir;
	float		speed;
	int			coordX;
	float		jumpTime;
	float		fallingSpeed;
	bool		jumping;
	Vec2		pos; // SpikeBall 아래쪽으로 진행
};

struct CommandQuiz_Info
{
	char	alpha;
	int		state;
	Vec2	pos;
	Vec2	renderPos;
};

class SpikeBall : public ImageObject
{
public:
	SpikeBall(int ZOrder, const TCHAR* frameKey, int coordX, int coordY, const Vec2& Pos, const Vec2& Size, const Vec2& RenderSize, bool collisionOn = false, int splitMode = 0);
	virtual ~SpikeBall();

	virtual void Render() override;
	virtual int Update() override;
	virtual void Late_Update() override;

public:
	virtual void Collision_Up(Collider* collider, const int& px, const int& py);
	virtual void Collision_Down(Collider* collider, const int& px, const int& py);
	virtual void Collision_Left(Collider* collider, const int& px, const int& py);
	virtual void Collision_Right(Collider* collider, const int& px, const int& py);

	void SetSpikeBallSpeedTime(LONG time) { m_dwSpikeBallSpeedTime = time; }
	void SubSpikeBallSpeedTime(LONG time) { m_dwSpikeBallSpeedTime -= time; }
	void SetSpikeBallSoundSpeed(LONG time) { m_dwSpikeBallSoundSpeed = time; }
	void SubSpikeBallSoundSpeed(LONG time) { m_dwSpikeBallSoundSpeed -= time; }

private:
	void Fragment_Jump();
	void SetFragmentInfo();
	void Move_FragmentInfo();
	void Render_CommandQuiz();
	void Render_TimeAttackBar();
	
	void Generate_CommandQuiz();
	void TimeLimit_CommandQuiz();
	void Key_Input();
	void NextQuiz(bool success);

private:
	LONG m_dwSpikeBallSoundTime;

	LONG m_dwSpikeBallSoundSpeed;
	LONG m_dwSpikeBallSpeedTime;
	Fragment_Info m_FragmentInfo[FRAGMENT_NUM];

	vector<CommandQuiz_Info> m_CommandQuiz;
	int m_CurQuizIndex;
	bool m_bNextQuiz;
	float m_fAddSpeed;
	int m_TimeAttackBarSize;

	int m_TimeAttackBarSpeedIndex;
	DWORD m_TimeAttackBarSpeed[2];
	DWORD m_TimeAttackBarTime;

	DWORD m_dwNextQuizTime;

public:
	float GetAddSpeed() const { return m_fAddSpeed; }
	

	
	
};


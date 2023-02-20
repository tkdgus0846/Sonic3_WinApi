#pragma once

#include "Define.h"

class SoundManager
{
	DECLARE_SINGLE(SoundManager)

public:
	void Init(); 
	void Release(); 

public:
	void PlaySound(const TCHAR* pSoundKey, CHANNELID eID, float fVolume);
	void PlayBGM(const TCHAR* pSoundKey, float fVolume);
	void StopSound(CHANNELID eID);
	void StopAll();
	void SetChannelVolume(CHANNELID eID, float fVolume);

	bool IsPlaying(CHANNELID eID);

private:
	void LoadSoundFile(); 

private:

	// 사운드 리소스 정보를 갖는 객체 
	map<TCHAR*, FMOD_SOUND*> m_mapSound; 
	
	// FMOD_CHANNEL : 재생하고 있는 사운드를 관리할 객체 
	FMOD_CHANNEL* m_pChannelArr[MAXCHANNEL]; 

	// 사운드 ,채널 객체 및 장치를 관리하는 객체 
	FMOD_SYSTEM* m_pSystem; 
};

#define SOUND SoundManager::GetInstance()
#define PLAY_SOUND(soundkey, channelID, volume) SOUND->PlaySound(soundkey, channelID, volume)
#define PLAY_BGM(soundKey, volume) SOUND->PlayBGM(soundKey, volume)
#define STOP_SOUND(channelID) SOUND->StopSound(channelID)
#define STOP_ALL_SOUND SOUND->StopAll()
#define SET_CHANNEL_VOLUME(channelID, volume) SOUND->SetChannelVolume(channelID, volume)

#define STOP_PLAY_SOUND(soundkey, channelID, volume)        \
STOP_SOUND(channelID);											\
PLAY_SOUND(soundkey, channelID, volume)							\

#define IS_PLAYING(channelID) SOUND->IsPlaying(channelID)



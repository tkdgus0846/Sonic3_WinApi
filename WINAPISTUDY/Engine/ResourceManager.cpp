#include "pch.h"
#include "ResourceManager.h"
#include "Image.h"
#include "Animation.h"

void ResourceManager::Init()
{
	m_ResourceFolder = L"../Resources";

	// 메인 로고
	Load_Image(L"Logo, Main", L"Logo");
	Load_Image(L"Logo, Main", L"MainMenu2");
	Load_Image(L"Logo, Main", L"Airplane");
	Load_Image(L"Logo, Main", L"MainYear");
	Load_Image(L"Logo, Main", L"MainBack");
	Load_Image(L"Logo, Main", L"MainTitle");
	Load_Image(L"Logo, Main", L"MainSonic");
	Load_Image(L"Logo, Main", L"Ending");
	Load_Image(L"Logo, Main", L"JusinEnding");

	// 소닉 가로64, 세로64
		// 왼 소닉
	Load_Image(L"Sonic", L"SonicL");
	Load_Image(L"Sonic", L"SonicL0");
	Load_Image(L"Sonic", L"SonicL45");
	Load_Image(L"Sonic", L"SonicL90");
	Load_Image(L"Sonic", L"SonicL135");
	Load_Image(L"Sonic", L"SonicL180");
	Load_Image(L"Sonic", L"SonicL225");
	Load_Image(L"Sonic", L"SonicL270");
	Load_Image(L"Sonic", L"SonicL315");

		// 오른 소닉
	Load_Image(L"Sonic", L"SonicR");
	Load_Image(L"Sonic", L"SonicR0");
	Load_Image(L"Sonic", L"SonicR45");
	Load_Image(L"Sonic", L"SonicR90");
	Load_Image(L"Sonic", L"SonicR135");
	Load_Image(L"Sonic", L"SonicR180");
	Load_Image(L"Sonic", L"SonicR225");
	Load_Image(L"Sonic", L"SonicR270");
	Load_Image(L"Sonic", L"SonicR315");

	Load_Image(L"Sonic", L"SuperSonicConvert");
	Load_Image(L"Sonic", L"SuperSonicFlyRight_1");
	Load_Image(L"Sonic", L"SuperSonicFlyRight_2");
	Load_Image(L"Sonic", L"SuperSonicFlyRight_3");
	Load_Image(L"Sonic", L"SuperSonicFlyRight_4");
	Load_Image(L"Sonic", L"SuperSonicFlyRight_5");
	Load_Image(L"Sonic", L"SuperSonicFlyRight_6");
	Load_Image(L"Sonic", L"Sparkle");

	Load_Image(L"Sonic", L"Skill1");
	Load_Image(L"Sonic", L"Skill2");
	Load_Image(L"Sonic", L"Skill3");
	Load_Image(L"Sonic", L"Skill4");
	Load_Image(L"Sonic", L"Skill5");
	Load_Image(L"Sonic", L"Skill6");

	// 배경
	Load_Image(L"TileMap", L"MushroomBack");
	Load_Image(L"TileMap", L"LavareepBack");

	// 타일 이미지 - 지형들은 한조각이 128x128
	Load_Image(L"TileMap", L"LavareefTile");
	Load_Image(L"TileMap", L"LavareefTileMask");
	Load_Image(L"TileMap", L"MushroomTile");
	Load_Image(L"TileMap", L"MushroomTileMask");

	// 가시 31*31
	Load_Image(L"Obj", L"Spike");
	Load_Image(L"Obj", L"Roller");
	Load_Image(L"Obj", L"Spring");
	Load_Image(L"Obj", L"Sign");
	Load_Image(L"Obj", L"Ring");
	Load_Image(L"Obj", L"Ring2"); 
	Load_Image(L"Obj", L"DropMushroom");
	Load_Image(L"Obj", L"Mushroom");

	// 28 32
	Load_Image(L"Obj", L"Item");
	Load_Image(L"Obj", L"Stone1");
	Load_Image(L"Obj", L"BGMushroom1");
	Load_Image(L"Obj", L"BGMushroom2");
	Load_Image(L"Obj", L"BGMushroom3");
	Load_Image(L"Obj", L"DiagonalSpring");
	Load_Image(L"Obj", L"SpikeBall");
	Load_Image(L"Obj", L"ShineEffect");
	Load_Image(L"Obj", L"FragmentEffect");
	Load_Image(L"Obj", L"BoomEffect");
	Load_Image(L"Obj", L"DeathEffect");

	// UI
	Load_Image(L"UI", L"UI");
	// 80x6
	Load_Image(L"UI", L"SmallNumber");
	// 80x11
	Load_Image(L"UI", L"Number");

	// 320 240
	Load_Image(L"UI", L"Start1");
	Load_Image(L"UI", L"Start2");

	// 211 117
	Load_Image(L"UI", L"End1");
	Load_Image(L"UI", L"End2");

	// Bonus Stage
	Load_Image(L"BonusStage", L"BonusBackground");
	Load_Image(L"BonusStage", L"ForwardAndRotate");
	Load_Image(L"BonusStage", L"ForwardAndRotate2");
	Load_Image(L"BonusStage", L"BonusSonicLeft");
	Load_Image(L"BonusStage", L"BonusSonicRight");

	// DoomsDay
	Load_Image(L"DoomsDay", L"Missile");
	Load_Image(L"DoomsDay", L"BigMeteor");
	Load_Image(L"DoomsDay", L"NormalMeteor");
	Load_Image(L"DoomsDay", L"SmallMeteor");
	Load_Image(L"DoomsDay", L"MeteorFragment1");
	Load_Image(L"DoomsDay", L"MeteorFragment2");
	Load_Image(L"DoomsDay", L"Rocket1");
	Load_Image(L"DoomsDay", L"Rocket2");
	Load_Image(L"DoomsDay", L"Trail");
	Load_Image(L"DoomsDay", L"DoomsDay");
	Load_Image(L"DoomsDay", L"BGLayer1");
	Load_Image(L"DoomsDay", L"BGLayer2");
	Load_Image(L"DoomsDay", L"BGLayer3");
	Load_Image(L"DoomsDay", L"BGLayer4");
	Load_Image(L"DoomsDay", L"BGLayer5");
	Load_Image(L"DoomsDay", L"BGLayer6");
	Load_Image(L"DoomsDay", L"BGLayer7");
	Load_Image(L"DoomsDay", L"BGLayer8");

	Load_Image(L"DoomsDay", L"LeftFlame");
	Load_Image(L"DoomsDay", L"LeftDownFlame");
	Load_Image(L"DoomsDay", L"LeftUpFlame");
	Load_Image(L"DoomsDay", L"UpFlame");
	Load_Image(L"DoomsDay", L"DownFlame");
	Load_Image(L"DoomsDay", L"RightFlame");
	Load_Image(L"DoomsDay", L"RightUpFlame");
	Load_Image(L"DoomsDay", L"RightDownFlame");

	// 32x24
	Load_Image(L"MarioConcept", L"Cloud1");
	// 64x24
	Load_Image(L"MarioConcept", L"Cloud2");
	// 16x16
	Load_Image(L"MarioConcept", L"CloudTile");

	// 32x32
	Load_Image(L"MarioConcept", L"Pipe");

	Load_Image(L"UI", L"BoxFont_Black");
	Load_Image(L"UI", L"BoxFont_Green");
	Load_Image(L"UI", L"BoxFont_Red");
}

void ResourceManager::Release()
{
	for (auto item : m_ImageMap)
	{
		Safe_Delete(&item.second);
	}

	for (auto item : m_AnimationMap)
	{
		Safe_Delete(&item.second);
	}
}

void ResourceManager::Load_Image(const wstring& folder, const wstring& name)
{
	auto it = m_ImageMap.find(name);
	if (it != m_ImageMap.end()) return;

	Image* image = new Image();

	wstring ImageDirectory = m_ResourceFolder + L"/Images/" + folder + L"/" + name + L".bmp";
	image->Load(ImageDirectory);
	image->SetPath(ImageDirectory);
	image->SetKey(name);

	m_ImageMap.insert({ name, image });
}

HDC ResourceManager::Find_Image(const wstring& name)
{
	Image* image = m_ImageMap[name];
	HDC hDC = nullptr;
	if (image)
	{
		hDC = image->Get_MemDC();
	}
	
	return hDC;
}

void ResourceManager::Load_Animation(const wstring& name, int iFrameStart, int iFrameEnd, int iMotion, DWORD dwSpeed, DWORD dwTime, bool repeat)
{
	auto it1 = m_AnimationMap.find(name);
	auto it2 = m_ImageMap.find(name);
	if (it1 != m_AnimationMap.end()) return;
	if (it2 == m_ImageMap.end()) return;

	Animation* animation = new Animation();


	animation->Create((*it2).second, iFrameStart, iFrameEnd, iMotion, dwSpeed, dwTime, repeat);
	animation->SetKey(name);

	m_AnimationMap.insert({ name, animation });
}

void ResourceManager::Load_Animation(const wstring& name, const FRAME& frame, bool repeat)
{
	Load_Animation(name, { frame.iFrameStart, frame.iFrameEnd, frame.iMotion, frame.dwSpeed, frame.dwTime }, repeat);
}

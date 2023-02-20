#pragma once

class Image;
class Animation;

class ResourceManager
{
	DECLARE_SINGLE(ResourceManager)

public:
	void Init();
	void Release();

	void Load_Image(const wstring& folder, const wstring& name);
	HDC Find_Image(const wstring& name);

	void Load_Animation(const wstring& name, int iFrameStart, int iFrameEnd, int iMotion, DWORD dwSpeed, DWORD dwTime, bool repeat = true);
	void Load_Animation(const wstring& name, const FRAME& frame, bool repeat = true);


private:
	map<wstring, Image*> m_ImageMap;
	map<wstring, Animation*> m_AnimationMap;

	wstring m_ResourceFolder;
	
};

#define RESOURCE ResourceManager::GetInstance()

#define FIND_IMAGE(name) RESOURCE->Find_Image(name)


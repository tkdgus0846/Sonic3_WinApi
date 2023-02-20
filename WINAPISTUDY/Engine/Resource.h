#pragma once

class Resource
{
public:
	Resource();
	virtual ~Resource();

public:
	const wstring& GetKey() const { return m_Key; }
	void SetKey(const wstring& key) { m_Key = key; }
	const wstring& GetPath() const { return m_Path; }
	void SetPath(const wstring& path) { m_Path = path; }

private:
	wstring m_Key;
	wstring m_Path;
};


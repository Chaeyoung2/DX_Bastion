#pragma once

#include "Include.h"
#include "Logo.h"

class CTexture;
class CTextureMgr
{
	DECLARE_SINGLETON(CTextureMgr);

private:
	map<wstring, CTexture*>		m_mapTexture;
	typedef	map<wstring, CTexture*>		MAPTEXTURE;

	wstring								m_wstrFullPath;


public:
	void		SetName(const wstring& wstrFullPath) { m_wstrFullPath = wstrFullPath; }
	const wstring& GetFilePath(void) { return m_wstrFullPath; }

public:
	HRESULT LoadImagePath(wstring wstrImagePath);


public:
	HRESULT	InsertTexture(const wstring& wstrFilePath, 
		const wstring& wstrObjKey, 
		eTextureType eeTextureType,
		const wstring& wstrStateKey = L"", 
		const int& iCount = 0) ;

	const TEX_INFO*	GetTexture( const wstring& wstrObjKey, 
		const wstring& wstrStateKey = L"", 
		const int& iCount = 0) ;

	void Release();

	size_t GetImageCnt(const wstring& wstrObjKey, const wstring& wstrStateKey);


private:
	wstring		m_wstrCurrnetPath;		//로딩 화면 출력용

public:
	void SetCurrentPath(wstring& rPath)
	{
		m_wstrCurrnetPath = rPath;
	}

	const wstring& GetCurrentPath(void)
	{
		return m_wstrCurrnetPath;
	}


public:
	CTextureMgr(void);
	~CTextureMgr(void);
};

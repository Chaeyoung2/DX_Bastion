#pragma once
#include "Texture.h"

class CMultiTexture : public CTexture
{
private:
	typedef	vector<TEX_INFO*>					VECTEX_INFO;
	typedef map<wstring, VECTEX_INFO>			MAPTEX_INFO;
	MAPTEX_INFO									m_MapMultiTexture;

public:
	virtual HRESULT InsertTexture(const wstring& wstrFilePath, 
		const wstring& wstrStateKey = L"", 
		const int& iCount = 0);

	virtual const TEX_INFO* GetTexture(const wstring& wstrStateKey = L"", 
		const int& iCount = 0);

	size_t	GetImageCnt(const wstring& wstrStateKey);

	virtual void Release();

public:
	CMultiTexture(void);
	~CMultiTexture(void);
};

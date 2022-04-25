#pragma once
#include "texture.h"

class CSingleTexture :
	public CTexture
{
public:
	virtual HRESULT InsertTexture(const wstring& wstrFilePath, 
		const wstring& wstrStateKey = L"", 
		const int& iCount = 0);

	virtual const TEX_INFO* GetTexture(const wstring& wstrStateKey = L"", 
		const int& iCount = 0);
	virtual void Release(void);

private:
	TEX_INFO*				m_pTEX_INFO;

public:
	CSingleTexture(void);
	virtual ~CSingleTexture(void);
};

#pragma once

#include "Include.h"

class CDevice;
class CTexture
{
protected:
	CDevice*		m_pDevice;

public:
	virtual	HRESULT	InsertTexture(const wstring& wstrFilePath, 
		const wstring& wstrStateKey = L"", 
		const int& iCount = 0) PURE;

	virtual const TEX_INFO*	GetTexture(const wstring& wstrStateKey = L"", 
		const int& iCount = 0) PURE;

	virtual void Release()PURE;

	CTexture(void);
	virtual ~CTexture(void);
};

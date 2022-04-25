#pragma once

#include "Include.h"

class CTexture
{
public:
	virtual HRESULT InsertTexture(const TCHAR* pPaht,		//���
		const TCHAR* pStateKey = L"",	//�̹��� ����(Ű��)
		const int& iCount = 0)PURE;		//���° �̹��� ����?

	virtual const TEX_INFO* GetTexture(const TCHAR* pKey = L"",
		const int& iCount = 0)PURE;

	virtual void Release(void)PURE;

public:
	CTexture(void);
	virtual ~CTexture(void);
};

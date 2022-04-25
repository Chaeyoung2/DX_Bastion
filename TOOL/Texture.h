#pragma once

#include "Include.h"

class CTexture
{
public:
	virtual HRESULT InsertTexture(const TCHAR* pPaht,		//경로
		const TCHAR* pStateKey = L"",	//이미지 상태(키값)
		const int& iCount = 0)PURE;		//몇번째 이미지 인지?

	virtual const TEX_INFO* GetTexture(const TCHAR* pKey = L"",
		const int& iCount = 0)PURE;

	virtual void Release(void)PURE;

public:
	CTexture(void);
	virtual ~CTexture(void);
};

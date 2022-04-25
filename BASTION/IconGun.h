#pragma once
#include "ui.h"

class CIconGun :
	public CUI
{
private:
	bool		m_bIsCarryingGun;

public:
	virtual HRESULT Initialize(void);
	virtual eOBJECT_RESULT Update(void);
	virtual void Render(void);
	virtual void Release(void);

public:
	CIconGun(void);
	virtual ~CIconGun(void);
};

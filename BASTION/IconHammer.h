#pragma once
#include "ui.h"

class CIconHammer :
	public CUI
{
private:
	bool		m_bIsCarryingHammer;

public:
	virtual HRESULT Initialize(void);
	virtual eOBJECT_RESULT Update(void);
	virtual void Render(void);
	virtual void Release(void);

public:
	CIconHammer(void);
	virtual ~CIconHammer(void);
};

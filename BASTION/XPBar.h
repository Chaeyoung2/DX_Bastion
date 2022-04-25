#pragma once
#include "ui.h"

class CXPBar :
	public CUI
{
public:
	CXPBar(void);
	virtual ~CXPBar(void);

private:
	int			m_iXp;

public:
	virtual HRESULT Initialize(void);
	virtual eOBJECT_RESULT Update(void);
	virtual void Render(void);
	virtual void Release(void);

};

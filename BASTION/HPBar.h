#pragma once
#include "ui.h"

class CHPBar :
	public CUI
{
private:
	//CGameObject*	m_pPlayer;
	int				m_iHp;
	int				m_iHpMax;
	float			m_fHpValue;

	float			m_fWidth;

public:
	CHPBar(void);
	virtual ~CHPBar(void);

public:
	virtual HRESULT Initialize(void);
	virtual eOBJECT_RESULT Update(void);
	virtual void Render(void);
	virtual void Release(void);


};

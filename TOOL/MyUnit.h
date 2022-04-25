#pragma once

#include "Include.h"

class CTOOLView;
class CMyUnit
{
private:
	list<PMYUNIT>	m_listUnit;
	CTOOLView*		m_pMainView;

public:
	void SetMainView(CTOOLView* pMainView) { m_pMainView = pMainView; }

public:
	list<PMYUNIT>*	GetUnitList() { return &m_listUnit; }

public:
	HRESULT			Initialize();
	int				Update();
	void			Render();
	void			Release();

	void			SetUnit(D3DXVECTOR3 vMousePos, int iUnitID, int iDir);

	void			MiniRender();
public:
	CMyUnit(void);
	~CMyUnit(void);
};

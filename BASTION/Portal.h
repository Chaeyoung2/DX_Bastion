#pragma once
#include "gameobject.h"

class CPortal :
	public CGameObject
{
public:
	CPortal(void);
	virtual ~CPortal(void);

public:
	virtual HRESULT			Initialize(void);
	virtual eOBJECT_RESULT	Update(void);
	virtual void			Render(void);
	virtual void			Release(void);

	void					SettingCollRect();


public:
	void					SetWhereToGo(int iWhereToGo)
	{
		m_iWhereToGo = iWhereToGo;
	}

private:
	RECT					m_tRect;
	int						m_iWhereToGo;
};

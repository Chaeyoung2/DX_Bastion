#pragma once
#include "gameobject.h"

class CMonBullet :
	public CGameObject
{
public:
	CMonBullet(void);
	virtual ~CMonBullet(void);

public:
	virtual HRESULT			Initialize(void);
	virtual eOBJECT_RESULT	Update(void);
	virtual void			Render(void);
	virtual void			Release(void);

	void					MatrixCheck();
	void					FrameCheck();

	void					SettingCollRect();

private:
	FRAME			m_tFrame;
	wstring			m_wstrStateKey;
};
